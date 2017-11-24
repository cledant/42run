/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sprite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 17:57:56 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 17:57:56 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sprite.hpp"

Sprite::Sprite(Shader const *shader, glm::mat4 const *perspec_mult_view,
			   Texture const *tex, glm::vec3 const &pos, glm::vec3 const &scale,
			   size_t sprites_per_lines, size_t lines_of_sprites)
{
	if (shader == nullptr || perspec_mult_view == nullptr || tex == nullptr)
		throw InitException::InitException();

}

Sprite::~Sprite(void)
{
}

void Sprite::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void Sprite::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

glm::mat4 const &Sprite::getTotalMatrix(void) const
{
	return (this->_total);
}

GLuint Sprite::moveVAO(void)
{
	GLuint tmp = this->_vao;

	this->_vao = 0;
	return (tmp);
}

GLuint Sprite::moveVBO(void)
{
	GLuint tmp = this->_vbo;

	this->_vbo = 0;
	return (tmp);
}

Sprite::InitException::InitException(void)
{
	this->_msg = "Prop : Object initialization failed";
}

Sprite::InitException::~InitException(void) throw()
{
}

float Sprite::_vertices[] =
			  {
					  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
					  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

					  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
					  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
					  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			  };

size_t Sprite::_nb_faces = 2;