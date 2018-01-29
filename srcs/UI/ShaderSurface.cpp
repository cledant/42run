/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderSurface.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UI/ShaderSurface.hpp"

ShaderSurface::ShaderSurface(GLFW_Window const *win, Input const *input,
							 Shader const *shader) : _win(win), _input(input),
													 _shader(shader), _vao(0), _vbo(0)
{
	try
	{
		this->_vbo = oGL_module::oGL_create_vbo(sizeof(float) * 3 * 6,
												&(ShaderSurface::_vertices[0]));
		this->_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3,
										   sizeof(GLfloat) * 3, 0);
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw ShaderSurface::InitException();
	}
	this->update(0.0f);
}

ShaderSurface::~ShaderSurface()
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

ShaderSurface::ShaderSurface(ShaderSurface &&src)
{
	*this = std::move(src);
}

ShaderSurface &ShaderSurface::operator=(ShaderSurface &&rhs)
{
	this->_win    = rhs.getWindow();
	this->_input  = rhs.getInput();
	this->_shader = rhs.getShader();
	this->_vao    = rhs.moveVAO();
	this->_vbo    = rhs.moveVBO();
	return (*this);
}

Shader const *ShaderSurface::getShader(void) const
{
	return (this->_shader);
}

Input const *ShaderSurface::getInput(void) const
{
	return (this->_input);
}

GLFW_Window const *ShaderSurface::getWindow(void) const
{
	return (this->_win);
}

GLuint ShaderSurface::moveVAO(void)
{
	GLuint tmp = this->_vao;

	this->_vao = 0;
	return (tmp);
}

GLuint ShaderSurface::moveVBO(void)
{
	GLuint tmp = this->_vbo;

	this->_vbo = 0;
	return (tmp);
}

void ShaderSurface::update(float time)
{
	static_cast<void>(time);
}

void ShaderSurface::draw(void)
{
	GLint id_time;
	GLint id_resolution;

	if (this->_shader == nullptr || this->_win == nullptr || this->_input == nullptr ||
		!oGL_module::oGL_getUniformID("uniform_time",
									  this->_shader->getShaderProgram(),
									  &id_time) ||
		!oGL_module::oGL_getUniformID("uniform_resolution",
									  this->_shader->getShaderProgram(),
									  &id_resolution))
	{
		std::cout << "Warning : Can't draw ShaderSurface" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setVec2(id_resolution, glm::vec2{this->_win->cur_win_w,
													this->_win->cur_win_h});
	this->_shader->setFloat(id_time, Glfw_manager::getTime());
	oGL_module::oGL_draw_filled(this->_vao, ShaderSurface::_nb_faces);
}

ShaderSurface::InitException::InitException(void)
{
	this->_msg = "Simple_Box : Object initialization failed";
}

ShaderSurface::InitException::~InitException(void) throw()
{
}

float ShaderSurface::_vertices[] = {-1.0f, 1.0f, 0.5f,
									1.0f, 1.0f, 0.5f,
									-1.0f, -1.0f, 0.5f,
									-1.0f, -1.0f, 0.5f,
									1.0f, 1.0f, 0.5f,
									1.0f, -1.0f, 0.5f};

size_t ShaderSurface::_nb_faces = 6;