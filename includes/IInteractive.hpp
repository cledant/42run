/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IInteractive.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/13 10:16:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/20 17:32:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IINTERACTIVE_HPP
# define IINTERACTIVE_HPP

# include "glm/glm.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include "Gamepad.hpp"

class IInteractive
{
	public :

		IInteractive(void);
		virtual ~IInteractive(void);
		IInteractive(IInteractive const &src) = delete;
		IInteractive &operator=(IInteractive const &rhs) = delete;

		virtual bool update_keyboard_interaction(Input const &input,
												 float input_timer) = 0;
		virtual bool update_mouse_interaction(Input const &input,
											  GLFW_Window const &win, glm::vec3 const &origin,
											  std::vector<glm::vec3 const *> const &axes,
											  float input_timer) = 0;
		virtual bool update_gamepad_interaction(GamepadState const &state,
												float input_timer) = 0;
};

#endif
