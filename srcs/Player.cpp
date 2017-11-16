/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"

Player::Player(Shader const *shader, glm::mat4 const *perspec_mult_view,
			   glm::vec3 const &pos, glm::vec3 const &size,
			   ThirdPersonCamera const *cam, Texture const *tex) :
		_cam(cam), _model(shader, perspec_mult_view, tex, pos, size),
		_cb(pos, size), _pos(pos), _vel(glm::vec3({0.0f, 0.0f, 0.0f})),
		_acc(glm::vec3({0.0f, 0.0f, 0.0f})), _mass(1.0f)
{
	this->update(1.0f);
}

Player::~Player(void)
{
}

glm::vec3 const &Player::getPos(void) const
{
	static_cast<void>(this->_mass);
	return (this->_pos);
}

bool Player::update_keyboard_interaction(Input const &input, float input_timer)
{
	float velocity = 0.05f;
	bool  toogle   = false;

	static_cast<void>(input_timer);
	if (this->_cam != nullptr)
	{
		if (input.p_key[GLFW_KEY_W] == PRESSED)
		{
			this->_pos += velocity * this->_cam->getFront();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_S] == PRESSED)
		{
			this->_pos -= velocity * this->_cam->getFront();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_D] == PRESSED)
		{
			this->_pos += velocity * this->_cam->getRight();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_A] == PRESSED)
		{
			this->_pos -= velocity * this->_cam->getRight();
			toogle = true;
		}
		if (toogle == true)
			return (true);
	}
	else
		std::cout << "Player : Can't update keyboard interaction" << std::endl;
	return (false);
}

bool Player::update_mouse_interaction(Input const &input, GLFW_Window const &win, glm::vec3 const &origin,
									  std::vector<glm::vec3 const *> const &axes, float input_timer)
{
	static_cast<void>(input);
	static_cast<void>(win);
	static_cast<void>(origin);
	static_cast<void>(axes);
	static_cast<void>(input_timer);
	return (false);
}

void Player::update(float time)
{
	this->_model.update(time);
	this->_model.setPosition(this->_pos);
}

void Player::draw(void)
{
	this->_model.draw();
}