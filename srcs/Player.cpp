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

#include <Glfw_manager.hpp>
#include "Player.hpp"

Player::Params::Params(void)
{
	this->cb_shader              = nullptr;
	this->shader                 = nullptr;
	this->perspec_mult_view      = nullptr;
	this->pos                    = glm::vec3(0.0f, 0.0f, 0.0f);
	this->size                   = glm::vec3(0.0f, 0.0f, 0.0f);
	this->cam                    = nullptr;
	this->cb_tex                 = nullptr;
	this->tex                    = nullptr;
	this->sprite_tex_size_offset = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	this->nb_walk_frame          = 0;
	this->draw_cb                = false;
	this->max_jump               = 1;
	this->max_hoover_time        = 2.0f;
	this->hp                     = 10;
	this->max_immunity           = 2.0f;
	this->audio                  = nullptr;
	this->theme                  = Audio::THEME_1;
}

Player::Params::~Params(void)
{
}

Player::Player(Player::Params const &params) :
		_cam(params.cam), _model(params.shader, params.perspec_mult_view,
								 params.tex, params.pos, params.size,
								 params.sprite_tex_size_offset,
								 params.nb_walk_frame),
		_cb_model(params.cb_shader, params.perspec_mult_view, params.cb_tex,
				  params.pos, params.size),
		_cb(params.pos, params.size), _delta(glm::vec3({0.0f, 0.0f, 0.0f})),
		_pos(params.pos), _vel(glm::vec3({0.0f, 0.0f, 0.0f})),
		_acc(glm::vec3({0.0f, 0.0f, 0.0f})), _on_surface(false),
		_surface_cb(glm::vec3{0.0f, 0.0f, 0.0f},
					glm::vec3{0.0f, 0.0f, 0.0f}), _delay_jump(false),
		_friction(0.00001f), _force(100.f), _draw_cb(params.draw_cb),
		_dir(Player::BACK), _axis(glm::ivec2{0, 0}), _total_walked(0.0f),
		_cur_jump(params.max_jump), _max_jump(params.max_jump), _hoover(false),
		_cur_hoover_time(params.max_hoover_time),
		_max_hoover_time(params.max_hoover_time), _hp(params.hp),
		_cur_immunity(0.0f), _max_immunity(params.max_immunity), _audio(params.audio),
		_theme(params.theme), _last_jump(GLFW_RELEASE)
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

void Player::lowerHP(ICollidable::Damages type)
{
	this->_hp -= type;
}

