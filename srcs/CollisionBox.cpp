/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollisionBox.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CollisionBox.hpp"

CollisionBox::CollisionBox(glm::vec3 const &pos, glm::vec3 const &half_size) :
		_pos(pos), _half_size(half_size)
{
}

CollisionBox::~CollisionBox(void)
{
}

CollisionBox::CollisionBox(CollisionBox const &src)
{
	this->_pos        = src.getPos();
}

CollisionBox &CollisionBox::operator=(CollisionBox const &rhs)
{
	this->_pos        = rhs.getPos();
	return (*this);
}

void CollisionBox::setPos(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void CollisionBox::setHalfSize(glm::vec3 const &half_size)
{
	this->_half_size = half_size;
}

glm::vec3 const &CollisionBox::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &CollisionBox::getHalfSize(void) const
{
	return (this->_half_size);
}

CollisionBox::InitException::InitException(void)
{
	this->_msg = "CollisionBox : Object initialization failed";
}

CollisionBox::InitException::~InitException(void) throw()
{
}
