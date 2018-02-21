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

glm::vec3 g_bonus_cb_size(0.14f);

glm::vec3 g_colamachine_cb_size(0.1, 0.7f, 0.1f);

void initBonusParams(CollidableProp::Params &bonus_params, oGL_module &oGL)
{
	bonus_params.prop_params.shader      = &(oGL.getShader("prop"));
	bonus_params.prop_params.model       = &(oGL.getModel("cola"));
	bonus_params.prop_params.scale       = glm::vec3(0.03f, 0.03f, 0.03f);
	bonus_params.prop_params.orientation = glm::vec3(0.0f, 20.0f, 0.0f);
	bonus_params.dmg                     = ICollidable::Damages::NONE;
	bonus_params.active                  = false;
	bonus_params.pick_up                 = std::string("bonus");
	bonus_params.passthrough             = true;
	bonus_params.score_modifier          = 500;
	bonus_params.auto_rotation           = true;
	bonus_params.active                  = false;
}

void initObstacleParams(CollidableProp::Params &obstacle_params, oGL_module &oGL)
{
	obstacle_params.prop_params.shader      = &(oGL.getShader("prop"));
	obstacle_params.prop_params.model       = &(oGL.getModel("cola_machine"));
	obstacle_params.prop_params.scale       = glm::vec3(0.006f);
	obstacle_params.prop_params.offset      = glm::vec3(0.0f, 0.2f, 0.0f);
	obstacle_params.prop_params.orientation = glm::vec3(-90.0f, -90.0f, 0.0f);
	obstacle_params.dmg                     = ICollidable::Damages::HALF;
	obstacle_params.active                  = false;
}

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

	initBonusParams(bonus_params, oGL);
	for (size_t i = 0; i < 5; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			slot_name = "Slot";
			slot_name += std::to_string(c);
			if (!(i % 2))
			{
				bonus_params.prop_params.pos = glm::vec3(-4.0f + (i * 2.0f), -2.25f, -1.75f + (j * 1.75f));
				bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
															g_bonus_cb_size);
				room->addCollidableProp(slot_name, bonus_params);
			}
			else
			{
				bonus_params.prop_params.pos = glm::vec3(-4.0f + (i * 2.0f), -2.25f, -0.875f + (j * 1.75f));
				bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
															g_bonus_cb_size);
				room->addCollidableProp(slot_name, bonus_params);
				if (j >= 1)
					j = 3;
			}
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

	initObstacleParams(obstacle_params, oGL);

	//1st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(0.0f, -2.5f, -0.5f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot0", obstacle_params);

	//2nd Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, -2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot1", obstacle_params);

	//3td Obstacle
	obstacle_params.prop_params.pos = glm::vec3(5.0f, -2.5f, -2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot2", obstacle_params);

	//4th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(3.0f, -2.5f, -1.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot3", obstacle_params);

	//5th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(0.0f, -2.5f, 0.5f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot4", obstacle_params);

	//6th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, 2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot5", obstacle_params);

	//7th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(5.0f, -2.5f, 2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot6", obstacle_params);

	//8th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(3.0f, -2.5f, 1.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot7", obstacle_params);

	//9th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-2.5f, -2.5f, 0.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot8", obstacle_params);

	//10th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(3.0f, -2.5f, 0.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot9", obstacle_params);
}

