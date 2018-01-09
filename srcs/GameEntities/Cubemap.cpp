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
				 std::vector<std::string> const &files, glm::vec3 const &pos,
				 glm::vec3 const &scale) :
		_shader(shader), _perspec_mult_view(perspec_mult_view), _tex(nullptr),
		_vbo(0), _vao(0), _pos(pos), _scale(scale), _src_tex(Cubemap::INTERNAL)
{
	try
	{
		this->_oGL_alloc();
		this->_tex = new Texture("tex_cubemap", files, Texture::TEX_CUBE, Texture::TEX_DIFFUSE);
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

Cubemap::Cubemap(Shader const *shader, glm::mat4 const *perspec_mult_view,
				 Texture const *tex, glm::vec3 const &pos,
				 glm::vec3 const &scale) :
		_shader(shader), _perspec_mult_view(perspec_mult_view), _tex(tex),
		_vbo(0), _vao(0), _pos(pos), _scale(scale), _src_tex(Cubemap::EXTERNAL)
{
	try
	{
		this->_oGL_alloc();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw Cubemap::InitException();
	}
	this->update(0.0f);
}

Cubemap::~Cubemap(void)
{
	if (this->_src_tex == Cubemap::INTERNAL)
		delete this->_tex;
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

Cubemap::Cubemap(Cubemap &&src)
{
	*this = std::move(src);
}

Cubemap &Cubemap::operator=(Cubemap &&src)
{
	this->_shader            = src.getShader();
	this->_perspec_mult_view = src.getPerspecMultView();
	this->_tex               = src.moveTexture();
	this->_vbo               = src.moveVBO();
	this->_vao               = src.moveVAO();
	this->_pos               = src.getPos();
	this->_scale             = src.getScale();
	this->_total             = src.getTotalMatrix();
	this->_src_tex           = src.getSrcTex();
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
		this->_tex == nullptr)
	{
		std::cout << "Warning : Can't update Cubemap" << std::endl;
		return;
	}
	this->_total = *(this->_perspec_mult_view) *
				   glm::scale(glm::translate(glm::mat4(1.0f), this->_pos), this->_scale);
}

void Cubemap::draw(void)
{
	GLint uniform_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_tex == nullptr || !oGL_module::oGL_getUniformID("mat_total",
															   this->_shader->getShaderProgram(),
															   &uniform_id))
	{
		std::cout << "Warning : Can't draw Cubemap" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_id, this->_total);
	oGL_module::oGL_draw_cubemap(this->_vao, this->_tex->getTextureID(),
								 Cubemap::_nb_faces);
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

glm::mat4 const *Cubemap::getPerspecMultView(void) const
{
	return (this->_perspec_mult_view);
}

Texture const *Cubemap::moveTexture(void)
{
	Texture const *tmp;

	tmp = this->_tex;
	this->_tex = nullptr;
	return (tmp);
}

GLuint Cubemap::moveVAO(void)
{
	GLuint tmp;

	tmp = this->_vao;
	this->_vao = 0;
	return (tmp);
}

GLuint Cubemap::moveVBO(void)
{
	GLuint tmp;

	tmp = this->_vbo;
	this->_vbo = 0;
	return (tmp);
}

glm::vec3 const &Cubemap::getPos(void) const
{
	return (this->_pos);
}

glm::vec3 const &Cubemap::getScale(void) const
{
	return (this->_scale);
}

Cubemap::t_tex_source Cubemap::getSrcTex() const
{
	return (this->_src_tex);
}

/*
 * Private
 */

void Cubemap::_oGL_alloc(void)
{
	this->_vbo = oGL_module::oGL_create_vbo(sizeof(float) * 6 * 3 * 6,
											static_cast<void *>(Cubemap::_vertices));
	this->_vao = oGL_module::oGL_create_vao();
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3,
									   sizeof(GLfloat) * 3, 0);
}

Cubemap::InitException::InitException(void)
{
	this->_msg = "Cubemap : Object initialization failed";
}

Cubemap::InitException::~InitException(void) throw()
{
}

float            Cubemap::_vertices[] =
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

size_t            Cubemap::_nb_faces = 36;
