/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 17:05:36 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:46:06 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "stb_image.h"
# include <iostream>
# include <fstream>
# include <vector>

class Texture
{
	public :

		typedef enum e_tex_gl_type
		{
			TEX_FLAT,
			TEX_CUBE,
		} t_tex_gl_type;

		typedef enum e_tex_type
		{
			TEX_DIFFUSE;
			TEX_SPECULAR;
			TEX_NORMAL;
			TEX_HEIGHT;
		} t_tex_type;

		Texture(void);
		Texture(std::string const &name, std::vector<std::string> const &files,
				Texture::t_tex_gl_type gl_type, Texture::t_tex_type type);
		Texture(const Texture &src) = delete;
		Texture &operator=(Texture const &rhs) = delete;
		Texture(Texture &&src);
		Texture &operator=(Texture &&rhs);
		virtual ~Texture(void);

		std::string const &getName(void) const;
		GLuint getTextureID(void) const;
		Texture::t_tex_type getTextureType(void) const;
		GLuint moveTexture(void);

		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException(void);
				explicit FileOpenException(std::string const &path);
				virtual ~FileOpenException(void) throw();
		};

		class AllocationException : public GeneralException
		{
			public :

				explicit AllocationException(void);
				virtual ~AllocationException(void) throw();
		};

		class TypeException : public GeneralException
		{
			public :

				explicit TypeException(void);
				virtual ~TypeException(void) throw();
		};

		class NumberException : public GeneralException
		{
			public :

				explicit NumberException(void);
				virtual ~NumberException(void) throw();
		};

		class ChannelNumberException : public GeneralException
		{
			public :

				explicit ChannelNumberException(void);
				virtual ~ChannelNumberException(void) throw();
		};

	private :

		std::string _name;
		GLuint      _tex_id;
		t_tex_type	_type;

		static GLuint _load_cubemap(std::vector<std::string> const &files);
		static GLuint _load_flat(std::vector<std::string> const &files);
};

#endif
