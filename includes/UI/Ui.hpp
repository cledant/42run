/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ui.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 10:20:32 by cledant           #+#    #+#             */
/*   Updated: 2017/11/11 10:20:32 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_HPP
# define UI_HPP

# include "glad/glad.h"
# include "WindowManager/Window.hpp"
# include "UI/FontSet.hpp"
# include "UI/ShaderSurface/ShaderSurface.hpp"
# include "WindowManager/Input.hpp"
# include <vector>
# include <string>

class Ui
{
	public :

		Ui(GLFW_Window const &win, Input const &input);
		virtual ~Ui(void);
		Ui(Ui const &src) = delete;
		Ui &operator=(Ui const &rhs) = delete;

		void update(void);
		void addFontSet(Shader const *shader, std::string const &name,
						std::string const &font_path, size_t size);

		/*
		 * Getter
		 */

		FontSet const &getFontSet(std::string const &name) const;

		/*
		 * Draw
		 */

		void drawText(std::string const &fontset_name, std::string const &str,
					  glm::vec3 const &color, glm::vec3 const &pos_scale) const;

		class FontSetNotFoundException : public GeneralException
		{
			public :

				explicit FontSetNotFoundException(void);
				virtual ~FontSetNotFoundException(void) throw();
		};

	private :

		GLFW_Window const    &_win;
		Input const          &_input;
		std::vector<FontSet> _fset_list;
		glm::mat4            _proj_matrix;
};

#endif