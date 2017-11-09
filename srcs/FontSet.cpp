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
		this->_load_char_list(font_path);
		this->_vao = oGL_module::oGL_create_vao();
		this->_vbo = oGL_module::oGL_create_dynamic_vbo(sizeof(GLfloat) * 6 * 4, NULL);
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
}

Shader const *FontSet::getShader(void) const
{
	return (this->_shader);
}

glm::mat4 const &FontSet::getProjectionMatrix(void) const
{
	return (this->_proj_matrix);
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
}