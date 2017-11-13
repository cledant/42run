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

CollisionBox::CollisionBox(Shader const *shader, glm::mat4 const *perspec_mult_view,
						   glm::vec3 const &pos, glm::vec3 const &min, glm::vec3 const &max,
						   std::string const &name) :
		_debug_name(name), _min(min), _max(max), _pos(pos)
{
	try
	{
		this->_debug_draw = std::make_shared<Cubemap>(shader, perspec_mult_view,
													  std::vector<std::string>(
															  {"./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png"}),
													  pos, this->_max);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		throw CollisionBox::InitException();
	}
}

CollisionBox::~CollisionBox(void)
{
}

CollisionBox::CollisionBox(CollisionBox const &src)
{
	this->_debug_draw = src.getCubemap();
	this->_debug_name = src.getName();
	this->_min        = src.getMin();
	this->_max        = src.getMax();
	this->_pos        = src.getPos();
}

CollisionBox &CollisionBox::operator=(CollisionBox const &rhs)
{
	this->_debug_draw = rhs.getCubemap();
	this->_debug_name = rhs.getName();
	this->_min        = rhs.getMin();
	this->_max        = rhs.getMax();
	this->_pos        = rhs.getPos();
	return (*this);
}

void CollisionBox::update(float time)
{
	this->_debug_draw.get()->update(time);
}

void CollisionBox::draw(void)
{
	this->_debug_draw.get()->draw();
}

void CollisionBox::setPos(glm::vec3 const &pos)
{
	this->_pos = pos;
}

glm::vec3 const &CollisionBox::getMin(void) const
{
	return (this->_min);
}

glm::vec3 const &CollisionBox::getMax(void) const
{
	return (this->_max);
}

glm::vec3 const &CollisionBox::getPos(void) const
{
	return (this->_pos);
}

std::string const &CollisionBox::getName(void) const
{
	return (this->_debug_name);
}

std::shared_ptr<Cubemap> const &CollisionBox::getCubemap(void) const
{
	return (this->_debug_draw);
}

bool CollisionBox::checkCollision(CollisionBox const &box) const
{
	return ((((this->_min.x + this->_pos.x) <= (box.getMax().x + box.getPos().x)) &&
			 ((this->_max.x + this->_pos.x) >= (box.getMin().x + box.getPos().x)))
			&& (((this->_min.y + this->_pos.y) <= (box.getMax().y + box.getPos().y)) &&
				((this->_max.y + this->_pos.y) >= (box.getMin().y + box.getPos().y)))
			&& (((this->_min.z + this->_pos.z) <= (box.getMax().z + box.getPos().z)) &&
				((this->_max.z + this->_pos.z) >= (box.getMin().z + box.getPos().z))));
}

CollisionBox::InitException::InitException(void)
{
	this->_msg = "CollisionBox : Object initialization failed";
}

CollisionBox::InitException::~InitException(void) throw()
{
}

/*
 *  Set of functions for collision unit_test
 */

void CollisionBox::debug_checkCollision(CollisionBox const &box) const
{
	std::cout << "Collision between : " << this->_debug_name << " and " << box.getName() << " = " << this->checkCollision(box) << std::endl;
}