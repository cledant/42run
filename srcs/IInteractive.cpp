/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IInteractive.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 12:25:33 by cledant           #+#    #+#             */
/*   Updated: 2017/09/13 12:25:52 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IInteractive.hpp"

IInteractive::IInteractive(void)
{
}

IInteractive::~IInteractive(void)
{
}

IInteractive::IInteractive(IInteractive const &src)
{
	static_cast<void>(src);
}

IInteractive		&IInteractive::operator=(IInteractive const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}
