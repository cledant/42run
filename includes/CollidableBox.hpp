/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollidableBox.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/11/15 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLIDABLEBOX_HPP
# define COLLIDABLEBOX_HPP

# include "oGL_module.hpp"
# include "Cubemap.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "IEntity.hpp"
# include "ICollidable.hpp"

class CollidableBox : public IEntity, public ICollidable
{
	public :

		CollidableBox(Shader const *shader, glm::mat4 const *perspec_mult_view,
					  glm::vec3 const &pos, glm::vec3 const &size, Texture const *tex,
					  ICollidable::Damages dmg, bool passthrough, int score_modifier);
		virtual ~CollidableBox();
		CollidableBox(const CollidableBox &src) = delete;
		CollidableBox &operator=(const CollidableBox &rhs) = delete;

		glm::vec3 const &getPos(void) const;

		/*
		 * Interface ICollidable
		 */
		virtual void setPassthrough(bool value);

		CollisionBox const &getCollisionBox(void) const;
		ICollidable::Damages getDamages(void) const;
		virtual int getScoreModifier(void) const;
		virtual bool getPassthrough(void) const;

		/*
		 * Interface IEntity
		 */
		void update(float time);
		void draw(void);

	private :

		Cubemap              _model;
		CollisionBox         _cb;
		glm::vec3            _pos;
		ICollidable::Damages _dmg;
		bool                 _passthrough;
		int                  _score_modifier;
};

#endif
