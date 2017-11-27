/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROP_HPP
# define PROP_HPP

# include "IEntity.hpp"
# include "oGL_module.hpp"
# include "Model.hpp"
# include "Shader.hpp"
# include "GeneralException.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>

class Prop : public IEntity
{
	public :

		Prop(Shader const *shader, glm::mat4 const *perspec_mult_view,
			 Model const *model, glm::vec3 const &pos, glm::vec3 const &orientation,
			 glm::vec3 const &scale, glm::vec3 const &offset);
		virtual ~Prop(void);
		Prop(Prop const &src) = delete;
		Prop &operator=(Prop const &rhs) = delete;

		void setPosition(glm::vec3 const &pos);
		void setScale(glm::vec3 const &scale);
		void setYaw(GLfloat yaw);
		void setPitch(GLfloat pitch);
		void setRoll(GLfloat roll);
		void setOffset(glm::vec3 const &offset);

		glm::mat4 const &getTotalMatrix(void) const;

		/*
		 * Interface IEntity
		 */
		void update(float time);
		void draw(void);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	protected :

		Shader const    *_shader;
		glm::mat4 const *_perspec_mult_view;
		Model const     *_model;
		GLfloat         _yaw;
		GLfloat         _pitch;
		GLfloat         _roll;
		glm::vec3       _pos;
		glm::vec3       _scale;
		glm::mat4       _total;
		glm::vec3       _offset;
};

#endif