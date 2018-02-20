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
# include "Utility/CollisionBox.hpp"
# include "Interfaces/ICollidable.hpp"
# include "Interfaces/IEntity.hpp"
# include "Interfaces/ITranslatable.hpp"

class CollidableProp : public ICollidable, public IEntity, public ITranslatable
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
			std::string          pick_up;
			bool                 auto_rotation;
		};

		CollidableProp(CollidableProp::Params const &params);
		virtual ~CollidableProp(void);
		CollidableProp(CollidableProp const &src);
		CollidableProp &operator=(CollidableProp const &rhs);

		CollisionBox const &getCollisionBox(void) const;
		Damages getDamages(void) const;
		int getScoreModifier(void) const;
		bool getPassthrough(void) const;
		bool getActive(void) const;
		std::string const &getPickUpSound(void) const;

		/*
		 * Interface ICollidable
		 */

		void setPassthrough(bool value);
		void setActive(bool value);

		/*
		 * Interface IEntity
		 */
		void update(float time);
		void draw(void);

		/*
		 * Interface ITranslatable
		 */

		void translateObject(glm::vec3 const &vec);
		void scaleObject(glm::vec3 const &vec);

		/*
		 * Getter
		 */

		Prop const &getProp(void) const;
		bool getAutoRotation(void) const;

	private :

		Prop                 _prop;
		CollisionBox         _cb;
		ICollidable::Damages _dmg;
		bool                 _passthrough;
		int                  _score_modifier;
		bool                 _active;
		std::string          _pick_up;
		bool                 _auto_rotation;
};

#endif