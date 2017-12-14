/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cubemap.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 16:07:59 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:43:03 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBEMAP_HPP
# define CUBEMAP_HPP

#include "IEntity.hpp"
#include "oGL_module.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ITranslatable.hpp"

class Cubemap : public IEntity, public ITranslatable
{
	public :

		typedef enum e_tex_source
		{
			EXTERNAL,
			INTERNAL,
		} t_tex_source;

		Cubemap(Shader const *shader, glm::mat4 const *perspec_mult_view,
				std::vector<std::string> const &files, glm::vec3 const &pos,
				glm::vec3 const &scale);
		Cubemap(Shader const *shader, glm::mat4 const *perspec_mult_view,
				Texture const *tex, glm::vec3 const &pos,
				glm::vec3 const &scale);
		virtual ~Cubemap(void);
		Cubemap(Cubemap const &src) = delete;
		Cubemap &operator=(Cubemap const &rhs) = delete;
		Cubemap(Cubemap &&src);
		Cubemap &operator=(Cubemap &&rhs);

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
		 * Setter
		 */
		void setPosition(glm::vec3 const &pos);
		void setScale(glm::vec3 const &scale);

		/*
		 * Getter
		 */
		glm::mat4 const &getTotalMatrix(void) const;
		Shader const *getShader(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		Texture const *moveTexture(void);
		GLuint moveVAO(void);
		GLuint moveVBO(void);
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getScale(void) const;
		t_tex_source getSrcTex(void) const;

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
		GLuint          _vbo;
		GLuint          _vao;
		glm::vec3       _pos;
		glm::vec3       _scale;
		glm::mat4       _total;
		t_tex_source    _src_tex;

		static float  _vertices[];
		static size_t _nb_faces;

		/*
		 * Private
		 */
		void _oGL_alloc(void);
};

#endif
