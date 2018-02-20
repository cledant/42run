/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollidableProp.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:42:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntities/composed/CollidableProp.hpp"

CollidableProp::Params::Params(void) : cb(CollisionBox(this->prop_params.pos,
													   glm::vec3(1.0f, 1.0f, 1.0f)))
{
	this->dmg            = ICollidable::Damages::SINGLE;
	this->passthrough    = false;
	this->score_modifier = 0;
	this->active         = true;
	this->pick_up        = std::string("");
	this->auto_rotation  = false;
}

CollidableProp::Params::~Params(void)
{
}

CollidableProp::CollidableProp(CollidableProp::Params const &params) :
		_prop(params.prop_params), _cb(params.cb), _dmg(params.dmg),
		_passthrough(params.passthrough), _score_modifier(params.score_modifier),
		_active(params.active), _pick_up(params.pick_up),
		_auto_rotation(params.auto_rotation)
{
	this->update(0.0f);
}

CollidableProp::~CollidableProp(void)
{
}

CollidableProp::CollidableProp(CollidableProp const &src) : _prop(src.getProp()),
															_cb(src.getCollisionBox())
{
	this->_dmg            = src.getDamages();
	this->_passthrough    = src.getPassthrough();
	this->_score_modifier = src.getScoreModifier();
	this->_pick_up        = src.getPickUpSound();
	this->_auto_rotation  = src.getAutoRotation();
	this->_active         = src.getActive();
}

CollidableProp &CollidableProp::operator=(CollidableProp const &rhs)
{
	this->_prop           = rhs.getProp();
	this->_cb             = rhs.getCollisionBox();
	this->_dmg            = rhs.getDamages();
	this->_passthrough    = rhs.getPassthrough();
	this->_score_modifier = rhs.getScoreModifier();
	this->_pick_up        = rhs.getPickUpSound();
	this->_auto_rotation  = rhs.getAutoRotation();
	this->_active         = rhs.getActive();
	return (*this);
}

/*
 * Interface ICollidable
 */

void CollidableProp::setPassthrough(bool value)
{
	this->_passthrough = value;
}

void CollidableProp::setActive(bool value)
{
	this->_active = value;
}

CollisionBox const &CollidableProp::getCollisionBox(void) const
{
	return (this->_cb);
}

ICollidable::Damages CollidableProp::getDamages(void) const
{
	return (this->_dmg);
}

int CollidableProp::getScoreModifier(void) const
{
	return (this->_score_modifier);
}

bool CollidableProp::getPassthrough(void) const
{
	return (this->_passthrough);
}

bool CollidableProp::getActive(void) const
{
	return (this->_active);
}

std::string const &CollidableProp::getPickUpSound(void) const
{
	return (this->_pick_up);
}

/*
 * Interface IEntity
 */

void CollidableProp::update(float time)
{
	if (this->_active)
	{
		if (this->_auto_rotation)
			this->_prop.setYaw(this->_prop.getYaw() + time * 50.f);
		this->_prop.update(time);
	}
}

void CollidableProp::draw(void)
{
	if (this->_active)
		this->_prop.draw();
}

/*
 * Interface ITranslatable
 */

void CollidableProp::translateObject(glm::vec3 const &vec)
{
	this->_prop.translateObject(vec);
	this->_cb.translateObject(vec);
}

void CollidableProp::scaleObject(glm::vec3 const &vec)
{
	this->_prop.scaleObject(vec);
	this->_cb.scaleObject(vec);
}

/*
 * Getter
 */

Prop const &CollidableProp::getProp(void) const
{
	return (this->_prop);
}

bool CollidableProp::getAutoRotation(void) const
{
	return (this->_auto_rotation);
}