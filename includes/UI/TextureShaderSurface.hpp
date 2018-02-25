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

#ifndef TEXTURESHADERSURFACE_HPP
# define TEXTURESHADERSURFACE_HPP

# include "UI/ShaderSurface.hpp"

class TextueShaderSurface : public ShaderSurface
{
	public :

		TextueShaderSurface(GLFW_Window const *win, Input const *input, Shader const *shader,
							GLuint tex_id);
		virtual ~TextueShaderSurface(void);
		TextueShaderSurface(TextueShaderSurface const &src) = delete;
		TextueShaderSurface &operator=(TextueShaderSurface const &rhs) = delete;
		TextueShaderSurface(TextueShaderSurface &&src);
		TextueShaderSurface &operator=(TextueShaderSurface &&rhs);

		/*
		 * Getter
		 */

		GLuint getTextureID(void) const;

		/*
		 * Setter
		 */

		void setTextureID(void) const;

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void draw(void);

	protected :

		GLuint _tex_id;

		virtual void _allocate_buffer(void);

		static float  _tex_vertices[];
		static size_t _tex_nb_faces;
};

#endif