/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 17:38:14 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 18:07:09 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.hpp"

Texture::Texture(std::string const &name, std::vector<std::string> const &files,
	Texture::t_tex_type type) : _name(name), _tex_id(0)
{
	switch (type)
	{
		case TEX_CUBE :
			this->_tex_id = Texture::_load_cubemap(files);
			break;
		default :
			throw TypeException();
			break;
	}
}

Texture::~Texture(void)
{
}

Texture::Texture(Texture const &src)
{
	this->_name = src.getName();
	this->_tex_id = src.getTextureID();
}

Texture		&Texture::operator=(Texture const &rhs)
{
	this->_name = rhs.getName();
	this->_tex_id = rhs.getTextureID();
	return (*this);
}

std::string const		&Texture::getName(void) const
{
	return (this->_name);
}

GLuint					Texture::getTextureID(void) const
{
	return (this->_tex_id);
}

GLuint			Texture::_load_cubemap(std::vector<std::string> const &files)
{
	GLuint			tex_id;
	size_t			i = 0;
	int				tex_w;
	int				tex_h;
	int				tex_nb_chan;
	unsigned char	*data;

	if (files.size() != 6)
		throw Texture::NumberException();
	glGenTextures(1, &tex_id);
	if (glGetError() != GL_NO_ERROR)
		throw Texture::AllocationException();
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
	while (i < 6)
	{
		if ((data = stbi_load(files[i].c_str(), &tex_w, &tex_h,
			&tex_nb_chan, 0)) != NULL)
		{
			std::cout << "Loading : " << files[i] << std::endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glDeleteTextures(1, &tex_id);
			throw FileOpenException(files[i]);
		}
		++i;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return (tex_id);
}

Texture::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Texture : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Texture::FileOpenException::FileOpenException(void)
{
	this->_msg = "Texture : Failed to find to open file";
}

Texture::FileOpenException::~FileOpenException(void) throw()
{
}

Texture::AllocationException::AllocationException(void)
{
	this->_msg = "Texture : Failed to allocate memory";
}

Texture::AllocationException::~AllocationException(void) throw()
{
}

Texture::TypeException::TypeException(void)
{
	this->_msg = "Texture : Unsupported type";
}

Texture::TypeException::~TypeException(void) throw()
{
}

Texture::NumberException::NumberException(void)
{
	this->_msg = "Texture : Invalid number of file";
}

Texture::NumberException::~NumberException(void) throw()
{
}
