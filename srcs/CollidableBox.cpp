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

#include "CollidableBox.hpp"

CollidableBox::CollidableBox(Shader const *shader, glm::mat4 const *perspec_mult_view,
							 glm::vec3 const &pos, glm::vec3 const &size,
							 Texture const *tex) :
		_model(shader, perspec_mult_view, tex, pos, size), _cb(pos, size),
		_pos(pos)
{
	this->update(1.0f);
}

CollidableBox::~CollidableBox(void)
{
}

glm::vec3 const &CollidableBox::getPos(void) const
{
	return (this->_pos);
}

CollisionBox const &CollidableBox::getCollisionBox(void) const
{
	return (this->_cb);
}

void CollidableBox::update(float time)
{
	this->_model.update(time);
}

void CollidableBox::draw(void)
{
	this->_model.draw();
}
