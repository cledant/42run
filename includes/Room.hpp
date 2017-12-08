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

# include "ICollidable.hpp"
# include "IEntity.hpp"
# include "CollidableBox.hpp"
# include "CollidableProp.hpp"

class Room : public ICollidable, public IEntity
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
		Room(Room const &src) const = delete;
		Room &operator=(Room const &rhs) const = delete;
		Room(Room &&src);
		Room &operator=(Room &&rhs);

	private :

		CollisionBox  _room_cb;
		CollidableBox _floor;
		CollidableBox _roof;
		CollidableBox _right_wall;
		CollidableBox _left_wall;
		CollidableBox _front_wall;
};

#endif
