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

#include "Utility/WorldSelect.hpp"

int title_screen_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui,
					  oGL_module &oGL)
{
	std::unique_ptr<ShaderSurface> title_shader;

	try
	{
		title_shader = std::make_unique<ShaderSurface>(&manager.getWindow(),
													   &manager.getInput(),
													   &oGL.getShader("title_screen"));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl << "Exiting 42Run" << std::endl;
		return (0);
	}
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			if (manager.getInput().p_key[GLFW_KEY_SPACE])
				return (1);
			oGL_module::oGL_clear_buffer(0.2f, 0.3f, 0.3f);
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
				manager.update_events();
			title_shader->draw();
			ui.update();
			ui.drawText("roboto", "42 Run",
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 85.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 100.0f,
								  1.0f));
			ui.drawText("roboto", "Press space to start",
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2 - 250.f),
								  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
								  1.0f));
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	return (1);
}