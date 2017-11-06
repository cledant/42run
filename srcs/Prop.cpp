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

Prop::Prop(Shader const *shader, glm::mat4 const *perspec_mult_view,
		   Model const *model, glm::vec3 const &pos,
		   glm::vec3 const &scale) :
		_shader(shader), _perspec_mult_view(perspec_mult_view), _model(model),
		_yaw(0.0f), _pitch(0.0f), _pos(pos), _scale(scale)
{
	this->update(0.0f);
}

Prop::~Prop(void)
{
}

void Prop::update(float time)
{
	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr)
	{
		std::cout << "Warning : Can't update Cubemap" << std::endl;
		return;
	}
	this->_total = *(this->_perspec_mult_view) *
				   glm::scale(glm::translate(glm::mat4(1.0f), this->_pos), this->_scale);
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
									 &uniform_tex_diff_id))
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
				this->_shader->getShaderProgram().setTexture(uniform_tex_diff_id,
															 this->_model->getMeshList()[i].getTextureList()[j]
																	 .getTextureID());
				break;
			}
		}
		oGL_module::oGL_draw_indiced_filled(this->_model->getMeshList()[i].getVAO(),
											this->_model->getMeshList()[i].getIndiceList().size(),
											&(this->_model->getMeshList()[i].getIndiceList()));
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