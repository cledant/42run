/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderSurface.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERSURFACE_HPP
# define SHADERSURFACE_HPP

# include "OpenGL/oGL_module.hpp"
# include "WindowManager/Glfw_manager.hpp"
# include "Interfaces/IEntity.hpp"
# include "WindowManager/Window.hpp"
# include "WindowManager/Input.hpp"
# include "OpenGL/Shader.hpp"

class ShaderSurface : public IEntity
{
	public :

		ShaderSurface(GLFW_Window const *win, Input const *input, Shader const *shader);
		virtual ~ShaderSurface(void);
		ShaderSurface(ShaderSurface const &src) = delete;
		ShaderSurface &operator=(ShaderSurface const &rhs) = delete;
		ShaderSurface(ShaderSurface &&src);
		ShaderSurface &operator=(ShaderSurface &&rhs);

		/*
		 * Getter
		 */

		Shader const *getShader(void) const;
		Input const *getInput(void) const;
		GLFW_Window const *getWindow(void) const;
		GLuint moveVAO(void);
		GLuint moveVBO(void);

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void draw(void);

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	protected :

		GLFW_Window const *_win;
		Input const       *_input;
		Shader const      *_shader;
		GLuint            _vao;
		GLuint            _vbo;

		virtual void _allocate_buffer(void);

		static float  _vertices[];
		static size_t _nb_faces;
};

#endif
