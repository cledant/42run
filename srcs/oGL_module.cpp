/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/30 13:58:09 by cledant           #+#    #+#             */
/*   Updated: 2017/09/19 14:33:44 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "oGL_module.hpp"

oGL_module::oGL_module(void)
{
}

oGL_module::~oGL_module(void)
{
}

void oGL_module::oGL_check_error(void)
{
	if (glGetError() != GL_NO_ERROR)
		throw oGL_module::oGLFailException();
}

void oGL_module::oGL_display_error(void)
{
	GLuint error;

	error = glGetError();
	switch (error)
	{
		case GL_NO_ERROR :
			std::cout << "No error" << std::endl;
			break;
		case GL_INVALID_VALUE :
			std::cout << "Invalid value" << std::endl;
			break;
		case GL_INVALID_OPERATION :
			std::cout << "Invalid operation" << std::endl;
			break;
		default :
			std::cout << "Other error" << std::endl;
			break;
	}
}

GLuint oGL_module::oGL_create_vbo(size_t size, void *data)
{
	GLuint new_vbo;

	glGenBuffers(1, &new_vbo);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data,
				 GL_STATIC_DRAW);
	oGL_module::oGL_check_error();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (new_vbo);
}

void oGL_module::oGL_delete_vbo(GLuint vbo)
{
	glDeleteBuffers(1, &vbo);
}

GLuint oGL_module::oGL_create_vao(void)
{
	GLuint new_vao;

	glGenVertexArrays(1, &new_vao);
	oGL_module::oGL_check_error();
	return (new_vao);
}

void oGL_module::oGL_set_vao_parameters(GLuint vbo, GLuint vao,
										GLuint index, GLint size, GLsizei stride,
										size_t shift)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride,
						  reinterpret_cast<void *>(shift));
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void oGL_module::oGL_delete_vao(GLuint vao)
{
	glDeleteVertexArrays(1, &vao);
}

void oGL_module::oGL_clear_buffer(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void oGL_module::oGL_update_framebuffer(int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

void oGL_module::oGL_enable_depth(void)
{
	glEnable(GL_DEPTH_TEST);
}

bool oGL_module::oGL_getUniformID(std::string const &name,
								  GLuint prog, GLint *uniform_id)
{
	if (uniform_id == nullptr)
		return (false);
	if ((*uniform_id = glGetUniformLocation(prog, name.c_str())) == -1)
	{
		oGL_module::oGL_display_error();
		return (false);
	}
	return (true);
}

void oGL_module::oGL_draw_filled(GLuint vao, size_t nb_faces)
{
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, nb_faces);
	glBindVertexArray(0);
}

void oGL_module::oGL_draw_points(GLuint vao, size_t nb_faces)
{
	glBindVertexArray(vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glDrawArrays(GL_POINTS, 0, nb_faces);
	glBindVertexArray(0);
}

void oGL_module::oGL_draw_cubemap(GLuint vao, GLuint tex,
								  size_t nb_faces)
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glDepthFunc(GL_LEQUAL);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, nb_faces);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void oGL_module::oGL_finish(void)
{
	glFinish();
}

void oGL_module::add_shader(std::string const &name,
							std::string const &vs_path, std::string const &fs_path)
{
	this->_shader_list.push_back({name, vs_path, fs_path});
}

Shader const &oGL_module::getShader(std::string const &name)
{
	std::vector<Shader>::iterator it;

	for (it = this->_shader_list.begin(); it != this->_shader_list.end(); ++it)
	{
		if (it->getName().compare(name) == 0)
			return (*it);
	}
	throw oGL_module::ShaderNotFoundException(name);
}

void oGL_module::add_model(std::string const &name,
						   std::string const &path)
{
	this->_model_list.push_back({name, path});
}

Model const &oGL_module::getModel(std::string const &name)
{
	std::vector<Model>::iterator it;

	for (it = this->_model_list.begin(); it != this->_model_list.end(); ++it)
	{
		if (it->getName().compare(name) == 0)
			return (*it);
	}
	throw oGL_module::ModelNotFoundException(name);
}

oGL_module::ShaderNotFoundException::ShaderNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested shader";
}

oGL_module::ShaderNotFoundException::ShaderNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find shader : ";
	this->_msg += name.c_str();
}

oGL_module::ShaderNotFoundException::~ShaderNotFoundException(void) throw()
{
}

oGL_module::ModelNotFoundException::ModelNotFoundException(void)
{
	this->_msg = "OpenGL : Failed to find requested model";
}

oGL_module::ModelNotFoundException::ModelNotFoundException(std::string const &name)
{
	this->_msg = "OpenGL : Failed to find model : ";
	this->_msg += name.c_str();
}

oGL_module::ModelNotFoundException::~ModelNotFoundException(void) throw()
{
}

oGL_module::oGLFailException::oGLFailException(void)
{
	this->_msg = "OpenGL : Something failed !";
}

oGL_module::oGLFailException::~oGLFailException(void) throw()
{
}
