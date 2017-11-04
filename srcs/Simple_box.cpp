/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simple_box.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/05 17:30:41 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:32:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Simple_box.hpp"

Simple_box::Simple_box(Shader const *shader, glm::mat4 const *perspec_mult_view,
					   glm::vec3 const &pos, glm::vec3 const &scale) : _shader(shader),
																	   _perspec_mult_view(perspec_mult_view), _vbo(0),
																	   _vao(0), _pos(pos),
																	   _scale(scale)
{
	try
	{
		this->_vbo = oGL_module::oGL_create_vbo(sizeof(float) * 6 * 6 * 6,
												static_cast<void *>(Simple_box::_vertices));
		this->_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3,
										   sizeof(GLfloat) * 6, 0);
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 1, 3,
										   sizeof(GLfloat) * 6, sizeof(GLfloat) * 3);
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw Simple_box::InitException();
	}
	this->update(0.0f);
}

Simple_box::~Simple_box(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

void Simple_box::update(float time)
{
	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr)
	{
		std::cout << "Warning : Can't update Simple_box" << std::endl;
		return;
	}
	this->_total = *(this->_perspec_mult_view) *
				   glm::scale(glm::translate(glm::mat4(1.0f), this->_pos), this->_scale);
}

void Simple_box::draw(void)
{
	GLint uniform_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		oGL_module::oGL_getUniformID("mat_total",
									 this->_shader->getShaderProgram(), &uniform_id) == false)
	{
		std::cout << "Warning : Can't draw Simple_box" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_id, this->_total);
	oGL_module::oGL_draw_filled(this->_vao, Simple_box::_nb_faces);
}

void Simple_box::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void Simple_box::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

glm::mat4 const &Simple_box::getTotalMatrix(void) const
{
	return (this->_total);
}

Simple_box::InitException::InitException(void)
{
	this->_msg = "Simple_Box : Object initialization failed";
}

Simple_box::InitException::~InitException(void) throw()
{
}

float            Simple_box::_vertices[] =
						 {
								 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
								 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
								 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
								 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
								 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
								 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

								 -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
								 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
								 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
								 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
								 -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
								 -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

								 -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
								 -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
								 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
								 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
								 -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
								 -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

								 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
								 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
								 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
								 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
								 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
								 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

								 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
								 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
								 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
								 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
								 -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
								 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

								 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
								 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
								 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
								 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
								 -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
								 -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f
						 };

size_t            Simple_box::_nb_faces = 36;
