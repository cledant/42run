/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureShaderSurface.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UI/TextureShaderSurface.hpp"

TextueShaderSurface::TextueShaderSurface(GLFW_Window const *win, Input const *input,
										 Shader const *shader, GLuint tex_id) :
		ShaderSurface(win, input, shader), _tex_id(tex_id)
{
	oGL_module::oGL_delete_vao(this->_vao);
	oGL_module::oGL_delete_vbo(this->_vbo);
	try
	{
		this->_allocate_buffer();
	}
	catch (std::exception &e)
	{
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		throw ShaderSurface::InitException();
	}
	this->update(0.0f);
}