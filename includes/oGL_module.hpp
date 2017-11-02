/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oGL_module.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/10/13 09:45:57 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OGL_MODULE_HPP
# define OGL_MODULE_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "Shader.hpp"
# include "Texture.hpp"
# include "Model.hpp"
# include "GeneralException.hpp"
# include <iostream>
# include <vector>

class oGL_module
{
	public :

		oGL_module(void);
		virtual ~oGL_module(void);

		static void			oGL_check_error(void);
		static void			oGL_display_error(void);
		static GLuint		oGL_create_vbo(size_t size, void *data);
		static void			oGL_delete_vbo(GLuint vbo);
		static GLuint		oGL_create_vao(void);
		static void			oGL_delete_vao(GLuint vao);
		static void			oGL_set_vao_parameters(GLuint vao, GLuint vbo,
								GLuint index, GLint size, GLsizei stride,
								size_t shift);
		static void			oGL_clear_buffer(float r, float g, float b);
		static void			oGL_update_framebuffer(int width, int height);
		static void			oGL_enable_depth(void);
		static bool			oGL_getUniformID(std::string const &name,
								GLuint prog, GLint *uniform_id);
		static void			oGL_draw_filled(GLuint vao, size_t nb_faces);
		static void			oGL_draw_points(GLuint vao, size_t nb_faces);
		static void			oGL_draw_cubemap(GLuint vao, GLuint tex,
								size_t nb_faces);
		static void			oGL_finish(void);

		void				add_shader(std::string const &name,
								std::string const &vs_path,
								std::string const &fs_path);
		Shader const		&getShader(std::string const &name);
		void				delete_all_shaders(void);
		void				add_texture(std::string const &name,
								std::vector<std::string> const &files,
								Texture::t_tex_type type);
		Texture const		&getTexture(std::string const &name);
		void				delete_all_textures(void);
        void				add_model(std::string const &name,
                                std::string const &path);
        Model const		    &getModel(std::string const &name);
        void				delete_all_models(void);

	class ShaderNotFoundException : public GeneralException
	{
		public :

			explicit ShaderNotFoundException(void);
			explicit ShaderNotFoundException(std::string const &name);
			virtual ~ShaderNotFoundException(void) throw();
	};

	class TextureNotFoundException : public GeneralException
	{
		public :

			explicit TextureNotFoundException(void);
			explicit TextureNotFoundException(std::string const &name);
			virtual ~TextureNotFoundException(void) throw();
	};

    class ModelNotFoundException : public GeneralException
    {
        public :

            explicit ModelNotFoundException(void);
            explicit ModelNotFoundException(std::string const &name);
            virtual ~ModelNotFoundException(void) throw();
    };

    class oGLFailException : public GeneralException
	{
		public :

			explicit oGLFailException(void);
			virtual ~oGLFailException(void) throw();
	};

	private :

		std::vector<Shader>			_shader_list;
		std::vector<Texture>		_texture_list;
        std::vector<Model>          _model_list;

		oGL_module(oGL_module const &src);
		oGL_module		&operator=(oGL_module const &rhs);
};

#endif
