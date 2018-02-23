/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RunnerWorld.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Runner/RunnerWorld.hpp"

RunnerWorld::RunnerWorld(Input const &input, GLFW_Window const &win, Gamepad &gamepad,
						 glm::vec3 cam_pos, float max_fps, size_t max_frame_skip,
						 long int high_score) :
		_active(nullptr), _active_shadow(nullptr), _input(input),
		_window(win), _gamepad(gamepad),
		_camera(input, gamepad, cam_pos, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f),
		_fov(45.0f), _max_fps(max_fps),
		_max_frame_skip(max_frame_skip), _next_update_tick(0.0f),
		_last_update_tick(0.0f), _delta_tick(0.0f), _skip_loop(0),
		_input_timer(0.0f), _input_mouse_timer(0.0f),
		_gravity(glm::vec3(0.0f, -50.0f, 0.0f)), _str_hp("0"), _str_score("0"),
		_str_speed("0.0"), _score_modifier(0), _should_end(false),
		_current_score(0), _last_game_score(0), _high_score(high_score),
		_laps(0)
{
	if (max_frame_skip == 0)
		throw RunnerWorld::RunnerWorldFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
					static_cast<GLfloat>(win.cur_win_h);
	this->_tick            = 1.0f / this->_max_fps;
	this->_perspective     = glm::perspective(glm::radians(this->_fov), ratio,
											  RUNNER_FOV_MIN, RUNNER_FOV_MAX);
	this->_enabled_gamepad = this->_gamepad.isGamepadConnected(GLFW_JOYSTICK_1);
	this->_gamepad.printJoystickInfo(GLFW_JOYSTICK_1);
	this->_room_list.reserve(RunnerWorld::list_size);
	this->_camera.setLockCam(true);
	this->_camera.setPitch(-20.0f);
	this->_camera.setDistToTarget(5.0f);
	this->_check_stuck = std::vector<float>(MAX_STUCK_FRAME, 100000.0f);
}

RunnerWorld::~RunnerWorld(void)
{
	for (auto it = this->_room_list.begin(); it != this->_room_list.end(); ++it)
		delete *it;
	delete this->_active;
	delete this->_active_shadow;
}

void RunnerWorld::update(void)
{
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
			if (dynamic_cast<Player *>(this->_active)->isAlive())
			{
				dynamic_cast<Player *>(this->_active)->addAcceleration(
						40.0f * glm::log(glm::vec3((this->_current_score * this->_current_score) / (1000 * 1000)) +
										 glm::vec3(10.0f) * glm::vec3(this->_current_score / 1000) +
										 glm::vec3(100.0f)) * this->_camera.getXYFront());
				dynamic_cast<Player *>(this->_active)->forceBackSprite();
			}
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
				if (dynamic_cast<Player *>(this->_active)->isAlive())
				{
					dynamic_cast<Player *>(this->_active)->addAcceleration(
							40.0f * glm::log(glm::vec3((this->_current_score * this->_current_score) / (1000 * 1000)) +
											 glm::vec3(10.0f) * glm::vec3(this->_current_score / 1000) +
											 glm::vec3(100.0f)) * this->_camera.getXYFront());
					dynamic_cast<Player *>(this->_active)->forceBackSprite();
				}
			}
			else
				std::cout << "Gamepad not connected anymore" << std::endl;
		}
		reinterpret_cast<Player *>(this->_active)->update_gravity(this->_gravity, this->_tick);
		this->_check_collisions();
		reinterpret_cast<Player *>(this->_active)->update(this->_tick);
		this->_camera.update_third_person(this->_input.mouse_exclusive,
										  reinterpret_cast<Player *>(this->_active)->getPos(),
										  this->_enabled_gamepad);
		this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
		reinterpret_cast<Player *>(this->_active)->setSpriteYaw(this->_camera.getYaw());
		reinterpret_cast<Player *>(this->_active)->update_model(0.0f);
		this->_current_score = static_cast<long int>(std::trunc(
				reinterpret_cast<Player *>(this->_active)->getTotalWalked()) / 10.0f) +
							   this->_score_modifier;
		this->_add_pos_for_check(reinterpret_cast<Player *>(this->_active)->getPos());
		if (std::adjacent_find(this->_check_stuck.begin(), this->_check_stuck.end(), std::not_equal_to<float>()) ==
			this->_check_stuck.end())
		{
			reinterpret_cast<Player *>(this->_active)->lowerHP(ICollidable::Damages::INSTANT_DEATH);
			reinterpret_cast<Player *>(this->_active)->playSound("damage");
			return;
		}
	}
	for (auto it = this->_room_list.begin(); it != this->_room_list.end(); ++it)
		(*it)->update(this->_tick);
	auto      it = this->_list_collidable_box.find("tp_trigger");
	if (it != this->_list_collidable_box.end())
		it->second.update(this->_tick);
}

