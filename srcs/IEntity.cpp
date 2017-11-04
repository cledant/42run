/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEntity.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:18 by cledant           #+#    #+#             */
/*   Updated: 2017/09/04 11:44:21 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IEntity.hpp"

IEntity::IEntity(void)
{
}

IEntity::~IEntity(void)
{
}

IEntity::IEntity(IEntity const &src)
{
	static_cast<void>(src);
}

IEntity &IEntity::operator=(IEntity const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}
