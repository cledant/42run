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

#include "CollidableProp.hpp"

CollidableProp::Params::Params(void) : cb(CollisionBox(this->prop_params.pos,
													   glm::vec3(1.0f, 1.0f, 1.0f)))
{
	this->dmg            = ICollidable::Damages::SINGLE;
	this->passthrough    = false;
	this->score_modifier = 0;
	this->active         = true;
	this->pick_up                = std::string("NONE");
}

CollidableProp::Params::~Params(void)
{
}

CollidableProp::CollidableProp(CollidableProp::Params const &params) :
		_prop(params.prop_params), _cb(params.cb), _dmg(params.dmg),
		_passthrough(params.passthrough), _score_modifier(params.score_modifier),
		_active(params.active), _pick_up(params.pick_up)
{
	this->update(0.0f);
}

CollidableProp::~CollidableProp(void)
{
}

/*
 * Interface ICollidable
 */

void CollidableProp::setActive(bool value)
{
	this->_active = value;
}

void CollidableProp::setPassthrough(bool value)
{
	this->_passthrough = value;
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
		this->_prop.update(time);
}

void CollidableProp::draw(void)
{
	if (this->_active)
		this->_prop.draw();
}