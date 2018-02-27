/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gamepad.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:52:46 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMEPAD_HPP
# define GAMEPAD_HPP

# include "glfw3.h"
# include "Exceptions/GeneralException.hpp"
# include <fstream>
# include <iostream>
# include <sstream>
# include <iomanip>

# define MAX_GAMEPAD 16
# define MOV_DEAD_ZONE 0.25f
# define CAM_DEAD_ZONE 0.10f

typedef GLFWgamepadstate GamepadState;

class Gamepad
{
	public :

		Gamepad(size_t refreshed_gamepad_nb);
		virtual ~Gamepad(void);
		Gamepad(Gamepad const &src) = delete;
		Gamepad &operator=(Gamepad const &rhs) = delete;

		static void updateGamepadMapping(std::string const &file);
		void pollGamepads(void);
		bool isGamepadConnected(unsigned char joy_nb) const;
		void printJoystickInfo(unsigned char joy_nb) const;

		/*
		 * Getter
		 */

		GLFWgamepadstate const &getGamepadState(unsigned char joy_nb) const;


		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException(void);
				explicit FileOpenException(std::string const &path);
				virtual ~FileOpenException(void) throw();
		};

		class TooMuchGamepadException : public GeneralException
		{
			public :

				explicit TooMuchGamepadException(void);
				virtual ~TooMuchGamepadException(void) throw();
		};

	private :

		size_t           _refreshed_gamepad_nb;
		GLFWgamepadstate _state[MAX_GAMEPAD + 1];
};

#endif