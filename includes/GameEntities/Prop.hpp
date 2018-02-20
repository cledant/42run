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

# include "Interfaces/IEntity.hpp"
# include "Interfaces/ITranslatable.hpp"
# include "OpenGL/oGL_module.hpp"
# include "Model/Model.hpp"
# include "OpenGL/Shader.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>

class Prop : public IEntity, public ITranslatable
{
	public :


		struct Params
		{
			Params(void);
			~Params(void);

			Shader const    *shader;
			glm::mat4 const *perspec_mult_view;
			Model const     *model;
			glm::vec3       pos;
			glm::vec3       orientation;
			glm::vec3       scale;
			glm::vec3       offset;
		};

		Prop(Prop::Params const &params);
		virtual ~Prop(void);
		Prop(Prop const &src);
		Prop &operator=(Prop const &rhs);

		/*
		 * Setter
		 */

		void setPosition(glm::vec3 const &pos);
		void setScale(glm::vec3 const &scale);
		void setYaw(GLfloat yaw);
		void setPitch(GLfloat pitch);
		void setRoll(GLfloat roll);
		void setOffset(glm::vec3 const &offset);

		/*
		 * Getter
		 */

		glm::mat4 const &getTotalMatrix(void) const;
		GLfloat getYaw(void) const;
		GLfloat getPitch(void) const;
		GLfloat getRoll(void) const;
		Shader const *getShader(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		Model const *getModel(void) const;
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getScale(void) const;
		glm::vec3 const &getOffset(void) const;

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

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	private :

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