void RunnerWorld::render(void)
{
	oGL_module::oGL_clear_buffer(0.2f, 0.2f, 0.2f);
	for (auto it = this->_room_list.begin(); it != this->_room_list.end(); ++it)
		(*it)->draw();
	if (this->_active != nullptr)
		reinterpret_cast<Player *>(this->_active)->draw();
}

Room *RunnerWorld::addRoomTemplate(std::string const &name, Room::Params &params)
{
	params.floor.perspec_mult_view      = &(this->_perspec_mult_view);
	params.roof.perspec_mult_view       = &(this->_perspec_mult_view);
	params.right_wall.perspec_mult_view = &(this->_perspec_mult_view);
	params.left_wall.perspec_mult_view  = &(this->_perspec_mult_view);
	params.front_wall.perspec_mult_view = &(this->_perspec_mult_view);
	params.perspec_mult_view            = &(this->_perspec_mult_view);
	this->_room_template_list.insert(std::pair<std::string, Room>(name, Room(params)));
	return (&(this->_room_template_list[name]));
}

IEntity *RunnerWorld::add_CollidableBox(CollidableBox::Params &params,
										std::string const &name)
{
	params.perspec_mult_view = &(this->_perspec_mult_view);
	this->_list_collidable_box.insert(std::pair<std::string, CollidableBox>(
			name, CollidableBox(params)));
	return (&(this->_list_collidable_box[name]));
}

void RunnerWorld::addCollidableToRoomTemplate(std::string const &room_name,
											  std::string const &slot_name,
											  CollidableProp::Params &params)
{
	auto it = this->_room_template_list.find(room_name);

	params.prop_params.perspec_mult_view = &(this->_perspec_mult_view);
	if (it == this->_room_template_list.end())
		throw RunnerWorld::RoomNotFoundException();
	(*it).second.addCollidableProp(slot_name, params);
}

void RunnerWorld::initRoomList(void)
{
	static bool isInit = false;
	auto        it     = this->_room_template_list.find("NormalRoomEmpty");

	if (!isInit)
	{
		if (it == this->_room_template_list.end())
			throw RoomNotFoundException();
		for (size_t i = 0; i < RunnerWorld::list_size; ++i)
			this->_room_list.push_back(new Room(it->second));
		isInit = true;
	}
}

void RunnerWorld::generateInitialRoomList(void)
{
	std::vector<std::map<std::string, Room>::iterator> vec_it     = {this->_room_template_list
																		 .find("NormalRoomEmpty"),
																	 this->_room_template_list
																		 .find("NormalRoomObstacleOnly"),
																	 this->_room_template_list
																		 .find("NormalRoomBonusOnly"),
																	 this->_room_template_list
																		 .find("NormalRoomBonusAndObstacle"),
																	 this->_room_template_list
																		 .find("FallRightRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallLeftRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallRightRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallFrontRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallMiddleRoomObstacle")
	};
	size_t                                             index_room = 0;
	size_t                                             index_prop = 0;
	std::mt19937_64                                    generator(this->_rd());
	std::uniform_int_distribution<size_t>              distri_room(1, vec_it.size() - 1);
	std::uniform_int_distribution<size_t>              distri_prop;
	std::uniform_int_distribution<size_t>              distri_max;
	size_t                                             max_obs;

	for (size_t i = 0; i < vec_it.size(); ++i)
	{
		if (vec_it[i] == this->_room_template_list.end())
			throw RoomNotFoundException();
	}
	for (size_t i = 0; i < 5; ++i)
	{
		index_room = distri_room(generator);
		*(this->_room_list[i])      = vec_it[index_room]->second;
		*(this->_room_list[i + 15]) = vec_it[index_room]->second;
		distri_prop = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size() - 1);
		distri_max  = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size());
		this->_room_list[i]->translateObject(glm::vec3(13.2f * i, 0.0f, 0.0f));
		this->_room_list[i + 15]->translateObject(glm::vec3(13.2f * (i + 15), 0.0f, 0.0f));
		if (index_room % vec_it.size())
		{
			max_obs = distri_max(generator);
			for (size_t j = 0; j < max_obs; ++j)
			{
				index_prop = distri_prop(generator);
				this->_room_list[i]->getCollidableProp("Slot" + std::to_string(index_prop)).setActive(true);
				this->_room_list[i + 15]->getCollidableProp("Slot" + std::to_string(index_prop)).setActive(true);
			}
		}
	}
	*(this->_room_list[5]) = vec_it[0]->second;
	this->_room_list[5]->translateObject(glm::vec3(13.2f * 5, 0.0f, 0.0f));
	for (size_t i = 6; i < RunnerWorld::list_size - 5; ++i)
	{
		index_room = distri_room(generator);
		*(this->_room_list[i]) = vec_it[index_room]->second;
		this->_room_list[i]->translateObject(glm::vec3(13.2f * i, 0.0f, 0.0f));
		distri_prop = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size() - 1);
		distri_max  = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size());
		if (index_room % vec_it.size())
		{
			max_obs = distri_max(generator);
			for (size_t j = 0; j < max_obs; ++j)
			{
				index_prop = distri_prop(generator);
				this->_room_list[i]->getCollidableProp("Slot" + std::to_string(index_prop)).setActive(true);
			}
		}
	}
}