void NormalRoomBonusAndObstacle(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	CollidableProp::Params obstacle_params;
	CollidableProp::Params bonus_params;
	Room::Params           room_params;
	Room                   *room = nullptr;

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
	room = world.addRoomTemplate("NormalRoomBonusAndObstacle", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));

	initObstacleParams(obstacle_params, oGL);
	initBonusParams(bonus_params, oGL);

	//1st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(0.0f, -2.5f, 0.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot0", obstacle_params);

	//2sd Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-2.0f, -2.5f, -2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot1", obstacle_params);

	//3td Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-2.0f, -2.5f, 2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot2", obstacle_params);

	//4th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, -2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot3", obstacle_params);

	//5th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, 2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot4", obstacle_params);

	//1st bonus
	bonus_params.prop_params.pos = glm::vec3(0.0f, -2.25f, -1.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot5", bonus_params);

	//2nd bonus
	bonus_params.prop_params.pos = glm::vec3(0.0f, -2.25f, 1.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot6", bonus_params);

	//3td bonus
	bonus_params.prop_params.pos = glm::vec3(-4.5f, -2.25f, 0.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot7", bonus_params);

	//4th bonus
	bonus_params.prop_params.pos = glm::vec3(4.5f, -2.25f, 0.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot8", bonus_params);

	//7th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(4.5f, -2.5f, -1.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot9", obstacle_params);

	//6th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(4.5f, -2.5f, 1.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot10", obstacle_params);
}

void FallRightRoomEmpty(RunnerWorld &world, oGL_module &oGL)
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
	room = world.addRoomTemplate("FallRightRoomEmpty", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).translateObject(glm::vec3(0.0f, 0.0f, 3.2f));
}

void FallRightRoomObstacle(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	Room::Params           room_params;
	CollidableProp::Params obstacle_params;
	CollidableProp::Params bonus_params;
	Room                   *room = nullptr;

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
	room = world.addRoomTemplate("FallRightRoomObstacle", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).translateObject(glm::vec3(0.0f, 0.0f, 3.2f));

	initObstacleParams(obstacle_params, oGL);
	initBonusParams(bonus_params, oGL);

	//1st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, 0.1f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot0", obstacle_params);

	//2nd Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, 2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot1", obstacle_params);

	//3td Obstacle
	obstacle_params.prop_params.pos = glm::vec3(5.0f, -2.5f, 0.3f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot2", obstacle_params);

	//4th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(4.5f, -2.5f, 2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot3", obstacle_params);

	//5th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-2.0f, -2.5f, 1.1f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot4", obstacle_params);

	//6th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, 1.1f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot5", obstacle_params);

	//7th Bonus
	bonus_params.prop_params.pos = glm::vec3(-3.0f, -1.2f, -1.1f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot6", bonus_params);

	//8th Bonus
	bonus_params.prop_params.pos = glm::vec3(-1.0f, -1.2f, 2.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot7", bonus_params);
}

void FallLeftRoomEmpty(RunnerWorld &world, oGL_module &oGL)
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
	room = world.addRoomTemplate("FallLeftRoomEmpty", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).translateObject(glm::vec3(0.0f, 0.0f, -3.2f));
}

void FallLeftRoomObstacle(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	Room::Params           room_params;
	CollidableProp::Params obstacle_params;
	CollidableProp::Params bonus_params;
	Room                   *room = nullptr;

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
	room = world.addRoomTemplate("FallLeftRoomObstacle", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).translateObject(glm::vec3(0.0f, 0.0f, -3.2f));

	initBonusParams(bonus_params, oGL);
	initObstacleParams(obstacle_params, oGL);

	//1st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, -0.1f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot0", obstacle_params);

	//2nd Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, -2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot1", obstacle_params);

	//3td Obstacle
	obstacle_params.prop_params.pos = glm::vec3(5.0f, -2.5f, -0.3f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot2", obstacle_params);

	//4th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(4.5f, -2.5f, -2.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   glm::vec3(0.14f, 0.14f, 0.14f));
	room->addCollidableProp("Slot3", obstacle_params);

	//5th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-2.0f, -2.5f, -1.1f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot4", obstacle_params);

	//6th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, -1.1f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot5", obstacle_params);

	//7th Bonus
	bonus_params.prop_params.pos = glm::vec3(-3.0f, -1.2f, 1.1f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot6", bonus_params);

	//8th Bonus
	bonus_params.prop_params.pos = glm::vec3(-1.0f, -1.2f, -2.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot7", bonus_params);
}

void FallFrontRoomEmpty(RunnerWorld &world, oGL_module &oGL)
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
	room = world.addRoomTemplate("FallFrontRoomEmpty", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).translateObject(glm::vec3(6.6f * 0.5f, 0.0f, 0.0f));
	const_cast<CollidableBox &>(room->getFloor()).scaleObject(glm::vec3(0.5f, 1.0f, 1.0f));
}

void FallFrontRoomObstacle(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	Room::Params           room_params;
	Room                   *room = nullptr;
	CollidableProp::Params obstacle_params;
	CollidableProp::Params bonus_params;

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
	room = world.addRoomTemplate("FallFrontRoomObstacle", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).translateObject(glm::vec3(6.6f * 0.5f, 0.0f, 0.0f));
	const_cast<CollidableBox &>(room->getFloor()).scaleObject(glm::vec3(0.5f, 1.0f, 1.0f));

	initObstacleParams(obstacle_params, oGL);
	initBonusParams(bonus_params, oGL);

	//1st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, -0.6f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot0", obstacle_params);

	//2nd Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, -1.8f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot1", obstacle_params);

	//3td Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, 0.6f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot2", obstacle_params);

	//4st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(2.0f, -2.5f, 1.8f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot3", obstacle_params);

	//5th Bonus
	bonus_params.prop_params.pos = glm::vec3(-4.5f, -1.2f, -1.2f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot4", bonus_params);

	//6th Bonus
	bonus_params.prop_params.pos = glm::vec3(-4.5f, -1.2f, 1.2f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot5", bonus_params);

	//7th Bonus
	bonus_params.prop_params.pos = glm::vec3(-4.5f, -1.2f, 0.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot6", bonus_params);
}

void FallMiddleRoomEmpty(RunnerWorld &world, oGL_module &oGL)
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
	room = world.addRoomTemplate("FallMiddleRoomEmpty", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).scaleObject(glm::vec3(1.0f, 1.0f, 0.3f));
}

void FallMiddleRoomObstacle(RunnerWorld &world, oGL_module &oGL)
{
	Player::Params         player_params;
	Room::Params           room_params;
	CollidableProp::Params bonus_params;
	CollidableProp::Params obstacle_params;
	Room                   *room = nullptr;

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
	room = world.addRoomTemplate("FallMiddleRoomObstacle", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));
	const_cast<CollidableBox &>(room->getFloor()).scaleObject(glm::vec3(1.0f, 1.0f, 0.24f));

	initBonusParams(bonus_params, oGL);
	initObstacleParams(obstacle_params, oGL);

	//1st Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-5.0f, -2.5f, 0.0f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot0", obstacle_params);

	//2nd Obstacle
	obstacle_params.prop_params.pos = glm::vec3(-1.0f, -2.5f, -0.6f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot1", obstacle_params);

	//3td Obstacle
	obstacle_params.prop_params.pos = glm::vec3(0.0f, -2.5f, 0.6f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot2", obstacle_params);

	//4th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(4.0f, -2.5f, 0.6f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot3", obstacle_params);

	//5th Obstacle
	obstacle_params.prop_params.pos = glm::vec3(4.0f, -2.5f, -0.6f);
	obstacle_params.cb              = CollisionBox(obstacle_params.prop_params.pos,
												   g_colamachine_cb_size);
	room->addCollidableProp("Slot4", obstacle_params);

	//6th Bonus
	bonus_params.prop_params.pos = glm::vec3(-2.0f, -1.2f, 1.55f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot5", bonus_params);

	//7th Bonus
	bonus_params.prop_params.pos = glm::vec3(0.0f, -1.2f, -1.55f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot6", bonus_params);

	//8th Bonus
	bonus_params.prop_params.pos = glm::vec3(2.0f, -2.25f, 0.0f);
	bonus_params.cb              = CollisionBox(bonus_params.prop_params.pos,
												g_bonus_cb_size);
	room->addCollidableProp("Slot7", bonus_params);
}