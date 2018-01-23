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

#include "GameEntities/Room.hpp"

Room::Params::Params(void) : room_cb(CollisionBox(glm::vec3(0.0f, 0.0f, 0.0f),
												  glm::vec3(1.1f, 1.1f, 1.1f))),
							 floor(CollidableBox::Params()),
							 roof(CollidableBox::Params()),
							 right_wall(CollidableBox::Params()),
							 left_wall(CollidableBox::Params()),
							 front_wall(CollidableBox::Params()),
							 perspec_mult_view(nullptr)
{
	this->floor.pos       = glm::vec3(0.0f, -1.0f, 0.0f);
	this->floor.size      = glm::vec3(1.1f, 0.1f, 1.1f);
	this->roof.pos        = glm::vec3(0.0f, 1.0f, 0.0f);
	this->roof.size       = glm::vec3(1.1f, 0.1f, 1.1f);
	this->right_wall.pos  = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right_wall.size = glm::vec3(1.1f, 0.9f, 0.1f);
	this->left_wall.pos   = glm::vec3(0.0f, 0.0f, -1.0f);
	this->left_wall.size  = glm::vec3(1.1f, 0.9f, 0.1f);
	this->front_wall.pos  = glm::vec3(1.0f, 0.0f, 0.0f);
	this->front_wall.size = glm::vec3(0.1f, 0.9f, 0.9f);
	this->front_wall.dmg  = Damages::INSTANT_DEATH;
}

Room::Params::~Params(void)
{
}

Room::Room(void) : _perspec_mult_view(basic_params.perspec_mult_view),
				   _room_cb(basic_params.room_cb), _floor(basic_params.floor),
				   _roof(basic_params.roof), _right_wall(basic_params.right_wall),
				   _left_wall(basic_params.left_wall),
				   _front_wall(basic_params.front_wall), _pick_up("")
{
}

Room::Room(Room::Params const &params) : _perspec_mult_view(params.perspec_mult_view),
										 _room_cb(params.room_cb), _floor(params.floor),
										 _roof(params.roof), _right_wall(params.right_wall),
										 _left_wall(params.left_wall),
										 _front_wall(params.front_wall), _pick_up("")
{
}

Room::~Room(void)
{
}

Room::Room(Room const &src) : _perspec_mult_view(src.getPerspecMultView()),
							  _room_cb(src.getCollisionBox()),
							  _floor(src.getFloor()),
							  _roof(src.getRoof()),
							  _right_wall(src.getRightWall()),
							  _left_wall(src.getLeftWall()),
							  _front_wall(src.getFrontWall()),
							  _pick_up("")
{
	this->_list_collidable_prop = src.getCollidablePropList();
}

Room &Room::operator=(Room const &rhs)
{
	this->_room_cb              = rhs.getCollisionBox();
	this->_floor                = rhs.getFloor();
	this->_roof                 = rhs.getRoof();
	this->_right_wall           = rhs.getRightWall();
	this->_left_wall            = rhs.getLeftWall();
	this->_front_wall           = rhs.getFrontWall();
	this->_pick_up              = "";
	this->_perspec_mult_view    = rhs.getPerspecMultView();
	this->_list_collidable_prop = rhs.getCollidablePropList();
	return (*this);
}

void Room::addCollidableProp(std::string const &slot, CollidableProp::Params &params)
{
	params.prop_params.perspec_mult_view = this->_perspec_mult_view;
	this->_list_collidable_prop.insert(std::pair<std::string, CollidableProp>
											   (slot, CollidableProp(params)));
}

/*
 * Interface ITranslatable
 */

void Room::translateObject(glm::vec3 const &vec)
{
	this->_room_cb.translateObject(vec);
	this->_floor.translateObject(vec);
	this->_roof.translateObject(vec);
	this->_right_wall.translateObject(vec);
	this->_left_wall.translateObject(vec);
	this->_front_wall.translateObject(vec);
	for (auto it = this->_list_collidable_prop.begin(); it != this->_list_collidable_prop.end(); ++it)
		(*it).second.translateObject(vec);
}

void Room::scaleObject(glm::vec3 const &vec)
{
	glm::vec3 tr = glm::vec3(1.0f, 1.0f, 1.0f) - vec;

	this->_room_cb.scaleObject(vec);
	this->_floor.scaleObject(vec);
	this->_roof.scaleObject(vec);
	this->_right_wall.scaleObject(vec);
	this->_left_wall.scaleObject(vec);
	this->_front_wall.scaleObject(vec);
	this->_floor.translateObject(glm::vec3(0.0f, tr.y, 0.0f));
	this->_roof.translateObject(glm::vec3(0.0f, -tr.y, 0.0f));
	this->_right_wall.translateObject(glm::vec3(0.0f, 0.0f, -tr.z));
	this->_left_wall.translateObject(glm::vec3(0.0f, 0.0f, tr.z));
	this->_front_wall.translateObject(glm::vec3(-tr.x, 0.0f, 0.0f));
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
	for (auto it = this->_list_collidable_prop.begin(); it != this->_list_collidable_prop.end(); ++it)
		(*it).second.update(time);
}

void Room::draw(void)
{
	this->_floor.draw();
	this->_roof.draw();
	this->_right_wall.draw();
	this->_left_wall.draw();
	if (this->_front_wall.getActive())
		this->_front_wall.draw();
	for (auto it = this->_list_collidable_prop.begin(); it != this->_list_collidable_prop.end(); ++it)
	{
		if ((*it).second.getActive())
			(*it).second.draw();
	}
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
 * Setter
 */

void Room::activeFrontWall(bool value)
{
	this->_front_wall.setActive(value);
}

/*
 * Getter
 */

CollidableBox const &Room::getFloor(void) const
{
	return (this->_floor);
}

CollidableBox const &Room::getRoof(void) const
{
	return (this->_roof);
}

CollidableBox const &Room::getRightWall(void) const
{
	return (this->_right_wall);
}

CollidableBox const &Room::getLeftWall(void) const
{
	return (this->_left_wall);
}

CollidableBox const &Room::getFrontWall(void) const
{
	return (this->_front_wall);
}

CollidableProp &Room::getCollidableProp(std::string const &name)
{
	auto it = this->_list_collidable_prop.find(name);

	if (it == this->_list_collidable_prop.end())
		throw Room::BonusNotFoundException();
	return ((*it).second);
}

std::map<std::string, CollidableProp> const &Room::getCollidablePropList() const
{
	return (this->_list_collidable_prop);
};

/*
 * Exceptions
 */

glm::mat4 const *Room::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

Room::BonusNotFoundException::BonusNotFoundException(void)
{
	this->_msg = "Room : Bonus not found";
}

Room::BonusNotFoundException::~BonusNotFoundException(void) throw()
{
}

Room::ObstacleNotFoundException::ObstacleNotFoundException(void)
{
	this->_msg = "Room : Obstacle not found";
}

Room::ObstacleNotFoundException::~ObstacleNotFoundException(void) throw()
{
}

Room::Params Room::basic_params = Room::Params();