void RunnerWorld::generateMiddleRoomList(void)
{
	std::vector<std::map<std::string, Room>::iterator> vec_it     = {this->_room_template_list
																		 .find("NormalRoomEmpty"),
																	 this->_room_template_list
																		 .find("NormalRoomObstacleOnly"),
																	 this->_room_template_list
																		 .find("NormalRoomBonusOnly"),
																	 this->_room_template_list
																		 .find("NormalRoomBonusAndObstacle"),
																	 this->_room_template_list
																		 .find("FallRightRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallLeftRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallRightRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallFrontRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallMiddleRoomObstacle")
	};
	size_t                                             index_room = 0;
	size_t                                             index_prop = 0;
	std::mt19937_64                                    generator(this->_rd());
	std::uniform_int_distribution<size_t>              distri_room(1, vec_it.size() - 1);
	std::uniform_int_distribution<size_t>              distri_prop;
	std::uniform_int_distribution<size_t>              distri_max;
	size_t                                             max_obs;

	for (size_t i = 5; i < RunnerWorld::list_size - 5; ++i)
	{
		index_room = distri_room(generator);
		*(this->_room_list[i]) = vec_it[index_room]->second;
		this->_room_list[i]->translateObject(glm::vec3(13.2f * i, 0.0f, 0.0f));
		distri_prop = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size() - 1);
		distri_max  = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size());
		if (index_room % vec_it.size())
		{
			if (this->_should_spawn_full_obstacle())
			{
				for (size_t j = 0; j < vec_it[index_room]->second.getCollidablePropList().size(); ++j)
					this->_room_list[i]->getCollidableProp("Slot" + std::to_string(j)).setActive(true);
			}
			else
			{
				max_obs = distri_max(generator);
				for (size_t j = 0; j < max_obs; ++j)
				{
					index_prop = distri_prop(generator);
					this->_room_list[i]->getCollidableProp("Slot" + std::to_string(index_prop)).setActive(true);
				}
			}
		}
	}
}

