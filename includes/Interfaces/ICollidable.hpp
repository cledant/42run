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

# include "GameEntity/CollisionBox.hpp"
# include <string>

class ICollidable
{
	public :

		enum Damages
		{
			NONE          = 0,
			HALF          = 1,
			SINGLE        = 2,
			INSTANT_DEATH = 10000,
		};

		ICollidable(void);
		virtual ~ICollidable(void);
		ICollidable(ICollidable const &src) = delete;
		ICollidable &operator=(ICollidable const &rhs) = delete;

		virtual void setPassthrough(bool value) = 0;
		virtual void setActive(bool value) = 0;

		virtual CollisionBox const &getCollisionBox(void) const = 0;
		virtual Damages getDamages(void) const = 0;
		virtual int getScoreModifier(void) const = 0;
		virtual bool getPassthrough(void) const = 0;
		virtual bool getActive(void) const = 0;
		virtual std::string const &getPickUpSound(void) const = 0;
};

#endif