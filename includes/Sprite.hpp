/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sprite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_HPP
# define SPRITE_HPP

# include "IEntity.hpp"
# include "oGL_module.hpp"
# include "Model.hpp"
# include "Shader.hpp"
# include "GeneralException.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>

class Sprite : public IEntity
{
	public :

		Sprite(Shader const *shader, glm::mat4 const *perspec_mult_view,
			   Texture const *tex, glm::vec3 const &pos, glm::vec3 const &scale,
			   size_t sprites_per_lines, size_t lines_of_sprites);
		virtual ~Sprite(void);
		Sprite(Sprite const &src) = delete;
		Sprite &operator=(Sprite const &rhs) = delete;
		Sprite(Sprite &&src);
		Sprite &operator=(Sprite const &&rhs);

		void setPosition(glm::vec3 const &pos);
		void setScale(glm::vec3 const &scale);
		void setYaw(float yaw);

		glm::mat4 const &getTotalMatrix(void) const;
		GLuint moveVAO(void);
		GLuint moveVBO(void);

		/*
		 * IEntity Interface
		 */
		void update(float time);
		void draw(void);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	private :

		Shader const    *_shader;
		glm::mat4 const *_perspec_mult_view;
		Texture const   *_tex;
		size_t          _sprites_per_line;
		size_t          _lines_of_sprites;
		GLuint          _vao;
		GLuint          _vbo;
		glm::vec3       _pos;
		glm::vec3       _scale;
		glm::mat4       _total;
		float 			_yaw;

		void _allocate_set_GL_ressources(void);

		static float  _vertices[];
		static size_t _nb_faces;
};

#endif
