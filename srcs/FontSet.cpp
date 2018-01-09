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

#include "UI/FontSet.hpp"

FontSet::FontSet(Shader const *shader, glm::mat4 const *proj_mat,
				 std::string const &name, std::string const &font_path, size_t size) :
		_shader(shader), _proj_matrix(proj_mat), _name(name), _vao(0), _vbo(0)
{
	std::cout << "Loading : " << font_path << std::endl;
	try
	{
		this->_vao = oGL_module::oGL_create_vao();
		this->_vbo = oGL_module::oGL_create_dynamic_vbo(sizeof(GLfloat) * 6 * 4, NULL);
		this->_load_char_list(font_path, size);
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 4,
										   sizeof(GLfloat) * 4, 0);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw FontSet::FontSetInitException();
	}
}

FontSet::~FontSet(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

FontSet::FontSet(FontSet &&src)
{
	this->_shader      = src.getShader();
	this->_name        = src.getName();
	this->_char_list   = src.moveCharList();
	this->_proj_matrix = src.getProjectionMatrix();
	this->_vao         = src.moveVAO();
	this->_vbo         = src.moveVBO();
}

FontSet &FontSet::operator=(FontSet &&rhs)
{
	this->_shader      = rhs.getShader();
	this->_name        = rhs.getName();
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

std::string const &FontSet::getName(void) const
{
	return (this->_name);
}

glm::mat4 const *FontSet::getProjectionMatrix(void) const
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

void FontSet::setProjectionMatrix(glm::mat4 const *matrix)
{
	this->_proj_matrix = matrix;
}

void FontSet::_load_char_list(std::string const &path, size_t size)
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
	FT_Set_Pixel_Sizes(face, 0, size);
	oGL_module::oGL_disable_texture_alignment();
	for (GLubyte i = 0; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER) != 0)
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

void FontSet::drawText(std::string const &str, glm::vec3 const &color,
					   glm::vec3 const &pos_scale) const
{
	std::string::const_iterator                it;
	std::map<GLchar, FontChar>::const_iterator fchar;
	GLfloat                                    pos_x = pos_scale.x;
	GLfloat                                    pos_y = pos_scale.y;
	GLint                                      uniform_color;
	GLint                                      uniform_mat_proj;
	GLint                                      uniform_tex;

	if (this->_shader == nullptr || this->_proj_matrix == nullptr ||
		!oGL_module::oGL_getUniformID("uniform_color", this->_shader->getShaderProgram(),
									  &uniform_color) ||
		!oGL_module::oGL_getUniformID("uniform_mat_proj", this->_shader->getShaderProgram(),
									  &uniform_mat_proj) ||
		!oGL_module::oGL_getUniformID("uniform_tex", this->_shader->getShaderProgram(),
									  &uniform_tex))
	{
		std::cout << "Warning : Can't draw text" << std::endl;
		return;
	}
	oGL_module::oGL_enable_blend();
	for (it = str.begin(); it != str.end(); ++it)
	{
		if ((fchar = this->_char_list.find(*it)) == this->_char_list.end())
			fchar = this->_char_list.find('?');

		GLfloat xpos           = pos_x + fchar->second.bearing.x * pos_scale.z;
		GLfloat ypos           = pos_y - (fchar->second.size.y - fchar->second.bearing.y) *
										 pos_scale.z;
		GLfloat w              = fchar->second.size.x * pos_scale.z;
		GLfloat h              = fchar->second.size.y * pos_scale.z;
		GLfloat vertices[6][4] =
						{
								{xpos,     ypos + h, 0.0, 0.0},
								{xpos,     ypos,     0.0, 1.0},
								{xpos + w, ypos,     1.0, 1.0},

								{xpos,     ypos + h, 0.0, 0.0},
								{xpos + w, ypos,     1.0, 1.0},
								{xpos + w, ypos + h, 1.0, 0.0}
						};
		this->_shader->use();
		this->_shader->setMat4(uniform_mat_proj, *(this->_proj_matrix));
		this->_shader->setVec3(uniform_color, color);
		oGL_module::oGL_set_texture(uniform_tex, 0, fchar->second.tex.getTextureID());
		oGL_module::oGL_set_dynamic_vbo_data(this->_vao, this->_vbo,
											 sizeof(GLfloat) * 6 * 4, vertices);
		oGL_module::oGL_draw_filled(this->_vao, 6);
		pos_x += (fchar->second.advance >> 6) * pos_scale.z;

	}
	oGL_module::oGL_disable_blend();
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
	this->_msg = "FontSet : Glyph loading failed";
}

FontSet::GlyphLoadingException::~GlyphLoadingException(void) throw()
{
}