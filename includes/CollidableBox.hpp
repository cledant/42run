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

		struct Params
		{
			Params(void);
			~Params(void);

			Shader const         *shader;
			glm::mat4 const      *perspec_mult_view;
			Texture const        *tex;
			glm::vec3            pos;
			glm::vec3            size;
			ICollidable::Damages dmg;
			bool                 passthrough;
			int                  score_modifier;
			bool                 active;
			std::string          pick_up;
		};

		CollidableBox(CollidableBox::Params const &params);
		virtual ~CollidableBox();
		CollidableBox(const CollidableBox &src) = delete;
		CollidableBox &operator=(const CollidableBox &rhs) = delete;

		glm::vec3 const &getPos(void) const;

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
		std::string const &getPickUpSound(void) const;

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
		bool                 _active;
		std::string          _pick_up;
};

#endif