void RunnerWorld::generateBeginEndRoomList(void)
{
	std::vector<std::map<std::string, Room>::iterator> vec_it     = {this->_room_template_list
																		 .find("NormalRoomEmpty"),
																	 this->_room_template_list
																		 .find("NormalRoomObstacleOnly"),
																	 this->_room_template_list
																		 .find("NormalRoomBonusOnly"),
																	 this->_room_template_list
																		 .find("NormalRoomBonusAndObstacle"),
																	 this->_room_template_list
																		 .find("FallRightRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallLeftRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallRightRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallFrontRoomObstacle"),
																	 this->_room_template_list
																		 .find("FallMiddleRoomObstacle")
	};
	size_t                                             index_room = 0;
	size_t                                             index_prop = 0;
	std::mt19937_64                                    generator(this->_rd());
	std::uniform_int_distribution<size_t>              distri_room(1, vec_it.size() - 1);
	std::uniform_int_distribution<size_t>              distri_prop;
	std::uniform_int_distribution<size_t>              distri_max;
	size_t                                             max_obs;

	for (size_t i = 0; i < 5; ++i)
	{
		index_room = distri_room(generator);
		*(this->_room_list[i])      = vec_it[index_room]->second;
		*(this->_room_list[i + 15]) = vec_it[index_room]->second;
		this->_room_list[i]->translateObject(glm::vec3(13.2f * i, 0.0f, 0.0f));
		this->_room_list[i + 15]->translateObject(glm::vec3(13.2f * (i + 15), 0.0f, 0.0f));
		distri_prop = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size() - 1);
		distri_max  = std::uniform_int_distribution<size_t>(0, vec_it[index_room]->second.getCollidablePropList()
																				 .size());
		if (index_room % vec_it.size())
		{
			if (this->_should_spawn_full_obstacle())
			{
				for (size_t j = 0; j < vec_it[index_room]->second.getCollidablePropList().size(); ++j)
				{
					this->_room_list[i]->getCollidableProp("Slot" + std::to_string(j)).setActive(true);
					this->_room_list[i + 15]->getCollidableProp("Slot" + std::to_string(j)).setActive(true);
				}
			}
			else
			{
				max_obs = distri_max(generator);
				for (size_t j = 0; j < max_obs; ++j)
				{
					index_prop = distri_prop(generator);
					this->_room_list[i]->getCollidableProp("Slot" + std::to_string(index_prop)).setActive(true);
					this->_room_list[i + 15]->getCollidableProp("Slot" + std::to_string(index_prop)).setActive(true);
				}
			}
		}
	}
}

void RunnerWorld::generateDebug(size_t room_type, bool has_prop)
{
	std::vector<std::map<std::string, Room>::iterator> vec_it  = {this->_room_template_list
																	  .find("NormalRoomEmpty"),
																  this->_room_template_list
																	  .find("NormalRoomObstacleOnly"),
																  this->_room_template_list
																	  .find("NormalRoomBonusOnly"),
																  this->_room_template_list
																	  .find("NormalRoomBonusAndObstacle"),
																  this->_room_template_list
																	  .find("FallRightRoomEmpty"),
																  this->_room_template_list
																	  .find("FallLeftRoomEmpty"),
																  this->_room_template_list
																	  .find("FallFrontRoomEmpty"),
																  this->_room_template_list
																	  .find("FallRightRoomObstacle"),
																  this->_room_template_list
																	  .find("FallLeftRoomObstacle"),
																  this->_room_template_list
																	  .find("FallRightRoomObstacle"),
																  this->_room_template_list
																	  .find("FallFrontRoomObstacle"),
																  this->_room_template_list
																	  .find("FallMiddleRoomEmpty"),
																  this->_room_template_list
																	  .find("FallMiddleRoomObstacle")
	};
	size_t                                             max_obs = vec_it[room_type]->second.getCollidablePropList()
																				  .size();
	for (size_t                                        i       = 0; i < 5; ++i)
	{
		*(this->_room_list[i]) = vec_it[room_type]->second;
		this->_room_list[i]->translateObject(glm::vec3(13.2f * i, 0.0f, 0.0f));
		if (has_prop)
		{
			for (size_t j = 0; j < max_obs; ++j)
			{
				this->_room_list[i]->getCollidableProp("Slot" + std::to_string(j)).setActive(true);
			}
		}
	}
	*(this->_room_list[5])                                     = vec_it[0]->second;
	this->_room_list[5]->translateObject(glm::vec3(13.2f * 5, 0.0f, 0.0f));
	for (size_t i = 6; i < RunnerWorld::list_size; ++i)
	{
		*(this->_room_list[i]) = vec_it[room_type]->second;
		this->_room_list[i]->translateObject(glm::vec3(13.2f * i, 0.0f, 0.0f));
		if (has_prop)
		{
			for (size_t j = 0; j < max_obs; ++j)
			{
				this->_room_list[i]->getCollidableProp("Slot" + std::to_string(j)).setActive(true);
			}
		}
	}
}

IInteractive *RunnerWorld::add_Player(Player::Params &params)
{
	IInteractive *ptr;

	params.perspec_mult_view = &(this->_perspec_mult_view);
	params.cam               = &(this->_camera);
	ptr = new Player(params);
	this->_active = ptr;
	dynamic_cast<Player *>(this->_active)->setDisableFront(true);
	dynamic_cast<Player *>(this->_active)->setDisableBack(true);
	return (ptr);
}

