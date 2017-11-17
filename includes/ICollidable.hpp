/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ICollidable.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICOLLIDABLE_HPP
# define ICOLLIDABLE_HPP

# include "CollisionBox.hpp"

class ICollidable
{
	public :

		ICollidable(void);
		virtual ~ICollidable(void);
		ICollidable(ICollidable const &src) = delete;
		ICollidable &operator=(ICollidable const &rhs) = delete;

		virtual CollisionBox const &getCollisionBox(void) const = 0;
};

#endif