/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Room.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_HPP
# define ROOM_HPP

# include "Interfaces/ICollidable.hpp"
# include "Interfaces/IEntity.hpp"
# include "Interfaces/ITranslatable.hpp"
# include "GameEntity/CollidableBox.hpp"
# include "GameEntity/CollidableProp.hpp"

class Room : public ICollidable, public IEntity, public ITranslatable
{
	public :

		struct Params
		{
			Params(void);
			~Params(void);

			CollisionBox          room_cb;
			CollidableBox::Params floor;
			CollidableBox::Params roof;
			CollidableBox::Params right_wall;
			CollidableBox::Params left_wall;
			CollidableBox::Params front_wall;
		};

		Room(Room::Params const &params);
		virtual ~Room(void);
		Room(Room const &src) = delete;
		Room &operator=(Room const &rhs) = delete;
		Room(Room &&src);
		Room &operator=(Room &&rhs);

		/*
		 * Interface ITranslatable
		 */

		void translateObject(glm::vec3 const &vec);
		void scaleObject(glm::vec3 const &vec);

		/*
		 * Interface IEntity
		 */

		void update(float time);
		void draw(void);

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
		 * Setter
		 */

		void activeFrontWall(bool value);

		/*
		 * Getter
		 */
		CollidableBox &getFloor(void);
		CollidableBox &getRoof(void);
		CollidableBox &getRightWall(void);
		CollidableBox &getLeftWall(void);
		CollidableBox &getFrontWall(void);

	private :

		CollisionBox  _room_cb;
		CollidableBox _floor;
		CollidableBox _roof;
		CollidableBox _right_wall;
		CollidableBox _left_wall;
		CollidableBox _front_wall;
		std::string   _pick_up;
};

#endif
