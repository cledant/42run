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
# include "GameEntities/CollidableBox.hpp"
# include "GameEntities/CollidableProp.hpp"

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
			glm::mat4 const       *perspec_mult_view;
		};

		Room(void);
		Room(Room::Params const &params);
		virtual ~Room(void);
		Room(Room const &src);
		Room &operator=(Room const &rhs);

		void addBonus(std::string const &slot, CollidableProp::Params &params);
		void addObstacle(std::string const &slot, CollidableProp::Params &params);

		/*
		 * Interface ITranslatable
		 */

		// Translate also Bonuses/Obstacles in the room
		void translateObject(glm::vec3 const &vec);

		// Does not scale Bonuses/Obstacles in the room
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

		CollidableBox const &getFloor(void) const;
		CollidableBox const &getRoof(void) const;
		CollidableBox const &getRightWall(void) const;
		CollidableBox const &getLeftWall(void) const;
		CollidableBox const &getFrontWall(void) const;
		CollidableProp &getBonus(std::string const &name);
		CollidableProp &getObstacle(std::string const &name);
		glm::mat4 const *getPerspecMultView(void) const;
		std::map<std::string, CollidableProp> const &getBonusList(void) const;
		std::map<std::string, CollidableProp> const &getObstacleList(void) const;

		/*
		 * Exceptions
		 */

		class BonusNotFoundException : public GeneralException
		{
			public :

				explicit BonusNotFoundException(void);
				virtual ~BonusNotFoundException(void) throw();
		};

		class ObstacleNotFoundException : public GeneralException
		{
			public :

				explicit ObstacleNotFoundException(void);
				virtual ~ObstacleNotFoundException(void) throw();
		};

	private :

		glm::mat4 const                       *_perspec_mult_view;
		CollisionBox                          _room_cb;
		CollidableBox                         _floor;
		CollidableBox                         _roof;
		CollidableBox                         _right_wall;
		CollidableBox                         _left_wall;
		CollidableBox                         _front_wall;
		std::string                           _pick_up;
		std::map<std::string, CollidableProp> _list_bonuses;
		std::map<std::string, CollidableProp> _list_obstacles;

		static Room::Params basic_params;
};

#endif
