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

#include "Cubemap.hpp"

Cubemap::Cubemap(Shader const *shader, glm::mat4 const *perspec_mult_view,
	std::vector<std::string> const &files, glm::vec3 const &pos,
	glm::vec3 const &scale) :
	_shader(shader), _perspec_mult_view(perspec_mult_view), _tex(nullptr),
	_vbo(0), _vao(0), _pos(pos), _scale(scale)
{
	try
	{
		this->_vbo = oGL_module::oGL_create_vbo(sizeof(float) * 6 * 3 * 6,
			static_cast<void *>(Cubemap::_vertices));
		this->_vao = oGL_module::oGL_create_vao();
		oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3,
			sizeof(GLfloat) * 3, 0);
        this->_tex = new Texture("tex_cubemap", files, Texture::TEX_CUBE);
	}
	catch (std::exception &e)
	{
        std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
        delete _tex;
		throw Cubemap::InitException();
	}
	this->update(0.0f);
}

Cubemap::~Cubemap(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

Cubemap::Cubemap(Cubemap const &src)
{
	static_cast<void>(src);
}

Cubemap			&Cubemap::operator=(Cubemap const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void				Cubemap::update(float time)
{
	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
			this->_tex == nullptr)
	{
		std::cout << "Warning : Can't update Cubemap" << std::endl;
		return ;
	}
	this->_total = *(this->_perspec_mult_view) *
		glm::scale(glm::translate(glm::mat4(1.0f), this->_pos), this->_scale);
}

void				Cubemap::draw(void)
{
	GLint	uniform_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
			this->_tex == nullptr || oGL_module::oGL_getUniformID("mat_total",
			this->_shader->getShaderProgram(), &uniform_id) == false)
	{
		std::cout << "Warning : Can't draw Cubemap" << std::endl;
		return ;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_id, this->_total);
	oGL_module::oGL_draw_cubemap(this->_vao, this->_tex->getTextureID(),
		Cubemap::_nb_faces);
}

void				Cubemap::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void				Cubemap::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

glm::mat4 const		&Cubemap::getTotalMatrix(void) const
{
	return (this->_total);
}

Cubemap::InitException::InitException(void)
{
	this->_msg = "Cubemap : Object initialization failed";
}

Cubemap::InitException::~InitException(void) throw()
{
}

float			Cubemap::_vertices[] =
{
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

size_t			Cubemap::_nb_faces = 36;