void Player::setImmunityTimerToMax(void)
{
	this->_cur_immunity = this->_max_immunity;
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

float Player::getTotalWalked() const
{
	return (this->_total_walked);
}

int Player::getHP(void) const
{
	return (this->_hp);
}

bool Player::isAlive(void) const
{
	if (this->_hp > 0)
		return (true);
	else
		return (false);
}

bool Player::isImmune(void) const
{
	if (this->_cur_immunity > 0.0f)
		return (true);
	else
		return (false);
}

void Player::playSound(std::string const &name)
{
	if (this->_audio == nullptr)
	{
		std::cout << "No Audio class provided !" << std::endl;
		return;
	}
	this->_audio->playSound(name);
}

void Player::playTheme(Audio::theme_list slot)
{
	if (this->_audio == nullptr)
	{
		std::cout << "No Audio class provided !" << std::endl;
		return;
	}
	this->_audio->playTheme(slot);
}

void Player::playSetTheme(void)
{
	if (this->_audio == nullptr)
	{
		std::cout << "No Audio class provided !" << std::endl;
		return;
	}
	this->_audio->playTheme(this->_theme);
}

void Player::stopSetTheme(void)
{
	if (this->_audio == nullptr)
	{
		std::cout << "No Audio class provided !" << std::endl;
		return;
	}
	this->_audio->stopTheme(this->_theme);
}

void Player::update_model(float time)
{
	this->_cb_model.update(time);
	this->_model.update(time);
}

/*
 * Interface ICollidable
 */

void Player::setPassthrough(bool value)
{
	static_cast<void>(value);
}

CollisionBox const &Player::getCollisionBox(void) const
{
	return (this->_cb);
}

ICollidable::Damages Player::getDamages(void) const
{
	return (ICollidable::Damages::NONE);
}

int Player::getScoreModifier(void) const
{
	return (0);
}

bool Player::getPassthrough(void) const
{
	return (false);
}

/*
 * Interface IInteractive
 */

bool Player::update_keyboard_interaction(Input const &input, float input_timer)
{
	bool        toogle       = false;
	bool        change_dir   = false;
	static bool sound_toogle = true;

	this->_acc.x  = 0.0f;
	this->_acc.z  = 0.0f;
	this->_acc.y  = 0.0f;
	this->_axis.x = 0;
	this->_axis.y = 0;
	if (this->_cam != nullptr)
	{
		if (input.p_key[GLFW_KEY_SPACE] == RELEASED)
		{
			this->_hoover = false;
		}
		if (input.p_key[GLFW_KEY_W] == PRESSED)
		{
			this->_acc += this->_force * this->_cam->getXYFront();
			this->_axis.x += 1;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_S] == PRESSED)
		{
			this->_acc -= this->_force * this->_cam->getXYFront();
			this->_axis.x -= 1;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_D] == PRESSED)
		{
			this->_acc += this->_force * this->_cam->getRight();
			this->_axis.y += 1;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_A] == PRESSED)
		{
			this->_acc -= this->_force * this->_cam->getRight();
			this->_axis.y -= 1;
			change_dir = true;
		}
		if (input.p_key[GLFW_KEY_M] == PRESSED && input_timer >= INPUT_REPEAT_TIMER)
		{
			if (this->_audio->getThemeStatus(this->_theme) == sf::Music::Stopped)
				this->playSetTheme();
			else if (sound_toogle)
			{
				this->_audio->setVolumeTheme(this->_theme, 0.0f);
				sound_toogle = false;
			}
			else
			{
				this->_audio->setVolumeTheme(this->_theme, THEME_VOLUME);
				this->_audio->setOffsetTheme(this->_theme, sf::Time());
				sound_toogle = true;
			}
			toogle = true;
		}
		if (input.p_key[GLFW_KEY_SPACE] == PRESSED && this->_cur_jump &&
			input_timer >= DOUBLE_JUMP_REPEAT_TIMER &&
			this->_last_jump == GLFW_RELEASE)
		{
			this->_acc += this->_force * 10.0f * this->_cam->getWorldUp();
			toogle = true;
			(this->_cur_jump)--;
			this->_on_surface = false;
			this->_delay_jump = true;
			this->_audio->playSound("jump");
		}
		this->_last_jump = input.p_key[GLFW_KEY_SPACE];
		if (input.p_key[GLFW_KEY_SPACE] == PRESSED && !this->_cur_jump &&
			this->_cur_hoover_time > 0.0f)
		{
			this->_hoover = true;
		}
		if (change_dir)
			this->_set_sprite_direction();
		if (toogle)
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

