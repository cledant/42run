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

World::World(Input const &input, GLFW_Window const &win, glm::vec3 cam_pos,
			 float max_fps, size_t max_frame_skip) :
		_active(nullptr), _input(input), _window(win),
		_camera(input, cam_pos, 2.0f, glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, 0.0f),
		_fov(45.0f), _max_fps(max_fps),
		_max_frame_skip(max_frame_skip), _next_update_tick(0.0f),
		_last_update_tick(0.0f), _delta_tick(0.0f), _skip_loop(0),
		_input_timer(0.0f), _input_mouse_timer(0.0f), _gravity(glm::vec3(0.0f, -10.0f, 0.0f))
{
	if (max_frame_skip == 0)
		throw World::WorldFailException();
	GLfloat ratio = static_cast<GLfloat>(win.cur_win_w) /
					static_cast<GLfloat>(win.cur_win_h);
	this->_tick        = 1.0f / this->_max_fps;
	this->_perspective = glm::perspective(glm::radians(this->_fov), ratio, 0.1f,
										  400.0f);
	this->_camera.update_third_person(true, glm::vec3{0.0f, 0.0f, 0.0f});
}

World::~World(void)
{
	static_cast<void>(this->_input);
	std::vector<IEntity *>::iterator it;

	for (it = this->_entity_list.begin(); it != this->_entity_list.end(); ++it)
		delete *it;
}

