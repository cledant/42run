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

CollidableProp::CollidableProp(Shader const *shader, glm::mat4 const *perspec_mult_view,
							   Model const *model, glm::vec3 const &pos,
							   glm::vec3 const &orientation, glm::vec3 const &scale,
							   glm::vec3 const &offset, CollisionBox const &cb,
							   ICollidable::Damages dmg, bool passthrough,
							   int score_modifier) :
		Prop(shader, perspec_mult_view, model, pos, orientation, scale, offset),
		_cb(cb), _dmg(dmg), _passthrough(passthrough), _score_modifier(score_modifier)
{
	this->update(0.0f);
}

CollidableProp::~CollidableProp(void)
{
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