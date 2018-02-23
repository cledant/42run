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

#include "UI/OrientableShaderSurface.hpp"

OrientableShaderSurface::Params::Params(void)
{
	this->win               = nullptr;
	this->input             = nullptr;
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->pos               = glm::vec3(0.0f, 0.0f, 0.0f);
	this->offset            = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale             = glm::vec3(1.0f, 1.0f, 1.0f);
	this->orientation       = glm::vec3(0.0f, 0.0f, 0.0f);
}

OrientableShaderSurface::Params::~Params(void)
{
}

OrientableShaderSurface::OrientableShaderSurface(OrientableShaderSurface::Params const &params) :
		ShaderSurface(params.win, params.input, params.shader),
		_perspec_mult_view(params.perspec_mult_view), _pos(params.pos),
		_offset(params.offset), _scale(params.scale), _orientation(params.orientation)
{
	this->update(0.0f);
}

OrientableShaderSurface::~OrientableShaderSurface(void)
{
}

OrientableShaderSurface::OrientableShaderSurface(OrientableShaderSurface &&src) :
		ShaderSurface(src.getWindow(), src.getInput(), src.getShader())
{
	*this = std::move(src);
}

OrientableShaderSurface &OrientableShaderSurface::operator=(OrientableShaderSurface &&rhs)
{
	ShaderSurface::operator=(std::move(rhs));
	this->_perspec_mult_view = rhs.getPerspecMultView();
	this->_total             = rhs.getTotalMatrix();
	this->_pos               = rhs.getPos();
	this->_scale             = rhs.getScale();
	this->_offset            = rhs.getOffset();
	this->_offset            = rhs.getOrientation();
	return (*this);
}

/*
 * Setter
 */

void OrientableShaderSurface::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void OrientableShaderSurface::setOffset(glm::vec3 const &offset)
{
	this->_offset = offset;
}

void OrientableShaderSurface::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

void OrientableShaderSurface::setOrientation(glm::vec3 const &orientation)
{
	this->_orientation = orientation;
}

/*
 * Getter
 */

glm::mat4 const *OrientableShaderSurface::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

glm::mat4 const &OrientableShaderSurface::getTotalMatrix(void) const
{
	return (this->_total);
}

glm::vec3 const &OrientableShaderSurface::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &OrientableShaderSurface::getScale(void) const
{
	return (this->_scale);
}

glm::vec3 const &OrientableShaderSurface::getOffset(void) const
{
	return (this->_offset);
}

glm::vec3 const &OrientableShaderSurface::getOrientation(void) const
{
	return (this->_orientation);
}

/*
 * Inherited Interface IEntity
 */

void OrientableShaderSurface::update(float time)
{
	glm::mat4 model;

	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr)
	{
		std::cout << "Warning : Can't update OrientableShaderSuraface" << std::endl;
		return;
	}
	model = glm::mat4(1.0f);
	model = glm::translate(model, (this->_pos + this->_offset));
	model = glm::scale(model, this->_scale);
	model = glm::rotate(model, glm::radians(-this->_orientation.x),
						glm::vec3({1.0f, 0.0f, 0.0f}));
	model = glm::rotate(model, glm::radians(-this->_orientation.y),
						glm::vec3({0.0f, 1.0f, 0.0f}));
	model = glm::rotate(model, glm::radians(-this->_orientation.z),
						glm::vec3({0.0f, 0.0f, 1.0f}));
	this->_total = *(this->_perspec_mult_view) * model;
}

void OrientableShaderSurface::draw(void)
{
	GLint id_time;
	GLint id_resolution;
	GLint id_total_matrix;
	GLint id_inv_total_matrix;
	GLint id_viewport;

	if (this->_shader == nullptr || this->_win == nullptr ||
		!oGL_module::oGL_getUniformID("uniform_time",
									  this->_shader->getShaderProgram(),
									  &id_time) ||
		!oGL_module::oGL_getUniformID("uniform_resolution",
									  this->_shader->getShaderProgram(),
									  &id_resolution) ||
		!oGL_module::oGL_getUniformID("uniform_total_matrix",
									  this->_shader->getShaderProgram(),
									  &id_total_matrix) ||
		!oGL_module::oGL_getUniformID("uniform_inv_total_matrix",
									  this->_shader->getShaderProgram(),
									  &id_inv_total_matrix) ||
		!oGL_module::oGL_getUniformID("uniform_viewport",
									  this->_shader->getShaderProgram(),
									  &id_viewport))
	{
		std::cout << "Warning : Can't draw ShaderSurface" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setVec2(id_resolution, glm::vec2{this->_win->cur_win_w,
													this->_win->cur_win_h});
	this->_shader->setFloat(id_time, Glfw_manager::getTime());
	this->_shader->setMat4(id_total_matrix, this->_total);
	this->_shader->setMat4(id_inv_total_matrix, glm::inverse(this->_total));
	this->_shader->setVec4(id_viewport, glm::vec4{0, 0, this->_win->cur_win_w,
												  this->_win->cur_win_h});
	oGL_module::oGL_draw_filled(this->_vao, ShaderSurface::_nb_faces);
}