/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FontSet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FONTSET_HPP
# define FONTSET_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "ft2build.h"
# include "oGL_module.hpp"
# include "Texture.hpp"
# include "Shader.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include <iostream>
# include <map>
# include <utility>

# include FT_FREETYPE_H

class FontSet
{
	public:

		struct FontChar
		{
			Texture    tex;
			glm::ivec2 size;
			glm::ivec2 bearing;
			GLuint     advance;
		};

		FontSet(Shader const *shader, glm::mat4 const *proj_mat,
				std::string const &name, std::string const &font_path);
		virtual ~FontSet(void);
		FontSet(FontSet const &src) = delete;
		FontSet &operator=(FontSet const &rhs) = delete;
		FontSet(FontSet &&src);
		FontSet &operator=(FontSet &&rhs);

		Shader const *getShader(void) const;
		std::string const &getName(void) const;
		glm::mat4 const *getProjectionMatrix(void) const;
		std::map<GLchar, FontChar> moveCharList(void);
		GLuint moveVAO(void);
		GLuint moveVBO(void);
		void setProjectionMatrix(glm::mat4 const *matrix);
		void drawText(std::string const &str, glm::vec3 const &color,
					  glm::vec3 const &pos_scale) const;

		class FontSetInitException : public GeneralException
		{
			public :

				explicit FontSetInitException(void);
				virtual        ~FontSetInitException(void) throw();
		};

		class FreeTypeInitException : public GeneralException
		{
			public :

				explicit FreeTypeInitException(void);
				virtual        ~FreeTypeInitException(void) throw();
		};

		class FontLoadingException : public GeneralException
		{
			public :

				explicit FontLoadingException(void);
				virtual        ~FontLoadingException(void) throw();
		};

		class GlyphLoadingException : public GeneralException
		{
			public :

				explicit GlyphLoadingException(void);
				virtual        ~GlyphLoadingException(void) throw();
		};

	private :

		Shader const               *_shader;
		glm::mat4 const            *_proj_matrix;
		std::string                _name;
		std::map<GLchar, FontChar> _char_list;
		GLuint                     _vao;
		GLuint                     _vbo;

		void _load_char_list(std::string const &path);
};

#endif
