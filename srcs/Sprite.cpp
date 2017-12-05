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
			   glm::vec4 const &sprite_tex_size_offset, size_t nb_walk_frame) :
		_shader(shader), _perspec_mult_view(perspec_mult_view), _tex(tex),
		_sprite_tex_pos({sprite_tex_size_offset.x, sprite_tex_size_offset.y}),
		_sprite_tex_offset({sprite_tex_size_offset.z, sprite_tex_size_offset.w}),
		_pos(pos), _scale(scale), _yaw(0.0f), _nb_faces(6), _sprite_x(0),
		_sprite_y(0), _nb_walk_frame(nb_walk_frame), _offset(glm::vec2({0.0f, 0.0f}))
{
	float list_vertices[] = {
			0.0f, -1.0f, -1.0f, this->_sprite_tex_pos.x, this->_sprite_tex_pos.y,
			0.0f, -1.0f, 1.0f, 0.0f, this->_sprite_tex_pos.y,
			0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, -1.0f, this->_sprite_tex_pos.x, 0.0f,
			0.0f, -1.0f, -1.0f, this->_sprite_tex_pos.x, this->_sprite_tex_pos.y
	};

	if (shader == nullptr || perspec_mult_view == nullptr || tex == nullptr)
		throw InitException::InitException();
	std::memcpy(&(this->_vertices), &list_vertices, sizeof(float) * 30);
	try
	{
		this->_allocate_set_GL_ressources();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		throw InitException::InitException();
	}
	(void) this->_nb_walk_frame;
}

Sprite::~Sprite(void)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
}

void Sprite::setPosition(glm::vec3 const &pos)
{
	this->_pos = pos;
}

void Sprite::setScale(glm::vec3 const &scale)
{
	this->_scale = scale;
}

void Sprite::setYaw(float yaw)
{
	this->_yaw = yaw;
}

void Sprite::setSpriteX(size_t pos_x)
{
	this->_sprite_x = pos_x;
}

void Sprite::setSpriteY(size_t pos_y)
{
	this->_sprite_y = pos_y;
}

glm::mat4 const &Sprite::getTotalMatrix(void) const
{
	return (this->_total);
}

size_t Sprite::getNbOfWalkFrame(void) const
{
	return (this->_nb_walk_frame);
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

void Sprite::update(float time)
{
	glm::mat4 model;

	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_tex == nullptr)
	{
		std::cout << "Warning : Can't update Sprite" << std::endl;
		return;
	}
	model = glm::mat4(1.0f);
	model = glm::translate(model, this->_pos);
	model = glm::scale(model, this->_scale);
	model = glm::rotate(model, glm::radians(-this->_yaw), glm::vec3({0.0f, 1.0f, 0.0f}));
	this->_total  = *(this->_perspec_mult_view) * model;
	this->_offset = {((this->_sprite_tex_pos.x + this->_sprite_tex_offset.x) * this->_sprite_x),
					 ((this->_sprite_tex_pos.y + this->_sprite_tex_offset.y) * this->_sprite_y)};
}

void Sprite::draw(void)
{
	GLint uniform_mat_total_id;
	GLint uniform_tex_diff_id;
	GLint uniform_tex_offset_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_tex == nullptr || !oGL_module::oGL_getUniformID(
			"uniform_mat_total", this->_shader->getShaderProgram(),
			&uniform_mat_total_id) ||
		!oGL_module::oGL_getUniformID("uniform_tex_diff",
									  this->_shader->getShaderProgram(),
									  &uniform_tex_diff_id) ||
		!oGL_module::oGL_getUniformID("uniform_tex_offset",
									  this->_shader->getShaderProgram(),
									  &uniform_tex_offset_id))
	{
		std::cout << "Warning : Can't draw Sprite" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_mat_total_id, this->_total);
	this->_shader->setVec2(uniform_tex_offset_id, this->_offset);
	oGL_module::oGL_set_texture(uniform_tex_diff_id, 0, this->_tex->getTextureID());
	oGL_module::oGL_draw_filled(this->_vao, Sprite::_nb_faces);
}

void Sprite::_allocate_set_GL_ressources(void)
{
	this->_vbo = oGL_module::oGL_create_vbo(sizeof(GLfloat) * 5 * 6, &(Sprite::_vertices));
	this->_vao = oGL_module::oGL_create_vao();
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 0, 3, sizeof(GLfloat) * 5, 0);
	oGL_module::oGL_set_vao_parameters(this->_vao, this->_vbo, 1, 2, sizeof(GLfloat) * 5, sizeof(GLfloat) * 3);
}

Sprite::InitException::InitException(void)
{
	this->_msg = "Sprite : Object initialization failed";
}

Sprite::InitException::~InitException(void) throw()
{
}
