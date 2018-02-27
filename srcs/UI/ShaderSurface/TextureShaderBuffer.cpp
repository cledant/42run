/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureShaderSurface.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UI/ShaderSurface/TextureShaderSurface.hpp"

TextureShaderSurface::TextureShaderSurface(void) :
		ShaderSurface(), _tex_id(0)
{
}

TextureShaderSurface::TextureShaderSurface(GLFW_Window const *win, Input const *input,
										   Shader const *shader, GLuint tex_id) :
		ShaderSurface(), _tex_id(tex_id)
{
	this->_win    = win;
	this->_input  = input;
	this->_shader = shader;
	try
	{
		this->_allocate_tex_buffer();
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw ShaderSurface::InitException();
	}
	this->update(0.0f);
}

TextureShaderSurface::~TextureShaderSurface(void)
{
}

TextureShaderSurface::TextureShaderSurface(TextureShaderSurface &&src)
{
	*this = std::move(src);
}

TextureShaderSurface &TextureShaderSurface::operator=(TextureShaderSurface &&rhs)
{
	ShaderSurface::operator=(std::move(rhs));
	this->_tex_id = rhs.getTextureID();
	return (*this);
}

/*
 * Getter
 */

GLuint TextureShaderSurface::getTextureID(void) const
{
	return (this->_tex_id);
}

/*
 * Setter
 */

void TextureShaderSurface::setTextureID(GLuint id)
{
	this->_tex_id = id;
}

/*
 * Inherited Interface IEntity
 */

void TextureShaderSurface::update(float time)
{
	static_cast<void>(time);
}

void TextureShaderSurface::draw(void)
{
	GLint id_uniform_tex;

	if (this->_shader == nullptr || this->_win == nullptr || this->_input == nullptr ||
		!oGL_module::oGL_getUniformID("uniform_tex",
									  this->_shader->getShaderProgram(),
									  &id_uniform_tex))
	{
		std::cout << "Warning : Can't draw TextureShaderSurface" << std::endl;
		return;
	}
	this->_shader->use();
	oGL_module::oGL_set_texture(id_uniform_tex, 0, this->_tex_id);
	oGL_module::oGL_draw_filled(this->_vao, TextureShaderSurface::_nb_faces);
}

void TextureShaderSurface::_allocate_tex_buffer(void)
{
	this->_vbo = oGL_module::oGL_create_vbo(sizeof(float) * 5 * 6,
											&(TextureShaderSurface::_tex_vertices[0]));
	this->_vao = oGL_module::oGL_create_vao();
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3,
									   sizeof(GLfloat) * 5, 0);
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 1, 2,
									   sizeof(GLfloat) * 5, sizeof(GLfloat) * 3);
}

float TextureShaderSurface::_tex_vertices[] = {-1.0f, 1.0f, 0.5f, 0.0f, 1.0f,
											   1.0f, 1.0f, 0.5f, 1.0f, 1.0f,
											   -1.0f, -1.0f, 0.5f, 0.0f, 0.0f,
											   -1.0f, -1.0f, 0.5f, 0.0f, 0.0f,
											   1.0f, 1.0f, 0.5f, 1.0f, 1.0f,
											   1.0f, -1.0f, 0.5f, 1.0f, 0.0f};

size_t TextureShaderSurface::_tex_nb_faces = 6;