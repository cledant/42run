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
	this->_pos = src.getPos();
}

CollisionBox &CollisionBox::operator=(CollisionBox const &rhs)
{
	this->_pos = rhs.getPos();
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

/*
 * If IsPointInBox is given nullptr or NULL as param for res, it will
 * only check collision and wont return a new allocated Resolution struct.
 *
 * If IsPointInBox returns false, it won't change res pointer.
 */
bool CollisionBox::IsPointInBox(glm::vec3 const &pt, Resolution *res) const
{
	glm::vec3 d;
	glm::vec3 p;
	bool      is_in;

	d     = pt - this->_pos;
	p     = this->_half_size - glm::abs(d);
	is_in = (p.x <= 0.0f || p.y <= 0.0f || p.z <= 0.0f) ? false : true;
	if (res == nullptr || res == NULL)
		return (is_in);
	if (is_in == false)
		return (false);
	std::memset(res, 0, sizeof(Resolution));
	if (p.x < p.y)
	{
		if (p.x < p.z)
			this->_resolution_pt_x(res, pt, d, p);
		else
			this->_resolution_pt_z(res, pt, d, p);
	}
	else
	{
		if (p.y < p.z)
			this->_resolution_pt_y(res, pt, d, p);
		else
			this->_resolution_pt_z(res, pt, d, p);
	}
	return (true);
}

float CollisionBox::_sign(float nb)
{
	return ((nb < 0.0f) ? -1.0f : 1.0f);
}

void CollisionBox::_resolution_pt_x(Resolution *res, glm::vec3 const &pt,
									glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.x);
	res->delta.x  = p.x * sign;
	res->normal.x = sign;
	res->pos.x    = this->_pos.x + (this->_half_size.x * sign);
	res->pos.y    = pt.y;
	res->pos.z    = pt.z;
}

void CollisionBox::_resolution_pt_y(Resolution *res, glm::vec3 const &pt,
									glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.y);
	res->delta.y  = p.y * sign;
	res->normal.y = sign;
	res->pos.y    = this->_pos.y + (this->_half_size.y * sign);
	res->pos.x    = pt.x;
	res->pos.z    = pt.z;
}

void CollisionBox::_resolution_pt_z(Resolution *res, glm::vec3 const &pt,
									glm::vec3 const &d, glm::vec3 const &p) const
{
	float sign = CollisionBox::_sign(d.z);
	res->delta.z  = p.z * sign;
	res->normal.z = sign;
	res->pos.z    = this->_pos.z + (this->_half_size.z * sign);
	res->pos.x    = pt.x;
	res->pos.y    = pt.y;
}

CollisionBox::InitException::InitException(void)
{
	this->_msg = "CollisionBox : Object initialization failed";
}

CollisionBox::InitException::~InitException(void) throw()
{
}