IEntity *RunnerWorld::add_PlayerShadow(OrientableShaderSurface::Params &params)
{
	IEntity *ptr;

	params.perspec_mult_view = &(this->_perspec_mult_view);
	ptr = new OrientableShaderSurface(params);
	this->_active_shadow = ptr;
	return (ptr);
}

void RunnerWorld::deletePlayer(void)
{
	delete this->_active;
	this->_active = nullptr;
}

void RunnerWorld::deletePlayerShadow(void)
{
	delete this->_active_shadow;
	this->_active_shadow = nullptr;
}

void RunnerWorld::updatePerspective(float fov)
{
	GLfloat ratio = static_cast<GLfloat>(this->_window.cur_win_w) /
					static_cast<GLfloat>(this->_window.cur_win_h);
	this->_perspective = glm::perspective(glm::radians(fov), ratio, RUNNER_FOV_MIN,
										  RUNNER_FOV_MAX);
}

void RunnerWorld::reset_update_timer(float time)
{
	this->_next_update_tick = time;
	this->_last_update_tick = time;
}

void RunnerWorld::reset_skip_loop(void)
{
	this->_skip_loop = 0;
}

bool RunnerWorld::should_be_updated(float time)
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

void RunnerWorld::updateLastGameScore(void)
{
	this->_last_game_score = this->_current_score;
	this->_current_score   = 0;
	this->_score_modifier  = 0;
}

void RunnerWorld::updateHighScore(void)
{
	if (this->_high_score < this->_current_score)
		this->_high_score = this->_current_score;
	this->_score_modifier = 0;
}

void RunnerWorld::playPlayerTheme(void)
{
	reinterpret_cast<Player *>(this->_active)->playSetTheme();
}

void RunnerWorld::stopPlayerTheme(void)
{
	reinterpret_cast<Player *>(this->_active)->stopSetTheme();
}

/*
 * Getter
 */

std::string const &RunnerWorld::getStrCurrentScore(void)
{
	this->_str_score = std::to_string(this->_current_score);
	return (this->_str_score);
}

std::string const &RunnerWorld::getStrLastScore(void)
{
	this->_str_last_score = std::to_string(this->_last_game_score);
	return (this->_str_last_score);
}

std::string const &RunnerWorld::getStrHighScore(void)
{
	this->_str_high_score = std::to_string(this->_high_score);
	return (this->_str_high_score);
}

std::string const &RunnerWorld::getStrSpeed(void)
{
	std::stringstream sstream;

	sstream << std::fixed << std::setprecision(1) << reinterpret_cast<Player *>(this->_active)->getVelocity().x;
	this->_str_speed = sstream.str();
	return (this->_str_speed);
}

long int RunnerWorld::getCurrentScore(void) const
{
	return (this->_current_score);
}

long int RunnerWorld::getLastGameScore(void) const
{
	return (this->_last_game_score);
}

long int RunnerWorld::getHighScore(void) const
{
	return (this->_high_score);
}

std::string const &RunnerWorld::getStrPlayerHP(void)
{
	this->_str_hp = (this->_active == nullptr) ? "0" :
					std::to_string(reinterpret_cast<Player *>(this->_active)->getHP());
	return (this->_str_hp);
}

bool RunnerWorld::getShouldEnd(void) const
{
	return (this->_should_end);
}

float RunnerWorld::getTickRate(void) const
{
	return (this->_tick);
}

bool RunnerWorld::isPlayerAlive() const
{
	if (dynamic_cast<Player *>(this->_active)->getHP() > 0)
		return (true);
	return (false);
}

bool RunnerWorld::isGamepadEnabled() const
{
	return (this->_enabled_gamepad);
}

/*
 * Setter
 */

void RunnerWorld::setActiveInteractive(IInteractive *ptr)
{
	this->_active = ptr;
}

void RunnerWorld::resetInputTimer(void)
{
	this->_input_timer = 0.0f;
}

void RunnerWorld::setLaps(size_t laps)
{
	this->_laps = laps;
}

void RunnerWorld::forceKeyboard(void)
{
	this->_enabled_gamepad = false;
	std::cout << "Keyboard forced to be used" << std::endl;
}

/*
 * Private
 */