void World::update(void)
{
	std::vector<IEntity *>::iterator it;


	if (this->_window.resized == true)
		this->updatePerspective(this->_fov);
	if (this->_active == nullptr)
	{
		this->_camera.update_third_person(this->_input.mouse_exclusive, glm::vec3({0.0f, 0.0f, 0.0f}));
		this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
	}
	else
	{
		if (this->_active->update_mouse_interaction(this->_input, this->_window,
													this->_camera.getPos(), std::vector<glm::vec3 const *>{
						&(this->_camera.getFront()), &(this->_camera.getUp()),
						&(this->_camera.getRight())}, this->_input_mouse_timer) == true)
			this->_input_mouse_timer = 0.0f;
		else if (this->_input_mouse_timer < 1.0f)
			this->_input_mouse_timer += this->_tick;
		if (this->_active->update_keyboard_interaction(this->_input,
													   this->_input_timer) == true)
			this->_input_timer       = 0.0f;
		else if (this->_input_timer < 1.0f)
			this->_input_timer += this->_tick;
		reinterpret_cast<Player *>(this->_active)->update_gravity(this->_gravity, this->_delta_tick);
		this->_check_collisions();
		reinterpret_cast<Player *>(this->_active)->update(0.0f);
		this->_camera.update_third_person(this->_input.mouse_exclusive,
										  reinterpret_cast<Player *>(this->_active)->getPos());
		this->_perspec_mult_view = this->_perspective * this->_camera.getViewMatrix();
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

IEntity *World::add_Prop(Shader const *shader, Model const *model,
						 glm::vec3 const &pos, glm::vec3 const &orientation,
						 glm::vec3 const &scale)
{
	IEntity *ptr;

	ptr = new Prop(shader, &(this->_perspec_mult_view), model, pos, orientation,
				   scale);
	this->_entity_list.push_back(ptr);
	return (ptr);
}

IInteractive *World::add_Player(Shader const *shader, glm::vec3 const &pos,
								glm::vec3 const &size, Texture const *tex)
{
	IInteractive *ptr;

	ptr = new Player(shader, &(this->_perspec_mult_view), pos, size,
					 &(this->_camera), tex);
	this->_active = ptr;
	return (ptr);
}

IEntity *World::add_CollidableBox(Shader const *shader, glm::vec3 const &pos,
								  glm::vec3 const &size, Texture const *tex)
{
	IEntity *ptr;

	ptr = new CollidableBox(shader, &(this->_perspec_mult_view), pos, size, tex);
	this->_entity_list.push_back(ptr);
	this->_collision_check_list.push_back(reinterpret_cast<CollidableBox *>(ptr));
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

void World::_check_collisions(void)
{
	CollisionBox::SweepResolution res;
	glm::vec3                     inv_delta;

	inv_delta.x = -reinterpret_cast<Player *>(this->_active)->getDelta().x;
	inv_delta.y = -reinterpret_cast<Player *>(this->_active)->getDelta().y;
	inv_delta.z = -reinterpret_cast<Player *>(this->_active)->getDelta().z;
	for (auto it = this->_collision_check_list.begin(); it != this->_collision_check_list.end(); ++it)
	{
		if ((reinterpret_cast<Player *>(this->_active)->getCollisionBox().
				IsBoxInBoxSweep((*it)->getCollisionBox(),
								inv_delta, &res)) == true)
		{
/*			std::cout << "============" << std::endl;
			std::cout << "Inv Delta" << std::endl;
			std::cout << inv_delta.x << std::endl;
			std::cout << inv_delta.y << std::endl;
			std::cout << inv_delta.z << std::endl;
			std::cout << "============" << std::endl;*/
			this->_resolve_sweep_collision(reinterpret_cast<Player *>(this->_active),
										   (*it)->getCollisionBox(), inv_delta, &res);
			break;
		}
	}
}

void World::_resolve_sweep_collision(Player *player, CollisionBox const &box,
									 glm::vec3 const &inv_delta,
									 CollisionBox::SweepResolution const *res)
{
	glm::vec3 new_delta;

	(void) box;
	(void) inv_delta;
/*	std::cout << "============" << std::endl;
	std::cout << "RES TIME pos" << std::endl;
	std::cout << res->res.time << std::endl;
	std::cout << "PLAYER pos" << std::endl;
	std::cout << player->getPos().x << std::endl;
	std::cout << player->getPos().y << std::endl;
	std::cout << player->getPos().z << std::endl;
	std::cout << "PLAYER half size" << std::endl;
	std::cout << player->getCollisionBox().getHalfSize().x << std::endl;
	std::cout << player->getCollisionBox().getHalfSize().y << std::endl;
	std::cout << player->getCollisionBox().getHalfSize().z << std::endl;
	std::cout << "PLAYER delta" << std::endl;
	std::cout << player->getDelta().x << std::endl;
	std::cout << player->getDelta().y << std::endl;
	std::cout << player->getDelta().z << std::endl;
	std::cout << "BOX pos" << std::endl;
	std::cout << box.getPos().x << std::endl;
	std::cout << box.getPos().y << std::endl;
	std::cout << box.getPos().z << std::endl;
	std::cout << "BOX half size" << std::endl;
	std::cout << box.getHalfSize().x << std::endl;
	std::cout << box.getHalfSize().y << std::endl;
	std::cout << box.getHalfSize().z << std::endl;
	std::cout << "RESOLUTION" << std::endl;
	std::cout << "Sweep pos" << std::endl;
	std::cout << res->pos.x << std::endl;
	std::cout << res->pos.x << std::endl;
	std::cout << res->pos.x << std::endl;
	std::cout << "Hit pos" << std::endl;
	std::cout << res->res.pos.x << std::endl;
	std::cout << res->res.pos.y << std::endl;
	std::cout << res->res.pos.z << std::endl;
	std::cout << "Hit delta" << std::endl;
	std::cout << res->res.delta.x << std::endl;
	std::cout << res->res.delta.y << std::endl;
	std::cout << res->res.delta.z << std::endl;
	std::cout << "Hit normal" << std::endl;
	std::cout << res->res.normal.x << std::endl;
	std::cout << res->res.normal.y << std::endl;
	std::cout << res->res.normal.z << std::endl;
	std::cout << "Inv Delta" << std::endl;
	std::cout << inv_delta.x << std::endl;
	std::cout << inv_delta.y << std::endl;
	std::cout << inv_delta.z << std::endl;*/

	new_delta.x = !isnan(-res->res.delta.x) ? -res->res.delta.x : 0.0f;
	new_delta.y = !isnan(-res->res.delta.y) ? -res->res.delta.y : 0.0f;
	new_delta.z = !isnan(-res->res.delta.z) ? -res->res.delta.z : 0.0f;
	if (res->res.normal.y != 0.0f)
		new_delta.y += (res->res.normal.y < 0.0f) ? (player->getCollisionBox().getHalfSize().y * 0.01) :
					   -(player->getCollisionBox().getHalfSize().y * 0.01);
	else if (res->res.normal.x != 0.0f)
		new_delta.x += (res->res.normal.x < 0.0f) ? (player->getCollisionBox().getHalfSize().x * 0.01) :
					   -(player->getCollisionBox().getHalfSize().x * 0.01);
	else if (res->res.normal.z != 0.0f)
		new_delta.z += (res->res.normal.z < 0.0f) ? (player->getCollisionBox().getHalfSize().z * 0.01) :
					   -(player->getCollisionBox().getHalfSize().z * 0.01);
	player->setDelta(new_delta);
/*	std::cout << "PLAYER delta 2" << std::endl;
	std::cout << player->getDelta().x << std::endl;
	std::cout << player->getDelta().y << std::endl;
	std::cout << player->getDelta().z << std::endl;
	std::cout << "============" << std::endl;*/
}

World::WorldFailException::WorldFailException(void)
{
	this->_msg = "World : Something failed";
}

World::WorldFailException::~WorldFailException(void) throw()
{
}
