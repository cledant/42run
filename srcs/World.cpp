/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "World.hpp"

World::World(Input const &input, GLFW_Window const &win, Gamepad &gamepad,
			 glm::vec3 cam_pos, float max_fps, size_t max_frame_skip) :
		_active(nullptr), _input(input), _window(win), _gamepad(gamepad),
		_camera(input, gamepad, cam_pos, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f),
		_fov(45.0f), _max_fps(max_fps),
		_max_frame_skip(max_frame_skip), _next_update_tick(0.0f),
		_last_update_tick(0.0f), _delta_tick(0.0f), _skip_loop(0),
		_input_timer(0.0f), _input_mouse_timer(0.0f),
		_gravity(glm::vec3(0.0f, -50.0f, 0.0f)), _str_hp("0"), _str_score("0"),
		_score_modifier(0), _first_run_theme(true)
{
	if (max_frame_skip == 0)
		throw World::WorldFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
					static_cast<GLfloat>(win.cur_win_h);
	this->_tick            = 1.0f / this->_max_fps;
	this->_perspective     = glm::perspective(glm::radians(this->_fov), ratio, 0.1f,
											  400.0f);
	this->_enabled_gamepad = this->_gamepad.isGamepadConnected(GLFW_JOYSTICK_1);
	this->_gamepad.printJoystickInfo(GLFW_JOYSTICK_1);
}

World::~World(void)
{
	static_cast<void>(this->_input);
	std::vector<IEntity *>::iterator it;

	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		delete *it;
	delete this->_active;
}

void World::update(void)
{
	std::vector<IEntity *>::iterator it;

	if (this->_first_run_theme && this->_active != nullptr)
	{
		reinterpret_cast<Player *>(this->_active)->playSetTheme();
		this->_first_run_theme = false;
	}
	if (this->_window.resized)
		this->updatePerspective(this->_fov);
	if (this->_active == nullptr)
	{
		this->_camera.update_third_person(this->_input.mouse_exclusive,
										  glm::vec3({0.0f, 0.0f, 0.0f}),
										  this->_enabled_gamepad);
		this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
	}
	else
	{
		if (!this->_enabled_gamepad)
		{
			if (this->_active->update_mouse_interaction(this->_input, this->_window,
														this->_camera.getPos(), std::vector<glm::vec3 const *>{
							&(this->_camera.getFront()), &(this->_camera.getUp()),
							&(this->_camera.getRight())}, this->_input_mouse_timer))
				this->_input_mouse_timer = 0.0f;
			else if (this->_input_mouse_timer < 1.0f)
				this->_input_mouse_timer += this->_tick;
			if (this->_active->update_keyboard_interaction(this->_input,
														   this->_input_timer))
				this->_input_timer       = 0.0f;
			else if (this->_input_timer < INPUT_REPEAT_TIMER)
				this->_input_timer += this->_tick;
		}
		else
		{
			if (this->_gamepad.isGamepadConnected(GLFW_JOYSTICK_1))
			{
				this->_gamepad.pollGamepads();
				if (this->_active->update_gamepad_interaction(
						this->_gamepad.getGamepadState(GLFW_JOYSTICK_1), this->_input_timer))
					this->_input_timer = 0.0f;
				else if (this->_input_timer < INPUT_REPEAT_TIMER)
					this->_input_timer += this->_tick;
			}
			else
				std::cout << "Gamepad not connected anymore" << std::endl;
		}
		reinterpret_cast<Player *>(this->_active)->update_gravity(this->_gravity, this->_delta_tick);
		this->_check_collisions();
		reinterpret_cast<Player *>(this->_active)->update(this->_delta_tick);
		this->_camera.update_third_person(this->_input.mouse_exclusive,
										  reinterpret_cast<Player *>(this->_active)->getPos(),
										  this->_enabled_gamepad);
		this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
		reinterpret_cast<Player *>(this->_active)->setSpriteYaw(this->_camera.getYaw());
		reinterpret_cast<Player *>(this->_active)->update_model(0.0f);
	}
	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		(*it)->update(this->_delta_tick);
}

