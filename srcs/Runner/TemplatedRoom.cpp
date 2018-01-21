/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TemplatedRoom.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Runner/TemplatedRoom.hpp"

void NormalRoomEmpty(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params player_params;
	Room::Params   room_params;
	Room           *room = nullptr;

	room_params.floor.shader      = &(oGL.getShader("cubemap"));
	room_params.floor.model       = &(oGL.getModel("TestBox"));
	room_params.roof.shader       = &(oGL.getShader("cubemap"));
	room_params.roof.model        = &(oGL.getModel("TestBox"));
	room_params.right_wall.shader = &(oGL.getShader("cubemap"));
	room_params.right_wall.model  = &(oGL.getModel("TestBox"));
	room_params.left_wall.shader  = &(oGL.getShader("cubemap"));
	room_params.left_wall.model   = &(oGL.getModel("TestBox"));
	room_params.front_wall.shader = &(oGL.getShader("cubemap"));
	room_params.front_wall.model  = &(oGL.getModel("TestBox"));
	room_params.front_wall.active = false;
	room = world.addRoomTemplate("NormalRoomEmpty", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
}

void NormalRoomBonusOnly(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	CollidableProp::Params bonus_params;
	Room::Params           room_params;
	Room                   *room     = nullptr;
	size_t                 c         = 0;
	std::string            slot_name = "Slot";

	//Room Params
	room_params.floor.shader      = &(oGL.getShader("cubemap"));
	room_params.floor.model       = &(oGL.getModel("TestBox"));
	room_params.roof.shader       = &(oGL.getShader("cubemap"));
	room_params.roof.model        = &(oGL.getModel("TestBox"));
	room_params.right_wall.shader = &(oGL.getShader("cubemap"));
	room_params.right_wall.model  = &(oGL.getModel("TestBox"));
	room_params.left_wall.shader  = &(oGL.getShader("cubemap"));
	room_params.left_wall.model   = &(oGL.getModel("TestBox"));
	room_params.front_wall.shader = &(oGL.getShader("cubemap"));
	room_params.front_wall.model  = &(oGL.getModel("TestBox"));
	room_params.front_wall.active = false;
	room = world.addRoomTemplate("NormalRoomBonusOnly", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));

	//Bonus Params
	bonus_params.prop_params.shader      = &(oGL.getShader("prop"));
	bonus_params.prop_params.model       = &(oGL.getModel("cola"));
	bonus_params.prop_params.scale       = glm::vec3(0.03f, 0.03f, 0.03f);
	bonus_params.prop_params.orientation = glm::vec3(0.0f, 20.0f, 0.0f);
	bonus_params.dmg                     = ICollidable::Damages::NONE;
	bonus_params.pick_up                 = std::string("bonus");
	bonus_params.passthrough             = true;
	bonus_params.score_modifier          = 500;
	bonus_params.auto_rotation           = true;
	bonus_params.prop_params.pos         = glm::vec3(-4.0f, -2.0f, 1.5f);

	for (size_t j = 0; j < 3; ++j)
	{
		for (size_t i = 0; i < 5; ++i)
		{
			slot_name = "Slot";
			bonus_params.prop_params.pos = glm::vec3(-4.0f + (i * 2.0f), -2.0f, -1.5f + (j * 1.5f));
			bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
														glm::vec3(0.14f, 0.14f, 0.14f));
			slot_name += std::to_string(c);
			room->addBonus(slot_name, bonus_params);
			c++;
		}
	}
}

void NormalRoomObstacleOnly(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	CollidableProp::Params obstacle_params;
	Room::Params           room_params;
	Room                   *room     = nullptr;
	size_t                 c         = 0;
	std::string            slot_name = "Slot";

	//Room Params
	room_params.floor.shader      = &(oGL.getShader("cubemap"));
	room_params.floor.model       = &(oGL.getModel("TestBox"));
	room_params.roof.shader       = &(oGL.getShader("cubemap"));
	room_params.roof.model        = &(oGL.getModel("TestBox"));
	room_params.right_wall.shader = &(oGL.getShader("cubemap"));
	room_params.right_wall.model  = &(oGL.getModel("TestBox"));
	room_params.left_wall.shader  = &(oGL.getShader("cubemap"));
	room_params.left_wall.model   = &(oGL.getModel("TestBox"));
	room_params.front_wall.shader = &(oGL.getShader("cubemap"));
	room_params.front_wall.model  = &(oGL.getModel("TestBox"));
	room_params.front_wall.active = false;
	room = world.addRoomTemplate("NormalRoomObstacleOnly", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));

	//Obstacle Params
	obstacle_params.prop_params.shader = &(oGL.getShader("prop"));
	obstacle_params.prop_params.model  = &(oGL.getModel("Sakuya"));
	obstacle_params.prop_params.scale  = glm::vec3(0.005f, 0.005f, 0.005f);
	obstacle_params.prop_params.offset = glm::vec3(0.0f, 0.1f, 0.0f);
	obstacle_params.dmg                = ICollidable::Damages::HALF;

	for (size_t j = 0; j < 3; ++j)
	{
		for (size_t i = 0; i < 5; ++i)
		{
			slot_name = "Slot";
			obstacle_params.prop_params.pos = glm::vec3(-4.0f + (i * 2.0f), -2.0f, -1.5f + (j * 1.5f));
			obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
														   glm::vec3(0.14f, 0.14f, 0.14f));
			slot_name += std::to_string(c);
			room->addBonus(slot_name, obstacle_params);
			c++;
		}
	}
}