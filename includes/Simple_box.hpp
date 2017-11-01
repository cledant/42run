/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_box.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:31:40 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMPLE_BOX_HPP
# define SIMPLE_BOX_HPP

#include "IEntity.hpp"
#include "oGL_module.hpp"
#include "Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Simple_box : public IEntity
{
	public :

		Simple_box(Shader const *shader, glm::mat4 const *perspec_mult_view,
			glm::vec3 const &pos, glm::vec3 const &scale);
		virtual ~Simple_box(void);

		void				update(float time);
		void				draw(void);
		void				setPosition(glm::vec3 const &pos);
		void				setScale(glm::vec3 const &scale);
		glm::mat4 const		&getTotalMatrix(void) const;

	class InitException : public GeneralException
	{
		public :

			explicit	InitException(void);
			virtual		~InitException(void) throw();
	};

	private :

		Shader const		*_shader;
		glm::mat4 const		*_perspec_mult_view;
		GLuint				_vbo;
		GLuint				_vao;
		glm::vec3			_pos;
		glm::vec3			_scale;
		glm::mat4			_total;

		static float		_vertices[];
		static size_t		_nb_faces;

		Simple_box(Simple_box const &src);
		Simple_box	&operator=(Simple_box const &rhs);
};

#endif
