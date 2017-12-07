/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 17:57:56 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 17:57:56 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Prop.hpp"

Prop::Params::Params(void)
{
	this->shader            = nullptr;
	this->perspec_mult_view = nullptr;
	this->model             = nullptr;
	this->pos               = glm::vec3(0.0f, 0.0f, 0.0f);
	this->orientation       = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale             = glm::vec3(1.0f, 1.0f, 1.0f);
	this->offset            = glm::vec3(0.0f, 0.0f, 0.0f);
}

Prop::Params::~Params(void)
{
}

Prop::Prop(Prop::Params const &params) :
		_shader(params.shader), _perspec_mult_view(params.perspec_mult_view),
		_model(params.model), _yaw(params.orientation.x), _pitch(params.orientation.y),
		_roll(params.orientation.z), _pos(params.pos), _scale(params.scale),
		_offset(params.offset)
{
	this->update(0.0f);
}

Prop::~Prop(void)
{
}

void Prop::update(float time)
{
	glm::mat4 model;

	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Warning : Can't update Cubemap" << std::endl;
		return;
	}
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(this->_yaw), glm::vec3({0.0f, 1.0f, 0.0f}));
	model = glm::rotate(model, glm::radians(this->_pitch), glm::vec3({1.0f, 0.0f, 0.0f}));
	model = glm::rotate(model, glm::radians(this->_roll), glm::vec3({0.0f, 0.0f, 1.0f}));
	model = glm::translate(model, glm::vec3({-this->_model->getCenter().x * this->_scale.x,
											 -this->_model->getCenter().y * this->_scale.y,
											 -this->_model->getCenter().z * this->_scale.z}));
	model = glm::translate(model, (this->_pos + this->_offset));
	model = glm::scale(model, this->_scale);
	this->_total = *(this->_perspec_mult_view) * model;
}

void Prop::draw(void)
{
	GLint  uniform_mat_total_id;
	GLint  uniform_tex_diff_id;
	size_t nb_tex = 0;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr ||
		oGL_module::oGL_getUniformID("uniform_mat_total", this->_shader->getShaderProgram(),
									 &uniform_mat_total_id) == false ||
		oGL_module::oGL_getUniformID("uniform_tex_diff", this->_shader->getShaderProgram(),
									 &uniform_tex_diff_id) == false)
	{
		std::cout << "Warning : Can't draw Cubemap" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_mat_total_id, this->_total);
	for (size_t i = 0; i < this->_model->getMeshList().size(); ++i)
	{
		nb_tex = 0;
		for (size_t j = 0; j < this->_model->getMeshList()[i].getTextureList().size(); ++j)
		{
			if (this->_model->getMeshList()[i].getTextureList()[j]
						.getTextureType() == Texture::TEX_DIFFUSE)
			{
				oGL_module::oGL_set_texture(uniform_tex_diff_id, nb_tex,
											this->_model->getMeshList()[i].getTextureList()[j].getTextureID());
				nb_tex++;
				break;
			}
		}
		oGL_module::oGL_draw_filled(this->_model->getMeshList()[i].getVAO(),
									this->_model->getMeshList()[i].getIndiceList().size());
	}
}

void Prop::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void Prop::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

void Prop::setYaw(GLfloat yaw)
{
	this->_yaw = yaw;
}

void Prop::setPitch(GLfloat pitch)
{
	this->_pitch = pitch;
}

void Prop::setRoll(GLfloat roll)
{
	this->_roll = roll;
}

void Prop::setOffset(glm::vec3 const &offset)
{
	this->_offset = offset;
}

glm::mat4 const &Prop::getTotalMatrix(void) const
{
	return (this->_total);
}

Prop::InitException::InitException(void)
{
	this->_msg = "Prop : Object initialization failed";
}

Prop::InitException::~InitException(void) throw()
{
}