inline void RunnerWorld::_check_collisions(void)
{
	CollisionBox::SweepResolution res;
	glm::vec3                     inv_delta;
	CollisionBox::SweepResolution nearest;
	ICollidable const             *ptr                     = nullptr;
	Room                          *cur_room                = nullptr;
	Room                          *next_room               = nullptr;
	static bool                   should_trigger_regen_end = false;

	inv_delta.x = -reinterpret_cast<Player *>(this->_active)->getDelta().x;
	inv_delta.y = -reinterpret_cast<Player *>(this->_active)->getDelta().y;
	inv_delta.z = -reinterpret_cast<Player *>(this->_active)->getDelta().z;

/*
 * Check teleport and room generation trigger;
 */
	auto it  = this->_list_collidable_box.find("tp_trigger");
	if (it != this->_list_collidable_box.end())
	{
		if (it->second.getCollisionBox().IsBoxInBoxSweep(reinterpret_cast<Player *>(this->_active)->getCollisionBox(),
														 inv_delta, &res))
		{
			glm::vec3 player_pos = reinterpret_cast<Player *>(this->_active)->getPos();
			player_pos.x -= 13.2f * 15;
			should_trigger_regen_end = true;
			for (int i = 0; i < 5; ++i)
			{
				*(this->_room_list[i]) = *(this->_room_list[i + 15]);
				this->_room_list[i]->translateObject(glm::vec3(-13.2f * 15, 0.0f, 0.0f));
			}
			this->_laps++;
			this->generateMiddleRoomList();
//			this->generateDebug(DEBUG_FORCE_ROOM, DEBUG_FORCE_PROP);
			reinterpret_cast<Player *>(this->_active)->setPos(player_pos);
		}
	}
	auto it2 = this->_list_collidable_box.find("regen_end_trigger");
	if (it2 != this->_list_collidable_box.end() && should_trigger_regen_end)
	{
		if (it2->second.getCollisionBox().IsBoxInBoxSweep(reinterpret_cast<Player *>(this->_active)->getCollisionBox(),
														  inv_delta, &res))
		{
			this->generateBeginEndRoomList();
//			this->generateDebug(DEBUG_FORCE_ROOM, DEBUG_FORCE_PROP);
			should_trigger_regen_end = false;
		}
	}
/*
 * Check if Player is in a room
 */
	for (auto it = this->_room_list.begin(); it != this->_room_list.end(); ++it)
	{
		if ((reinterpret_cast<Player *>(this->_active)->getCollisionBox().
				IsBoxInBox((*it)->getCollisionBox(), nullptr)))
		{
			cur_room  = *it;
			next_room = ((it++) == this->_room_list.end()) ? nullptr : *(it++);
		}
	}
	if (cur_room == nullptr)
	{
		dynamic_cast<Player *>(this->_active)->lowerHP(Player::INSTANT_DEATH);
		return;
	}
/*
 * Check actual collisions
 */
	for (auto it = this->_room_list.begin(); it != this->_room_list.end(); ++it)
	{
		if ((*it)->getActive())
		{
			this->_check_collidable_box((*it)->getRoof(), &nearest, inv_delta, &ptr);
			this->_check_collidable_box((*it)->getRightWall(), &nearest, inv_delta, &ptr);
			this->_check_collidable_box((*it)->getLeftWall(), &nearest, inv_delta, &ptr);
			this->_check_collidable_box((*it)->getFloor(), &nearest, inv_delta, &ptr);
			if ((*it)->getFrontWall().getActive())
				this->_check_collidable_box((*it)->getFrontWall(), &nearest, inv_delta, &ptr);
			if (ptr == &((*it)->getRightWall()))
				reinterpret_cast<Player *>(this->_active)->setDisableRight(true);
			else if (ptr == &((*it)->getLeftWall()))
				reinterpret_cast<Player *>(this->_active)->setDisableLeft(true);

		}
	}
	for (auto it = cur_room->getCollidablePropList().begin(); it != cur_room->getCollidablePropList()
																			.end(); ++it)
	{
		if ((*it).second.getActive() && (reinterpret_cast<Player *>(this->_active)->getCollisionBox().
				IsBoxInBoxSweep((*it).second.getCollisionBox(), inv_delta, &res)))
		{
			if ((*it).second.getPassthrough())
			{
				if (!reinterpret_cast<Player *>(this->_active)->isImmune() &&
					(*it).second.getDamages() != ICollidable::Damages::NONE)
				{
					reinterpret_cast<Player *>(this->_active)->lowerHP((*it).second.getDamages());
					reinterpret_cast<Player *>(this->_active)->setImmunityTimerToMax();
					reinterpret_cast<Player *>(this->_active)->playSound("damage");
				}
				reinterpret_cast<Player *>(this->_active)->playSound((*it).second.getPickUpSound());
				this->_score_modifier += (*it).second.getScoreModifier();
				const_cast<CollidableProp &>((*it).second).setActive(false);
			}
			else if (ptr == nullptr)
			{
				ptr = (&(*it).second);
				std::memcpy(&nearest, &res, sizeof(CollisionBox::SweepResolution));
			}
			else if (res.time < nearest.time)
			{
				ptr = (&(*it).second);
				std::memcpy(&nearest, &res, sizeof(CollisionBox::SweepResolution));
			}
		}
	}
	if (ptr != nullptr)
		this->_resolve_sweep_collision(reinterpret_cast<Player *>(this->_active),
									   (*ptr).getCollisionBox(), nearest, ptr);
}

