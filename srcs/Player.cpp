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

Player::Player(Shader const *cb_shader, Shader const *shader,
			   glm::mat4 const *perspec_mult_view,
			   glm::vec3 const &pos, glm::vec3 const &size,
			   ThirdPersonCamera const *cam, Texture const *cb_tex,
			   Texture const *tex, glm::ivec2 sprite, bool draw_cb) :
		_cam(cam), _model(shader, perspec_mult_view, tex, pos, size, sprite.x, sprite.y),
		_cb_model(cb_shader, perspec_mult_view, cb_tex, pos, size),
		_cb(pos, size), _delta(glm::vec3({0.0f, 0.0f, 0.0f})), _pos(pos),
		_vel(glm::vec3({0.0f, 0.0f, 0.0f})), _acc(glm::vec3({0.0f, 0.0f, 0.0f})),
		_on_surface(false), _surface_cb(glm::vec3{0.0f, 0.0f, 0.0f},
										glm::vec3{0.0f, 0.0f, 0.0f}),
		_delay_jump(false), _friction(0.00001f), _force(100.f), _draw_cb(draw_cb)
{
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

void Player::setSurfaceCollisionBox(CollisionBox const &cb)
{
	this->_surface_cb = cb;
}

void Player::setOnSurface(bool flag)
{
	this->_on_surface = flag;
}

void Player::setVelocity(glm::vec3 const &vel)
{
	this->_vel = vel;
}

glm::vec3 const &Player::getDelta(void) const
{
	return (this->_delta);
}

glm::vec3 const &Player::getPos(void) const
{
	return (this->_pos);
}

bool Player::getOnSurface(void) const
{
	return (this->_on_surface);
}

void Player::update_model(float time)
{
	this->_cb_model.update(time);
	this->_model.update(time);
}

CollisionBox const &Player::getCollisionBox(void) const
{
	return (this->_cb);
}

bool Player::update_keyboard_interaction(Input const &input, float input_timer)
{
	bool toogle = false;

	static_cast<void>(input_timer);
	if (this->_cam != nullptr)
	{
		this->_acc.x = 0.0f;
		this->_acc.z = 0.0f;
		this->_acc.y = 0.0f;
		if (input.p_key[GLFW_KEY_W] == PRESSED)
		{
			this->_acc += this->_force * this->_cam->getXYFront();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_S] == PRESSED)
		{
			this->_acc -= this->_force * this->_cam->getXYFront();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_D] == PRESSED)
		{
			this->_acc += this->_force * this->_cam->getRight();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_A] == PRESSED)
		{
			this->_acc -= this->_force * this->_cam->getRight();
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_SPACE] == PRESSED && this->_on_surface)
		{
			this->_acc += this->_force * 10.0f * this->_cam->getWorldUp();
			toogle = true;
			this->_on_surface = false;
			this->_delay_jump = true;
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
	this->_cb_model.setPosition(this->_pos);
	this->_model.setPosition(this->_pos);
	this->_cb.setPos(this->_pos);
}

void Player::draw(void)
{
	if (this->_draw_cb)
		this->_cb_model.draw();
	this->_model.draw();
}

void Player::update_gravity(glm::vec3 const &vec_gravity, float delta)
{
	if (!this->_cb.IsBoxOnBox(this->_surface_cb) || !this->_on_surface)
	{
		this->_on_surface = false;
		if (!this->_delay_jump)
			this->_acc += vec_gravity;
		this->_delay_jump = false;
	}
	this->_vel += this->_acc * delta;
	this->_vel *= std::pow(this->_friction, delta);
	this->_delta = this->_vel * delta;
}
