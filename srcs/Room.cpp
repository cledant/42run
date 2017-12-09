/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Room.hpp"

Room::Params::Params(void) : room_cb(CollisionBox(glm::vec3(0.0f, 0.0f, 0.0f),
												  glm::vec3(0.5f, 0.5f, 0.5f))),
							 floor(CollidableBox::Params()),
							 roof(CollidableBox::Params()),
							 right_wall(CollidableBox::Params()),
							 left_wall(CollidableBox::Params()),
							 front_wall(CollidableBox::Params())
{
}

Room::Params::~Params(void)
{
}

Room::Room(Room::Params const &params) : _room_cb(params.room_cb), _floor(params.floor),
										 _roof(params.roof), _right_wall(params.right_wall),
										 _left_wall(params.left_wall),
										 _front_wall(params.front_wall)
{
}

Room::~Room(void)
{
}

Room::Room(Room &&src) : _room_cb(src.getCollisionBox()),
						 _floor(std::move(src.getFloor())),
						 _roof(std::move(src.getRoof())),
						 _right_wall(std::move(src.getRightWall())),
						 _left_wall(std::move(src.getLeftWall())),
						 _front_wall(std::move(src.getFrontWall()))
{
}

Room &Room::operator=(Room &&rhs)
{
	this->_room_cb    = rhs.getCollisionBox();
	this->_floor      = std::move(rhs.getFloor());
	this->_roof       = std::move(rhs.getRoof());
	this->_right_wall = std::move(rhs.getRightWall());
	this->_left_wall  = std::move(rhs.getLeftWall());
	this->_front_wall = std::move(rhs.getFrontWall());
	return (*this);
}

/*
 * Interface IEntity
 */
void Room::update(float time)
{
	this->_floor.update(time);
	this->_roof.update(time);
	this->_right_wall.update(time);
	this->_left_wall.update(time);
	if (this->_front_wall.getActive())
		this->_front_wall.update(time);
}

void Room::draw(void)
{
	this->_floor.draw();
	this->_roof.draw();
	this->_right_wall.draw();
	this->_left_wall.draw();
	if (this->_front_wall.getActive())
		this->_front_wall.draw();
}

/*
 * Interface ICollidable
 */

void Room::setPassthrough(bool value)
{
	static_cast<void>(value);
}

void Room::setActive(bool value)
{
	static_cast<void>(value);
}

CollisionBox const &Room::getCollisionBox(void) const
{
	return (this->_room_cb);
}

ICollidable::Damages Room::getDamages(void) const
{
	return (ICollidable::Damages::NONE);
}

int Room::getScoreModifier(void) const
{
	return (0);
}

bool Room::getPassthrough(void) const
{
	return (true);
}

bool Room::getActive(void) const
{
	return (true);
}

std::string const &Room::getPickUpSound(void) const
{
	return (this->_pick_up);
}

/*
 * Getter
 */

CollidableBox &Room::getFloor(void)
{
	return (this->_floor);
}

CollidableBox &Room::getRoof(void)
{
	return (this->_roof);
}

CollidableBox &Room::getRightWall(void)
{
	return (this->_right_wall);
}

CollidableBox &Room::getLeftWall(void)
{
	return (this->_left_wall);
}

CollidableBox &Room::getFrontWall(void)
{
	return (this->_front_wall);
}