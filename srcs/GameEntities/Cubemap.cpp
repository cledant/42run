/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cubemap.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 09:57:56 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:43:47 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameEntities/Cubemap.hpp"

Cubemap::Cubemap(Shader const *shader, glm::mat4 const *perspec_mult_view,
				 Model const *model, glm::vec3 const &pos, glm::vec3 const &scale) :
		_shader(shader), _perspec_mult_view(perspec_mult_view), _model(model),
		_pos(pos), _scale(scale)
{
	this->update(0.0f);
}

Cubemap::~Cubemap(void)
{
}

Cubemap::Cubemap(Cubemap const &src)
{
	*this = src;
}

Cubemap &Cubemap::operator=(Cubemap const &rhs)
{
	this->_shader            = rhs.getShader();
	this->_perspec_mult_view = rhs.getPerspecMultView();
	this->_model             = rhs.getModel();
	this->_pos               = rhs.getPos();
	this->_scale             = rhs.getScale();
	this->_total             = rhs.getTotalMatrix();
	return (*this);
}

/*
 * Interface ITranslatable
 */

void Cubemap::translateObject(glm::vec3 const &vec)
{
	this->_pos += vec;
}

void Cubemap::scaleObject(glm::vec3 const &vec)
{
	this->_scale *= vec;
}

/*
 * Interface IEntity
 */

void Cubemap::update(float time)
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

void Cubemap::draw(void)
{
	GLint                                          uniform_mat_total_id = 0;
	std::map<std::string, Texture>::const_iterator ftex;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_model == nullptr || !oGL_module::oGL_getUniformID("mat_total",
																 this->_shader->getShaderProgram(),
																 &uniform_mat_total_id))
	{
		std::cout << "Warning : Can't draw Cubemap" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_mat_total_id, this->_total);
	auto it = this->_model->getTextureList().find("tex1");
	oGL_module::oGL_draw_cubemap(this->_model->getMeshList()[0].getVAO(),
								 it->second.getTextureID(),
								 Cubemap::nb_faces);
}

/*
 * Setter
 */

void Cubemap::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void Cubemap::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

/*
 * Getter
 */

glm::mat4 const &Cubemap::getTotalMatrix(void) const
{
	return (this->_total);
}

Shader const *Cubemap::getShader(void) const
{
	return (this->_shader);
}

Model const *Cubemap::getModel(void) const
{
	return (this->_model);
}

glm::mat4 const *Cubemap::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

glm::vec3 const &Cubemap::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &Cubemap::getScale(void) const
{
	return (this->_scale);
}

Cubemap::InitException::InitException(void)
{
	this->_msg = "Cubemap : Object initialization failed";
}

Cubemap::InitException::~InitException(void) throw()
{
}

float            Cubemap::vertices[] =
						 {
								 -1.0f, 1.0f, -1.0f,
								 -1.0f, -1.0f, -1.0f,
								 1.0f, -1.0f, -1.0f,
								 1.0f, -1.0f, -1.0f,
								 1.0f, 1.0f, -1.0f,
								 -1.0f, 1.0f, -1.0f,

								 -1.0f, -1.0f, 1.0f,
								 -1.0f, -1.0f, -1.0f,
								 -1.0f, 1.0f, -1.0f,
								 -1.0f, 1.0f, -1.0f,
								 -1.0f, 1.0f, 1.0f,
								 -1.0f, -1.0f, 1.0f,

								 1.0f, -1.0f, -1.0f,
								 1.0f, -1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, -1.0f,
								 1.0f, -1.0f, -1.0f,

								 -1.0f, -1.0f, 1.0f,
								 -1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f,
								 1.0f, -1.0f, 1.0f,
								 -1.0f, -1.0f, 1.0f,

								 -1.0f, 1.0f, -1.0f,
								 1.0f, 1.0f, -1.0f,
								 1.0f, 1.0f, 1.0f,
								 1.0f, 1.0f, 1.0f,
								 -1.0f, 1.0f, 1.0f,
								 -1.0f, 1.0f, -1.0f,

								 -1.0f, -1.0f, -1.0f,
								 -1.0f, -1.0f, 1.0f,
								 1.0f, -1.0f, -1.0f,
								 1.0f, -1.0f, -1.0f,
								 -1.0f, -1.0f, 1.0f,
								 1.0f, -1.0f, 1.0f
						 };

size_t            Cubemap::nb_faces = 36;