void World::render(void)
{
	std::vector<IEntity *>::iterator it;

	oGL_module::oGL_clear_buffer(0.2f, 0.3f, 0.3f);
	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		(*it)->draw();
	if (this->_active != nullptr)
		reinterpret_cast<Player *>(this->_active)->draw();
}

IEntity *World::add_Simple_box(Shader const *shader, glm::vec3 const &pos,
							   glm::vec3 const &scale)
{
	IEntity *ptr;

	ptr = new Simple_box(shader, &(this->_perspec_mult_view), pos, scale);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

IEntity *World::add_Cubemap(Shader const *shader,
							std::vector<std::string> const &files,
							glm::vec3 const &pos, glm::vec3 const &scale)
{
	IEntity *ptr;

	ptr = new Cubemap(shader, &(this->_perspec_mult_view), files, pos, scale);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

IEntity *World::add_Prop(Prop::Params &params)
{
	IEntity *ptr;

	params.perspec_mult_view = &(this->_perspec_mult_view);
	ptr = new Prop(params);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

IInteractive *World::add_Player(Player::Params &params)
{
	IInteractive *ptr;

	params.perspec_mult_view = &(this->_perspec_mult_view);
	params.cam               = &(this->_camera);
	ptr = new Player(params);
	this->_active = ptr;
	return (ptr);
}

IEntity *World::add_CollidableBox(Shader const *shader, glm::vec3 const &pos,
								  glm::vec3 const &size, Texture const *tex,
								  ICollidable::Damages dmg, bool passthrough,
								  int score_modifier)
{
	IEntity *ptr;

	ptr = new CollidableBox(shader, &(this->_perspec_mult_view), pos, size, tex, dmg,
							passthrough, score_modifier);
	this->_entity_list.push_back(ptr);
	this->_collision_check_list.push_back(dynamic_cast<ICollidable *>(ptr));
	return (ptr);
}


IEntity *World::add_CollidableProp(Prop::Params &params, glm::vec3 const &half_size,
								   ICollidable::Damages dmg, bool passthrough,
								   int score_modifier)
{
	IEntity *ptr;

	params.perspec_mult_view = &(this->_perspec_mult_view);
	ptr = new CollidableProp(params, CollisionBox(params.pos, half_size),
							 dmg, passthrough, score_modifier);
	this->_entity_list.push_back(ptr);
	this->_collision_check_list.push_back(dynamic_cast<ICollidable *>(ptr));
	return (ptr);
}

void World::setActiveInteractive(IInteractive *ptr)
{
	this->_active = ptr;
}


void World::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
					static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, 0.1f, 400.0f);
}

void World::reset_update_timer(float time)
{
	this->_next_update_tick = time;
	this->_last_update_tick = time;
}

void World::reset_skip_loop(void)
{
	this->_skip_loop = 0;
}

bool World::should_be_updated(float time)
{
	if (time > this->_next_update_tick &&
		this->_skip_loop < this->_max_frame_skip)
	{
		(this->_skip_loop)++;
		this->_next_update_tick += this->_tick;
		this->_delta_tick       = time - this->_last_update_tick;
		this->_last_update_tick = time;
		return (true);
	}
	return (false);
}

std::string const &World::getScore(void)
{

	if (this->_active == nullptr)
		this->_str_score = "0";
	this->_str_score     = std::to_string(
			static_cast<long int>(std::trunc(reinterpret_cast<Player *>
											 (this->_active)->getTotalWalked()) / 10.0f)
			+ this->_score_modifier);
	return (this->_str_score);
}

std::string const &World::getStrPlayerHP(void)
{
	this->_str_hp = (this->_active == nullptr) ? "0" :
					std::to_string(reinterpret_cast<Player *>(this->_active)->getHP());
	return (this->_str_hp);
}

void World::_check_collisions(void)
{
	CollisionBox::SweepResolution res;
	glm::vec3                     inv_delta;
	CollisionBox::SweepResolution nearest;
	ICollidable                   *ptr        = nullptr;
	ICollidable::Damages          dmg         = ICollidable::Damages::NONE;
	int                           score_mod   = 0;
	bool                          passthrough = false;

	inv_delta.x = -reinterpret_cast<Player *>(this->_active)->getDelta().x;
	inv_delta.y = -reinterpret_cast<Player *>(this->_active)->getDelta().y;
	inv_delta.z = -reinterpret_cast<Player *>(this->_active)->getDelta().z;
	for (auto it = this->_collision_check_list.begin(); it != this->_collision_check_list.end(); ++it)
	{
		if ((reinterpret_cast<Player *>(this->_active)->getCollisionBox().
				IsBoxInBoxSweep((*it)->getCollisionBox(), inv_delta, &res)))
		{
			if (ptr == nullptr)
			{
				ptr = *it;
				std::memcpy(&nearest, &res, sizeof(CollisionBox::SweepResolution));
				dmg         = (*it)->getDamages();
				score_mod   = (*it)->getScoreModifier();
				passthrough = (*it)->getPassthrough();
			}
			else if (res.time < nearest.time)
			{
				ptr = *it;
				std::memcpy(&nearest, &res, sizeof(CollisionBox::SweepResolution));
				dmg         = (*it)->getDamages();
				score_mod   = (*it)->getScoreModifier();
				passthrough = (*it)->getPassthrough();
			}
		}
	}
	if (ptr != nullptr)
		this->_resolve_sweep_collision(reinterpret_cast<Player *>(this->_active),
									   (*ptr).getCollisionBox(), nearest, dmg, score_mod,
									   passthrough);
}

void World::_resolve_sweep_collision(Player *player, CollisionBox const &box,
									 CollisionBox::SweepResolution const &res,
									 ICollidable::Damages dmg_taken, int score_mod,
									 bool passthrough)
{
	glm::vec3 new_delta;

	new_delta.x = !isnan(-res.res.delta.x) ? -res.res.delta.x : 0.0f;
	new_delta.y = !isnan(-res.res.delta.y) ? -res.res.delta.y : 0.0f;
	new_delta.z = !isnan(-res.res.delta.z) ? -res.res.delta.z : 0.0f;
	if (!passthrough)
	{
		if (res.res.normal.y != 0.0f)
			new_delta.y += (res.res.normal.y < 0.0f) ? (player->getCollisionBox().getHalfSize().y * 0.01) :
						   -(player->getCollisionBox().getHalfSize().y * 0.01);
		else if (res.res.normal.x != 0.0f)
			new_delta.x += (res.res.normal.x < 0.0f) ? (player->getCollisionBox().getHalfSize().x * 0.01) :
						   -(player->getCollisionBox().getHalfSize().x * 0.01);
		else if (res.res.normal.z != 0.0f)
			new_delta.z += (res.res.normal.z < 0.0f) ? (player->getCollisionBox().getHalfSize().z * 0.01) :
						   -(player->getCollisionBox().getHalfSize().z * 0.01);
		player->setDelta(new_delta);
		if (res.res.normal.y < 0.0f)
		{
			player->setSurfaceCollisionBox(box);
			player->setOnSurface(true);
			player->setCurJumpToMax();
			player->setCurHooverTimeToMax();
		}
		player->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	if (!reinterpret_cast<Player *>(this->_active)->isImmune() &&
		dmg_taken != ICollidable::Damages::NONE)
	{
		reinterpret_cast<Player *>(this->_active)->lowerHP(dmg_taken);
		reinterpret_cast<Player *>(this->_active)->setImmunityTimerToMax();
		reinterpret_cast<Player *>(this->_active)->playSound("damage");
	}
/*	if (score_mod > 0)
		reinterpret_cast<Player *>(this->_active)->playSound("bonus");
	else if (score_mod < 0)
		reinterpret_cast<Player *>(this->_active)->playSound("malus");*/
	this->_score_modifier += score_mod;
}

World::WorldFailException::WorldFailException(void)
{
	this->_msg = "World : Something failed";
}

World::WorldFailException::~WorldFailException(void) throw()
{
}
