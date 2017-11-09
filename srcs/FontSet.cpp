/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FontSet.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FontSet.hpp"

FontSet::FontSet(Shader const *shader, std::string const &font_path, int win_width,
				 int win_height) :
		_shader(shader), _vao(0), _vbo(0)
{
	try
	{
		this->_vao = oGL_module::oGL_create_vao();
		this->_vbo = oGL_module::oGL_create_dynamic_vbo(sizeof(GLfloat) * 6 * 4, NULL);
		this->_load_char_list(font_path);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw FontSet::FontSetInitException();
	}
	this->_proj_matrix = glm::ortho(0.0f, static_cast<float>(win_width), 0.0f,
									static_cast<float>(win_height));
}

FontSet::~FontSet(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

FontSet::FontSet(FontSet &&src)
{
	this->_shader      = src.getShader();
	this->_char_list   = src.moveCharList();
	this->_proj_matrix = src.getProjectionMatrix();
	this->_vao         = src.moveVAO();
	this->_vbo         = src.moveVBO();
}

FontSet &FontSet::operator=(FontSet &&rhs)
{
	this->_shader      = rhs.getShader();
	this->_char_list   = rhs.moveCharList();
	this->_proj_matrix = rhs.getProjectionMatrix();
	this->_vao         = rhs.moveVAO();
	this->_vbo         = rhs.moveVBO();
	return (*this);
}

Shader const *FontSet::getShader(void) const
{
	return (this->_shader);
}

glm::mat4 const &FontSet::getProjectionMatrix(void) const
{
	return (this->_proj_matrix);
}

std::map<GLchar, FontSet::FontChar> FontSet::moveCharList(void)
{
	return (std::move(this->_char_list));
}

GLuint FontSet::moveVAO(void)
{
	GLuint tmp = this->_vao;

	this->_vao = 0;
	return (tmp);
}

GLuint FontSet::moveVBO(void)
{
	GLuint tmp = this->_vbo;

	this->_vbo = 0;
	return (tmp);
}

void FontSet::setProjectionMatrix(glm::mat4 const &matrix)
{
	this->_proj_matrix = matrix;
}

void FontSet::_load_char_list(std::string const &path)
{
	FT_Library  lib;
	FT_Face     face;
	FontChar    fchar;
	std::string tex_name;

	if (FT_Init_FreeType(&lib) != 0)
		throw FontSet::FreeTypeInitException();
	if (FT_New_Face(lib, path.c_str(), 0, &face) != 0)
	{
		FT_Done_FreeType(lib);
		throw FontSet::FontLoadingException();
	}
	FT_Set_Pixel_Sizes(face, 0, 48);
	oGL_module::oGL_disable_texture_alignment();
	for (GLubyte i = 0; i < 128; i++)
	{
		if (FT_Load_Glyph(face, i, FT_LOAD_RENDER) != 0)
		{
			FT_Done_Face(face);
			FT_Done_FreeType(lib);
			throw FontSet::GlyphLoadingException();
		}
		try
		{
			tex_name.clear();
			tex_name = "font_char_" + std::to_string(i);
			fchar    = {Texture(tex_name, face->glyph->bitmap.buffer,
								glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
								Texture::TEX_GLYPH),
						glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
						glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
						static_cast<GLuint>(face->glyph->advance.x)};
			this->_char_list.insert(std::pair<GLchar, FontChar>(i, std::move(fchar)));
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			FT_Done_Face(face);
			FT_Done_FreeType(lib);
			throw FontSet::GlyphLoadingException();
		}
	}
	FT_Done_Face(face);
	FT_Done_FreeType(lib);
}

FontSet::FontSetInitException::FontSetInitException(void)
{
	this->_msg = "FontSet : Initialization failed";
}

FontSet::FontSetInitException::~FontSetInitException(void) throw()
{
}

FontSet::FreeTypeInitException::FreeTypeInitException(void)
{
	this->_msg = "FontSet : FreeType initialization failed";
}

FontSet::FreeTypeInitException::~FreeTypeInitException(void) throw()
{
}

FontSet::FontLoadingException::FontLoadingException(void)
{
	this->_msg = "FontSet : Font loading failed";
}

FontSet::FontLoadingException::~FontLoadingException(void) throw()
{
}

FontSet::GlyphLoadingException::GlyphLoadingException(void)
{
	this->_msg = "FontSet : Font loading failed";
}

FontSet::GlyphLoadingException::~GlyphLoadingException(void) throw()
{
}