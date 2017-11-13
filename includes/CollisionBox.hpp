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

#include "IEntity.hpp"
#include "Cubemap.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <memory>

class CollisionBox : public IEntity
{
	public :

		CollisionBox(Shader const *shader, glm::mat4 const *perspec_mult_view,
					 glm::vec3 const &pos, glm::vec3 const &min, glm::vec3 const &max,
					 std::string const &name);
		CollisionBox(CollisionBox const &src);
		CollisionBox &operator=(CollisionBox const &rhs);
		virtual ~CollisionBox(void);

		void update(float time);
		void draw(void);
//		void setPos(glm::vec3 const &pos);
		glm::vec3 const &getMin(void) const;
		glm::vec3 const &getMax(void) const;
		glm::vec3 const &getPos(void) const;
		std::string const &getName(void) const;
		std::shared_ptr<Cubemap> const &getCubemap(void) const;
		bool checkCollision(CollisionBox const &box) const;

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	private :

		std::shared_ptr<Cubemap> _debug_draw;
		std::string              _debug_name;
		glm::vec3                _min;
		glm::vec3                _max;
		glm::vec3                _pos;
};

#endif