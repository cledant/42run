/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ui.hpp"

Ui::Ui(GLFW_Window const &win) : _win(win)
{
	this->_proj_matrix = glm::ortho(0.0f, static_cast<float>(this->_win.cur_win_w),
									0.0f, static_cast<float>(this->_win.cur_win_h));
}

Ui::~Ui(void)
{
}

void Ui::update(void)
{
	if (this->_win.resized)
	{
		this->_proj_matrix = glm::ortho(0.0f, static_cast<float>(this->_win.cur_win_w),
										0.0f, static_cast<float>(this->_win.cur_win_h));
	}
}

void Ui::addFontSet(Shader const *shader, std::string const &name,
					std::string const &font_path, size_t size)
{
	this->_fset_list.push_back(FontSet(shader, &(this->_proj_matrix), name, font_path, size));
}

FontSet const &Ui::getFontSet(std::string const &name) const
{
	std::vector<FontSet>::const_iterator it;

	for (it = this->_fset_list.begin(); it != this->_fset_list.end(); ++it)
	{
		if (name.compare(it->getName()) == 0)
			return (*it);
	}
	throw Ui::FontSetNotFoundException();
}

void Ui::drawText(std::string const &fontset_name, std::string const &str,
				  glm::vec3 const &color, glm::vec3 const &pos_scale) const
{
	this->getFontSet(fontset_name).drawText(str, color, pos_scale);
}

Ui::FontSetNotFoundException::FontSetNotFoundException(void)
{
	this->_msg = "Ui : FontSet not found";
}

Ui::FontSetNotFoundException::~FontSetNotFoundException(void) throw()
{
}