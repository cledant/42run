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

		CollidableProp(Shader const *shader, glm::mat4 const *perspec_mult_view,
					   Model const *model, glm::vec3 const &pos,
					   glm::vec3 const &orientation, glm::vec3 const &scale,
					   glm::vec3 const &offset, CollisionBox const &cb,
					   ICollidable::Damages dmg, bool passthrough, int score_modifier);
		virtual ~CollidableProp(void);
		CollidableProp(CollidableProp const &src) = delete;
		CollidableProp &operator=(CollidableProp const &rhs) = delete;

		/*
		 * Interface ICollidable
		 */
		virtual void setPassthrough(bool value);

		CollisionBox const &getCollisionBox(void) const;
		ICollidable::Damages getDamages(void) const;
		virtual int getScoreModifier(void) const;
		virtual bool getPassthrough(void) const;

	private :

		CollisionBox         _cb;
		ICollidable::Damages _dmg;
		bool                 _passthrough;
		int                  _score_modifier;
};

#endif