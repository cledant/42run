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

CollidableBox::Params::Params(void)
{
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->tex               = nullptr;
	this->pos               = glm::vec3(0.0f, 0.0f, 0.0f);
	this->size              = glm::vec3(1.0f, 1.0f, 1.0f);
	this->dmg               = ICollidable::Damages::NONE;
	this->passthrough       = false;
	this->score_modifier    = 0;
	this->active            = true;
	this->pick_up           = std::string("NONE");
}

CollidableBox::Params::~Params(void)
{
}

CollidableBox::CollidableBox(CollidableBox::Params const &params) :
		_model(params.shader, params.perspec_mult_view, params.tex,
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

glm::vec3 const &CollidableBox::getPos(void) const
{
	return (this->_pos);
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
		this->_model.update(time);
}

void CollidableBox::draw(void)
{
	if (this->_active)
		this->_model.draw();
}
