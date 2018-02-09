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

# include "OpenGL/oGL_module.hpp"
# include "Cubemap.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include "Interfaces/IEntity.hpp"
# include "Interfaces/ICollidable.hpp"
# include "Interfaces/ITranslatable.hpp"

class CollidableBox : public IEntity, public ICollidable, public ITranslatable
{
	public :

		struct Params
		{
			Params(void);
			~Params(void);

			Shader const         *shader;
			glm::mat4 const      *perspec_mult_view;
			Model const          *model;
			glm::vec3            pos;
			glm::vec3            size;
			ICollidable::Damages dmg;
			bool                 passthrough;
			int                  score_modifier;
			bool                 active;
			std::string          pick_up;
		};

		CollidableBox(void);
		CollidableBox(CollidableBox::Params const &params);
		virtual ~CollidableBox();
		CollidableBox(CollidableBox const &src);
		CollidableBox &operator=(CollidableBox const &rhs);

		/*
		 * Interface ITranslatable
		 */

		void translateObject(glm::vec3 const &vec);
		void scaleObject(glm::vec3 const &vec);

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

		/*
 		* Getter
		*/
		glm::vec3 const &getPos(void) const;
		Cubemap const &getCubemap(void) const;

	private :

		Cubemap              _cm;
		CollisionBox         _cb;
		glm::vec3            _pos;
		ICollidable::Damages _dmg;
		bool                 _passthrough;
		int                  _score_modifier;
		bool                 _active;
		std::string          _pick_up;
};

#endif
