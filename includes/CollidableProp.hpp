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
					   glm::vec3 const &angles, glm::vec3 const &scale,
					   CollisionBox cb, ICollidable::Damages dmg);
		virtual ~CollidableProp(void);
		CollidableProp(CollidableProp const &src) = delete;
		CollidableProp &operator=(CollidableProp const &rhs) = delete;

		/*
		 * Interface ICollidable
		 */
		CollisionBox const &getCollisionBox(void) const;
		ICollidable::Damages getDamages(void) const;

	private :

		CollisionBox         _cb;
		ICollidable::Damages _dmg;
};

#endif