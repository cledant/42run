/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollidableProp.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLIDABLEPROP_HPP
# define COLLIDABLEPROP_HPP

# include "Prop.hpp"
# include "CollisionBox.hpp"
# include "ICollidable.hpp"

class CollidableProp : public Prop, public ICollidable
{
	public :

		struct Params
		{
			Params(void);
			~Params(void);

			Prop::Params         prop_params;
			CollisionBox         cb;
			ICollidable::Damages dmg;
			bool                 passthrough;
			int                  score_modifier;
			bool                 active;
		};

		CollidableProp(CollidableProp::Params const &params);
		virtual ~CollidableProp(void);
		CollidableProp(CollidableProp const &src) = delete;
		CollidableProp &operator=(CollidableProp const &rhs) = delete;

		/*
		 * Interface ICollidable
		 */
		void setPassthrough(bool value);
		void setActive(bool value);

		CollisionBox const &getCollisionBox(void) const;
		Damages getDamages(void) const;
		int getScoreModifier(void) const;
		bool getPassthrough(void) const;
		bool getActive(void) const;

	private :

		CollisionBox         _cb;
		ICollidable::Damages _dmg;
		bool                 _passthrough;
		int                  _score_modifier;
		bool                 _active;
};

#endif