bool Player::update_gamepad_interaction(GamepadState const &state, float input_timer)
{
	bool        toogle       = false;
	bool        change_dir   = false;
	static bool sound_toogle = true;

	this->_acc.x  = 0.0f;
	this->_acc.z  = 0.0f;
	this->_acc.y  = 0.0f;
	this->_axis.x = 0;
	this->_axis.y = 0;
	if (this->_cam != nullptr)
	{
		if (state.buttons[GLFW_GAMEPAD_BUTTON_CROSS] == GLFW_RELEASE)
		{
			this->_hoover = false;
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] <= -MOV_DEAD_ZONE)
		{
			this->_acc += this->_force * this->_cam->getXYFront();
			this->_axis.x += 1;
			change_dir = true;
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] >= MOV_DEAD_ZONE)
		{
			this->_acc -= this->_force * this->_cam->getXYFront();
			this->_axis.x -= 1;
			change_dir = true;
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] >= MOV_DEAD_ZONE)
		{
			this->_acc += this->_force * this->_cam->getRight();
			this->_axis.y += 1;
			change_dir = true;
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] <= -MOV_DEAD_ZONE)
		{
			this->_acc -= this->_force * this->_cam->getRight();
			this->_axis.y -= 1;
			change_dir = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_TRIANGLE] == GLFW_PRESS &&
			input_timer >= INPUT_REPEAT_TIMER)
		{
			if (this->_audio->getThemeStatus(this->_theme) == sf::Music::Stopped)
				this->playSetTheme();
			else if (sound_toogle)
			{
				this->_audio->setVolumeTheme(this->_theme, 0.0f);
				sound_toogle = false;
			}
			else
			{
				this->_audio->setVolumeTheme(this->_theme, THEME_VOLUME);
				this->_audio->setOffsetTheme(this->_theme, sf::Time());
				sound_toogle = true;
			}
			toogle = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_CROSS] == GLFW_PRESS && this->_cur_jump &&
			input_timer >= DOUBLE_JUMP_REPEAT_TIMER &&
			this->_last_jump == GLFW_RELEASE)
		{
			this->_acc += this->_force * 10.0f * this->_cam->getWorldUp();
			toogle = true;
			(this->_cur_jump)--;
			this->_on_surface = false;
			this->_delay_jump = true;
			this->_audio->playSound("jump");
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_CROSS] == GLFW_PRESS &&
			!this->_cur_jump && this->_cur_hoover_time > 0.0f)
		{
			this->_hoover = true;
		}
		this->_last_jump = state.buttons[GLFW_GAMEPAD_BUTTON_CROSS];
		if (change_dir)
			this->_set_sprite_direction();
		if (toogle)
			return (true);
	}
	else
		std::cout << "Player : Can't update keyboard interaction" << std::endl;
	return (false);
}

/*
 * Interface IEntity
 */

void Player::update(float time)
{
	static char cd = BLINK_CD;

	this->_pos += this->_delta;
	this->_total_walked += glm::l2Norm(this->_delta);
	this->_cb_model.setPosition(this->_pos);
	this->_model.setPosition(this->_pos);
	this->_cb.setPos(this->_pos);
	this->_model.setSpriteY(this->_dir);
	this->_model.setSpriteX(static_cast<size_t>(this->_total_walked) %
							this->_model.getNbOfWalkFrame());
	this->_cur_immunity -= time;
	if (this->_cur_immunity > 0.0f && cd == BLINK_CD)
	{
		this->_display_sprite = true;
		cd--;
	}
	else if (this->_cur_immunity > 0.0f)
	{
		this->_display_sprite = false;
		cd--;
	}
	else
		this->_display_sprite = true;
	if (cd <= 0)
		cd = BLINK_CD;
}

void Player::draw(void)
{
	if (this->_draw_cb)
		this->_cb_model.draw();
	if (this->_display_sprite)
		this->_model.draw();
}

/*
 * Interface IGravity
 */

void Player::update_gravity(glm::vec3 const &vec_gravity, float delta)
{
	if (!this->_cb.IsBoxOnBox(this->_surface_cb) || !this->_on_surface)
	{
		this->_on_surface = false;
		if (!this->_delay_jump)
			this->_acc += (this->_hoover && this->_cur_hoover_time > 0.0f) ?
						  (vec_gravity * 0.5f) : vec_gravity;
		this->_delay_jump = false;
	}
	if (this->_hoover)
		this->_cur_hoover_time -= delta;
	this->_vel += this->_acc * delta;
	this->_vel *= std::pow(this->_friction, delta);
	this->_delta = this->_vel * delta;
}

/*
 * Private
 */

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