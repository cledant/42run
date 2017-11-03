/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 13:11:10 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:46:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <fstream>

class Shader
{
	public :

        Shader(void);
		Shader(std::string const &name, std::string const &vs_path,
				std::string const &fs_path);
		Shader(Shader &src) = delete;
		Shader		&operator=(Shader const &rhs) = delete;
        Shader(Shader &&src);
        Shader		&operator=(Shader &&rhs);
		virtual ~Shader(void);

		std::string const		&getName(void) const;
		GLuint					getShaderProgram(void) const;
        GLuint                  moveShaderProgram(void);
		void					use(void) const;
		void					setMat4(GLint uniform_id,
									glm::mat4 const &mat4) const;
		void					setVec3(GLint uniform_id,
									glm::vec3 const &vec3) const;

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

	class CompileException : public GeneralException
	{
		public :

			explicit CompileException(void);
			virtual ~CompileException(void) throw();
	};

	class LinkException : public GeneralException
	{
		public :

			explicit LinkException(void);
			virtual ~LinkException(void) throw();
	};

	private :

		std::string			_name;
		GLuint				_shader_program;

		static GLuint		_load_shader(std::string const &path, GLenum type);
		static GLuint		_compile_program(GLuint vs, GLuint fs);
		static void			_get_shader_error(GLuint shader);
		static void			_read_file(std::string const &path,
								std::string &content);
};

#endif
