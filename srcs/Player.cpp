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
		_cb(pos, size), _delta(glm::vec3({0.0f, 0.0f, 0.0f})), _pos(pos),
		_vel(glm::vec3({0.0f, 0.0f, 0.0f})),
		_acc(glm::vec3({0.0f, 0.0f, 0.0f})), _mass(1.0f)
{
	static_cast<void>(this->_mass);
	this->update(1.0f);
}

Player::~Player(void)
{
}

void Player::setPos(glm::vec3 const &new_pos)
{
	this->_pos = new_pos;
}

void Player::setDelta(glm::vec3 const &new_delta)
{
	this->_delta = new_delta;
}

glm::vec3 const &Player::getDelta(void) const
{
	return (this->_delta);
}

glm::vec3 const &Player::getOldPos(void) const
{
	return (this->_old_pos);
}

glm::vec3 const &Player::getPos(void) const
{
	return (this->_pos);
}

void Player::update_model(float time)
{
	this->_model.update(time);
}

CollisionBox const &Player::getCollisionBox(void) const
{
	return (this->_cb);
}

bool Player::update_keyboard_interaction(Input const &input, float input_timer)
{
	float velocity = 0.09f;
	bool  toogle   = false;

	static_cast<void>(input_timer);
	if (this->_cam != nullptr)
	{
		this->_delta.x = 0.0f;
		this->_delta.z = 0.0f;
		this->_delta.y = 0.0f;
		if (input.p_key[GLFW_KEY_W] == PRESSED)
		{
			this->_delta += velocity * this->_cam->getXYFront();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_S] == PRESSED)
		{
			this->_delta -= velocity * this->_cam->getXYFront();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_D] == PRESSED)
		{
			this->_delta += velocity * this->_cam->getRight();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_A] == PRESSED)
		{
			this->_delta -= velocity * this->_cam->getRight();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_R] == PRESSED)
		{
			this->_delta += velocity * this->_cam->getWorldUp();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_F] == PRESSED)
		{
			this->_delta += -velocity * this->_cam->getWorldUp();
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
	(void) time;
	this->_pos += this->_delta;
	this->_model.setPosition(this->_pos);
	this->_cb.setPos(this->_pos);
}

void Player::draw(void)
{
	this->_model.draw();
}

void Player::update_gravity(glm::vec3 const &vec_gravity, float delta)
{
	(void) delta;
	(void) vec_gravity;
}