inline void RunnerWorld::_resolve_sweep_collision(Player *player, CollisionBox const &box,
												  CollisionBox::SweepResolution const &res,
												  ICollidable const *ptr)
{
	glm::vec3 new_delta;

	new_delta.x = !std::isnan(-res.res.delta.x) ? -res.res.delta.x : 0.0f;
	new_delta.y = !std::isnan(-res.res.delta.y) ? -res.res.delta.y : 0.0f;
	new_delta.z = !std::isnan(-res.res.delta.z) ? -res.res.delta.z : 0.0f;
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
		player->setVelocityYtoZero();
		player->setSurfaceCollisionBox(box);
		player->setOnSurface(true);
		player->setCurJumpToMax();
		player->setCurHooverTimeToMax();
	}
	else
		player->setVelocityZtoZero();
	if (!reinterpret_cast<Player *>(this->_active)->isImmune() &&
		ptr->getDamages() != ICollidable::Damages::NONE)
	{
		reinterpret_cast<Player *>(this->_active)->lowerHP(ptr->getDamages());
		reinterpret_cast<Player *>(this->_active)->setImmunityTimerToMax();
		reinterpret_cast<Player *>(this->_active)->playSound("damage");
	}
}

inline void RunnerWorld::_check_collidable_box(CollidableBox const &cb,
											   CollisionBox::SweepResolution *nearest,
											   glm::vec3 const &inv_delta,
											   ICollidable const **ptr)
{
	CollisionBox::SweepResolution res;

	if ((reinterpret_cast<Player *>(this->_active)->getCollisionBox().
			IsBoxInBoxSweep(cb.getCollisionBox(), inv_delta, &res)))
	{
		if (*ptr == nullptr)
		{
			*ptr = static_cast<ICollidable const *>(&cb);
			std::memcpy(nearest, &res, sizeof(CollisionBox::SweepResolution));
		}
		else if (res.time < nearest->time)
		{
			*ptr = static_cast<ICollidable const *>(&cb);
			std::memcpy(nearest, &res, sizeof(CollisionBox::SweepResolution));
		}
	}
}

inline void RunnerWorld::_add_pos_for_check(glm::vec3 const &pos)
{
	static size_t i = 0;

	this->_check_stuck[i] = std::trunc(pos.x);
	i = (i == MAX_STUCK_FRAME - 1) ? 0 : (i + 1);
}

inline bool RunnerWorld::_should_spawn_full_obstacle(void)
{
	std::mt19937_64                       generator(this->_rd());
	std::uniform_int_distribution<size_t> distri;
	size_t                                value_for_random = INITIAL_MAX_SPAWN_CHANCE;

	value_for_random = (this->_laps >= INITIAL_MAX_SPAWN_CHANCE) ? 0 : value_for_random - this->_laps;
	distri           = std::uniform_int_distribution<size_t>(0, value_for_random);
	if (!distri(generator))
		return (true);
	return (false);
}

RunnerWorld::RunnerWorldFailException::RunnerWorldFailException(void)
{
	this->_msg = "RunnerWorld : Something failed";
}

RunnerWorld::RunnerWorldFailException::~RunnerWorldFailException(void) throw()
{
}

RunnerWorld::RoomNotFoundException::RoomNotFoundException(void)
{
	this->_msg = "RunnerWorld : Template Room not found";
}

RunnerWorld::RoomNotFoundException::~RoomNotFoundException(void) throw()
{
}

size_t RunnerWorld::list_size = 20;