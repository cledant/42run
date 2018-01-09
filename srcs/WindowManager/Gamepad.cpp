/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gamepad.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WindowManager/Gamepad.hpp"

Gamepad::Gamepad(size_t refreshed_gamepad_nb) : _refreshed_gamepad_nb(refreshed_gamepad_nb)
{
	if (this->_refreshed_gamepad_nb > MAX_GAMEPAD)
		throw Gamepad::TooMuchGamepadException();
	std::memset(&this->_state, 0, sizeof(GLFWgamepadstate) * (MAX_GAMEPAD + 1));
}

Gamepad::~Gamepad(void)
{
}

void Gamepad::updateGamepadMapping(std::string const &file)
{
	std::fstream fs;
	std::string  content;

	std::cout << "Loading : " << file << std::endl;
	try
	{
		fs.exceptions(std::fstream::failbit | std::fstream::badbit);
		fs.open(file, std::fstream::in);
		content.assign((std::istreambuf_iterator<char>(fs)),
					   std::istreambuf_iterator<char>());
		fs.close();
	}
	catch (std::exception &e)
	{
		throw Gamepad::FileOpenException(file);
	}
	glfwUpdateGamepadMappings(content.c_str());
}

void Gamepad::pollGamepads(void)
{
	for (size_t i = 0; i < MAX_GAMEPAD; ++i)
	{
		if (glfwJoystickPresent(i) && glfwJoystickIsGamepad(i))
			glfwGetGamepadState(i, &this->_state[i]);
	}
}

bool Gamepad::isGamepadConnected(unsigned char joy_nb) const
{
	if (joy_nb >= MAX_GAMEPAD)
		return (false);
	if (!glfwJoystickPresent(joy_nb) || !glfwJoystickIsGamepad(joy_nb))
		return (false);
	return (true);
}

void Gamepad::printJoystickInfo(unsigned char joy_nb) const
{
	char const *joy_guid = NULL;
	char const *joy_name = NULL;

	if (joy_nb >= MAX_GAMEPAD)
	{
		std::cout << "Invalid Joystick" << std::endl;
		return;
	}
	if (!glfwJoystickPresent(joy_nb))
	{
		std::cout << "No joystick or gamepad connected" << std::endl;
		return;
	}
	std::cout << "Joystick " << joy_nb << " : ";
	if (!(joy_name = glfwGetJoystickName(joy_nb)))
		std::cout << "Unknown Joystick name : ";
	else
		std::cout << joy_name << " : ";
	if (glfwJoystickIsGamepad(joy_nb))
		std::cout << "Is a Gamepad : ";
	if ((joy_guid = glfwGetJoystickGUID(joy_nb)) == NULL)
	{
		std::cout << "Unknowm GUID" << std::endl;
		return;
	}
	else
		std::cout << joy_guid << std::endl;
}

GLFWgamepadstate const &Gamepad::getGamepadState(unsigned char joy_nb) const
{
	if (joy_nb >= MAX_GAMEPAD || !glfwJoystickPresent(joy_nb) ||
		!glfwJoystickIsGamepad(joy_nb))
	{
		std::cout << "Invalid Gamepad requested" << std::endl;
		return (this->_state[MAX_GAMEPAD]);
	}
	return (this->_state[joy_nb]);
}

Gamepad::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Gamepad : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Gamepad::FileOpenException::FileOpenException(void)
{
	this->_msg = "Gamepad : Failed to find to open file";
}

Gamepad::FileOpenException::~FileOpenException(void) throw()
{
}

Gamepad::TooMuchGamepadException::TooMuchGamepadException(void)
{
	this->_msg = "Gamepad : Too much gamepad to refresh";
}

Gamepad::TooMuchGamepadException::~TooMuchGamepadException(void) throw()
{
}