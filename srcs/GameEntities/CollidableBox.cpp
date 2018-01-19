/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollidableBox.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntities/CollidableBox.hpp"

CollidableBox::Params::Params(void)
{
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->model             = nullptr;
	this->pos               = glm::vec3(0.0f, 0.0f, 0.0f);
	this->size              = glm::vec3(1.0f, 1.0f, 1.0f);
	this->dmg               = ICollidable::Damages::NONE;
	this->passthrough       = false;
	this->score_modifier    = 0;
	this->active            = true;
	this->pick_up           = std::string("");
}

CollidableBox::Params::~Params(void)
{
}

CollidableBox::CollidableBox(CollidableBox::Params const &params) :
		_cm(params.shader, params.perspec_mult_view, params.model,
			params.pos, params.size), _cb(params.pos, params.size),
		_pos(params.pos), _dmg(params.dmg), _passthrough(params.passthrough),
		_score_modifier(params.score_modifier), _active(params.active),
		_pick_up(params.pick_up)
{
	this->update(1.0f);
}

CollidableBox::~CollidableBox(void)
{
}

CollidableBox::CollidableBox(CollidableBox const &src) : _cm(src.getCubemap()),
														 _cb(src.getCollisionBox())
{
	this->_pos         = src.getPos();
	this->_dmg         = src.getDamages();
	this->_passthrough = src.getPassthrough();
	this->_active      = src.getActive();
	this->_pick_up     = src.getPickUpSound();
}

CollidableBox &CollidableBox::operator=(CollidableBox const &rhs)
{
	this->_cm          = rhs.getCubemap();
	this->_cb          = rhs.getCollisionBox();
	this->_pos         = rhs.getPos();
	this->_dmg         = rhs.getDamages();
	this->_passthrough = rhs.getPassthrough();
	this->_active      = rhs.getActive();
	this->_pick_up     = rhs.getPickUpSound();
	return (*this);
}

/*
 * Interface ITranslatable
 */

void CollidableBox::translateObject(glm::vec3 const &vec)
{
	this->_pos += vec;
	this->_cm.translateObject(vec);
	this->_cb.translateObject(vec);
}

void CollidableBox::scaleObject(glm::vec3 const &vec)
{
	this->_cm.scaleObject(vec);
	this->_cb.scaleObject(vec);
}

/*
 * Interface ICollidable
 */

void CollidableBox::setPassthrough(bool value)
{
	this->_passthrough = value;
}

void CollidableBox::setActive(bool value)
{
	this->_active = value;
}

CollisionBox const &CollidableBox::getCollisionBox(void) const
{
	return (this->_cb);
}

ICollidable::Damages CollidableBox::getDamages(void) const
{
	return (this->_dmg);
}

int CollidableBox::getScoreModifier(void) const
{
	return (this->_score_modifier);
}

bool CollidableBox::getPassthrough(void) const
{
	return (this->_passthrough);
}

bool CollidableBox::getActive(void) const
{
	return (this->_active);
}

std::string const &CollidableBox::getPickUpSound(void) const
{
	return (this->_pick_up);
}

/*
 * Interface IEntity
 */

void CollidableBox::update(float time)
{
	if (this->_active)
		this->_cm.update(time);
}

void CollidableBox::draw(void)
{
	if (this->_active)
		this->_cm.draw();
}

/*
 * Getter
 */

Cubemap const &CollidableBox::getCubemap(void) const
{
	return (this->_cm);
}

glm::vec3 const &CollidableBox::getPos(void) const
{
	return (this->_pos);
}