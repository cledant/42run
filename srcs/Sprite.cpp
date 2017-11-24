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
			   size_t sprites_per_lines, size_t lines_of_sprites) :
		_shader(shader), _perspec_mult_view(perspec_mult_view), _tex(tex),
		_sprites_per_line(sprites_per_lines), _lines_of_sprites(lines_of_sprites),
		_pos(pos), _scale(scale)
{
	if (shader == nullptr || perspec_mult_view == nullptr || tex == nullptr)
		throw InitException::InitException();
	this->_allocate_set_GL_ressources();
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

void Sprite::update(float time)
{
	static_cast<void>(time);
	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_tex == nullptr)
	{
		std::cout << "Warning : Can't update Sprite" << std::endl;
		return;
	}
	this->_total = *(this->_perspec_mult_view) *
				   glm::scale(glm::translate(glm::mat4(1.0f), this->_pos), this->_scale);
}

void Sprite::draw(void)
{
	GLint uniform_mat_total_id;
	GLint uniform_tex_diff_id;

	if (this->_shader == nullptr || this->_perspec_mult_view == nullptr ||
		this->_tex == nullptr || oGL_module::oGL_getUniformID(
			"uniform_mat_total", this->_shader->getShaderProgram(), &uniform_mat_total_id) == false ||
		oGL_module::oGL_getUniformID("uniform_tex_diff", this->_shader->getShaderProgram(),
									 &uniform_tex_diff_id) == false)
	{
		std::cout << "Warning : Can't draw Sprite" << std::endl;
		return;
	}
	this->_shader->use();
	this->_shader->setMat4(uniform_mat_total_id, this->_total);
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
	this->_msg = "Prop : Object initialization failed";
}

Sprite::InitException::~InitException(void) throw()
{
}

float Sprite::_vertices[] =
			  {
					  0.0f, -1.0f, -1.0f, 0.0f, 0.0f,
					  0.0f, -1.0f, 1.0f, 0.0f, 1.0f,
					  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

					  0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
					  0.0f, 1.0f, -1.0f, 1.0f, 0.0f,
					  0.0f, -1.0f, -1.0f, 0.0f, 0.0f
			  };

size_t Sprite::_nb_faces = 12;