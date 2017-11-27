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
							   CollisionBox const &cb, ICollidable::Damages dmg) :
		Prop(shader, perspec_mult_view, model, pos, orientation, scale),
		_cb(cb), _dmg(dmg)
{
	this->update(0.0f);
}

CollidableProp::~CollidableProp(void)
{
}

CollisionBox const &CollidableProp::getCollisionBox(void) const
{
	return (this->_cb);
}

ICollidable::Damages CollidableProp::getDamages(void) const
{
	return (this->_dmg);
}