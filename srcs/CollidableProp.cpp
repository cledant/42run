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

CollidableProp::CollidableProp(Prop::Params const &params, CollisionBox const &cb,
							   ICollidable::Damages dmg, bool passthrough,
							   int score_modifier) :
		Prop(params), _cb(cb), _dmg(dmg), _passthrough(passthrough), _score_modifier(score_modifier),
		_active(true)
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