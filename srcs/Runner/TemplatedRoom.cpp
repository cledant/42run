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
	room = world.addRoomTemplate("NormalRoomBonusOnly", room_params);
	room->scaleObject(glm::vec3(6.0f, 3.0f, 3.0f));

	//Bonus Params
	bonus_params.prop_params.shader      = &(oGL.getShader("prop"));
	bonus_params.prop_params.model       = &(oGL.getModel("cola"));
	bonus_params.prop_params.pos         = glm::vec3(-4.0f, -2.0f, 1.5f);
	bonus_params.prop_params.scale       = glm::vec3(0.03f, 0.03f, 0.03f);
	bonus_params.prop_params.orientation = glm::vec3(0.0f, 20.0f, 0.0f);
	bonus_params.cb                      = CollisionBox(bonus_params.prop_params.pos,
														glm::vec3(0.14f, 0.14f, 0.14f));
	bonus_params.dmg                     = ICollidable::Damages::NONE;
	bonus_params.pick_up                 = std::string("bonus");
	bonus_params.passthrough             = true;
	bonus_params.score_modifier          = 500;
	bonus_params.auto_rotation           = true;
	room->addBonus("Slot1", bonus_params);
	bonus_params.prop_params.pos = glm::vec3(0.0f, -2.0f, 1.5f);
	room->addBonus("Slot2", bonus_params);
	bonus_params.prop_params.pos = glm::vec3(4.0f, -2.0f, 1.5f);
	room->addBonus("Slot3", bonus_params);
	bonus_params.prop_params.pos = glm::vec3(2.0f, -2.0f, 0.0f);
	room->addBonus("Slot4", bonus_params);
	bonus_params.prop_params.pos = glm::vec3(-2.0f, -2.0f, 0.0f);
	room->addBonus("Slot5", bonus_params);
	bonus_params.prop_params.pos = glm::vec3(0.0f, -2.0f, -1.5f);
	room->addBonus("Slot6", bonus_params);
}