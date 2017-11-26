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
			   Texture const *tex, glm::vec4 const &sprite_tex_size_offset,
			   size_t nb_walk_frame, bool draw_cb) :
		_cam(cam), _model(shader, perspec_mult_view, tex, pos, size,
						  sprite_tex_size_offset, nb_walk_frame),
		_cb_model(cb_shader, perspec_mult_view, cb_tex, pos, size),
		_cb(pos, size), _delta(glm::vec3({0.0f, 0.0f, 0.0f})), _pos(pos),
		_vel(glm::vec3({0.0f, 0.0f, 0.0f})), _acc(glm::vec3({0.0f, 0.0f, 0.0f})),
		_on_surface(false), _surface_cb(glm::vec3{0.0f, 0.0f, 0.0f},
										glm::vec3{0.0f, 0.0f, 0.0f}),
		_delay_jump(false), _friction(0.00001f), _force(100.f), _draw_cb(draw_cb),
		_dir(Player::BACK), _axis(glm::ivec2{0, 0}), _total_walked(0.0f),
		_cur_jump(2), _max_jump(2), _hoover(true), _cur_hoover_time(10.0f),
		_max_hoover_time(10.0f)
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

void Player::setSpriteYaw(float yaw)
{
	this->_model.setYaw(yaw);
}

void Player::setCurJumpToMax(void)
{
	this->_cur_jump = this->_max_jump;
}

void Player::setCurHooverTimeToMax(void)
{
	this->_cur_hoover_time = this->_max_hoover_time;
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

size_t Player::getMaxJump(void) const
{
	return (this->_max_jump);
}

bool Player::getCanHoover(void) const
{
	return (this->_hoover);
}

float Player::getMaxHooverTime(void) const
{
	return (this->_max_hoover_time);
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
	bool toogle     = false;
	bool change_dir = false;

	static_cast<void>(input_timer);
	if (this->_cam != nullptr)
	{
		this->_acc.x  = 0.0f;
		this->_acc.z  = 0.0f;
		this->_acc.y  = 0.0f;
		this->_axis.x = 0;
		this->_axis.y = 0;
		if (input.p_key[GLFW_KEY_SPACE] == RELEASED)
		{
			this->_hoover = true;
		}
		if (input.p_key[GLFW_KEY_W] == PRESSED)
		{
			this->_acc += this->_force * this->_cam->getXYFront();
			this->_axis.x += 1;
			toogle     = true;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_S] == PRESSED)
		{
			this->_acc -= this->_force * this->_cam->getXYFront();
			this->_axis.x -= 1;
			toogle     = true;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_D] == PRESSED)
		{
			this->_acc += this->_force * this->_cam->getRight();
			this->_axis.y += 1;
			toogle     = true;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_A] == PRESSED)
		{
			this->_acc -= this->_force * this->_cam->getRight();
			this->_axis.y -= 1;
			toogle     = true;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_SPACE] == PRESSED && this->_cur_jump)
		{
			this->_acc += this->_force * 10.0f * this->_cam->getWorldUp();
			toogle = true;
			(this->_cur_jump)--;
			if (this->_cur_jump > 0 || this->_max_hoover_time == 0.0f)
				const_cast<Input &>(input).p_key[GLFW_KEY_SPACE] = RELEASED;
			this->_on_surface = false;
			this->_delay_jump = true;

		}
		if (input.p_key[GLFW_KEY_SPACE] == PRESSED && !this->_cur_jump &&
			this->_cur_hoover_time > 0.0f)
		{
			this->_hoover = false;
		}
		if (change_dir)
			this->_set_sprite_direction();
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
	this->_total_walked += glm::l2Norm(this->_delta);
	this->_cb_model.setPosition(this->_pos);
	this->_model.setPosition(this->_pos);
	this->_cb.setPos(this->_pos);
	this->_model.setSpriteY(this->_dir);
	this->_model.setSpriteX(static_cast<size_t>(this->_total_walked) %
							this->_model.getNbOfWalkFrame());
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
			this->_acc += (this->_hoover) ? vec_gravity : (vec_gravity * 0.5f);
		this->_delay_jump = false;
	}
//	if (!this->_hoover)

	this->_vel += this->_acc * delta;
	this->_vel *= std::pow(this->_friction, delta);
	this->_delta = this->_vel * delta;
}

void Player::_set_sprite_direction(void)
{
	if (this->_axis.x == 0 && this->_axis.y == 0)
		return;
	else if (this->_axis.x == 1 && this->_axis.y == 0)
		this->_dir = Player::BACK;
	else if (this->_axis.x == -1 && this->_axis.y == 0)
		this->_dir = Player::FRONT;
	else if (this->_axis.x == 0 && this->_axis.y == -1)
		this->_dir = Player::RIGHT;
	else if (this->_axis.x == 0 && this->_axis.y == 1)
		this->_dir = Player::LEFT;
	else if (this->_axis.x == -1 && this->_axis.y == 1)
		this->_dir = Player::FRONT_LEFT;
	else if (this->_axis.x == -1 && this->_axis.y == -1)
		this->_dir = Player::FRONT_RIGHT;
	else if (this->_axis.x == 1 && this->_axis.y == 1)
		this->_dir = Player::BACK_LEFT;
	else if (this->_axis.x == 1 && this->_axis.y == -1)
		this->_dir = Player::BACK_RIGHT;
}