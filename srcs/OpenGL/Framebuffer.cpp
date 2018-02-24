/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Framebuffer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/Framebuffer.hpp"

Framebuffer::Framebuffer(int h, int w) : _fbo(0), _textureColorBuffer(0), _rbo(0)
{
	try
	{
		this->_allocate_buffers(h, w);
	}
	catch (std::exception &e)
	{
		glDeleteFramebuffers(1, &this->_fbo);
		glDeleteTextures(1, &this->_textureColorBuffer);
		glDeleteRenderbuffers(1, &this->_rbo);
	}
}

Framebuffer::~Framebuffer(void)
{
	glDeleteFramebuffers(1, &this->_fbo);
	glDeleteTextures(1, &this->_textureColorBuffer);
	glDeleteRenderbuffers(1, &this->_rbo);
}

Framebuffer::Framebuffer(Framebuffer &&src)
{
	*this = std::move(src);
}

Framebuffer &Framebuffer::operator=(Framebuffer &&rhs)
{
	this->_fbo                = rhs.moveFramebuffer();
	this->_textureColorBuffer = rhs.moveTextureColorBuffer();
	this->_rbo                = rhs.moveRBO();
	return (*this);
}

void Framebuffer::useFramebuffer(void) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
}

void Framebuffer::defaultFramebuffer(void) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
 * Getter
 */

GLuint Framebuffer::moveFramebuffer(void)
{
	GLuint tmp = this->_fbo;

	this->_fbo = 0;
	return (tmp);
}

GLuint Framebuffer::moveTextureColorBuffer(void)
{
	GLuint tmp = this->_textureColorBuffer;

	this->_textureColorBuffer = 0;
	return (tmp);
}

GLuint Framebuffer::moveRBO(void)
{
	GLuint tmp = this->_rbo;

	this->_rbo = 0;
	return (tmp);
}

GLuint Framebuffer::getTextureColorBuffer(void) const
{
	return (this->_textureColorBuffer);
}

inline void Framebuffer::_allocate_buffers(int h, int w)
{
	glGenFramebuffers(1, &this->_fbo);
	if (glGetError() != GL_NO_ERROR)
		throw Framebuffer::InitException();
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fbo);
	
}