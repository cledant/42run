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

#include "GameEntities/basic/Prop.hpp"

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

Prop::Prop(Prop const &src) : IEntity(), ITranslatable()
{
	*this = src;
}

Prop &Prop::operator=(Prop const &rhs)
{
	this->_shader            = rhs.getShader();
	this->_perspec_mult_view = rhs.getPerspecMultView();
	this->_model             = rhs.getModel();
	this->_yaw               = rhs.getYaw();
	this->_pitch             = rhs.getPitch();
	this->_roll              = rhs.getRoll();
	this->_pos               = rhs.getPos();
	this->_scale             = rhs.getScale();
	this->_offset            = rhs.getOffset();
	return (*this);
}

/*
 * Interface IEntity
 */

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
	model = glm::translate(model, (this->_pos + this->_offset));
	model = glm::rotate(model, glm::radians(this->_yaw), glm::vec3({0.0f, 1.0f, 0.0f}));
	model = glm::rotate(model, glm::radians(this->_pitch), glm::vec3({1.0f, 0.0f, 0.0f}));
	model = glm::rotate(model, glm::radians(this->_roll), glm::vec3({0.0f, 0.0f, 1.0f}));
	model = glm::translate(model, glm::vec3({-this->_model->getCenter().x * this->_scale.x,
											 -this->_model->getCenter().y * this->_scale.y,
											 -this->_model->getCenter().z * this->_scale.z}));
	model = glm::scale(model, this->_scale);
	this->_total = *(this->_perspec_mult_view) * model;
}

void Prop::draw(void)
{
	GLint                                          uniform_mat_total_id;
	GLint                                          uniform_tex_diff_id;
	size_t                                         nb_tex = 0;
	std::map<std::string, Texture>::const_iterator ftex;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr ||
		!oGL_module::oGL_getUniformID("uniform_mat_total", this->_shader->getShaderProgram(),
									  &uniform_mat_total_id) ||
		!oGL_module::oGL_getUniformID("uniform_tex_diff", this->_shader->getShaderProgram(),
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
		for (auto it = this->_model->getMeshList()[i].getTextureNameList().cbegin();
			 it != this->_model->getMeshList()[i].getTextureNameList().cend(); ++it)
		{
			if (it->second == Texture::TEX_DIFFUSE)
			{
				if ((ftex = this->_model->getTextureList().find(it->first))
					!= this->_model->getTextureList().cend())
					oGL_module::oGL_set_texture(uniform_tex_diff_id, nb_tex,
												ftex->second.getTextureID());
				nb_tex++;
				break;
			}
		}
		oGL_module::oGL_draw_filled(this->_model->getMeshList()[i].getVAO(),
									this->_model->getMeshList()[i].getIndiceList().size());
	}
}

/*
 * Interface ITranslatable
 */

void Prop::translateObject(glm::vec3 const &vec)
{
	this->_pos += vec;
}

void Prop::scaleObject(glm::vec3 const &vec)
{
	this->_scale *= vec;
}

/*
 * Setter
 */

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

/*
 * Getter
 */

glm::mat4 const &Prop::getTotalMatrix(void) const
{
	return (this->_total);
}

GLfloat Prop::getYaw(void) const
{
	return (this->_yaw);
}

GLfloat Prop::getPitch(void) const
{
	return (this->_pitch);
}

GLfloat Prop::getRoll(void) const
{
	return (this->_roll);
}

Shader const *Prop::getShader(void) const
{
	return (this->_shader);
}

glm::mat4 const *Prop::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

Model const *Prop::getModel(void) const
{
	return (this->_model);
}

glm::vec3 const &Prop::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &Prop::getScale(void) const
{
	return (this->_scale);
}

glm::vec3 const &Prop::getOffset(void) const
{
	return (this->_offset);
}

Prop::InitException::InitException(void)
{
	this->_msg = "Prop : Object initialization failed";
}

Prop::InitException::~InitException(void) throw()
{
}