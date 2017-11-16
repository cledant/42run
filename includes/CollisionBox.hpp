/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollisionBox.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 16:07:59 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:43:03 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISIONBOX_HPP
# define COLLISIONBOX_HPP

#include "GeneralException.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <memory>

class CollisionBox
{
	public :

		CollisionBox(glm::vec3 const &pos, glm::vec3 const &half_size);
		CollisionBox(CollisionBox const &src);
		CollisionBox &operator=(CollisionBox const &rhs);
		virtual ~CollisionBox(void);

		void setPos(glm::vec3 const &pos);
		void setHalfSize(glm::vec3 const &pos);
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getHalfSize(void) const;

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	private :

		glm::vec3 _pos;
		glm::vec3 _half_size;
};

#endif