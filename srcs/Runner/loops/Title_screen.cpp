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

bool title_screen_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui,
					   oGL_module &oGL, Audio &audio)
{
	std::unique_ptr<ShaderSurface> title_shader = nullptr;
	bool                           validate     = false;
	int                            selection    = 0;

	try
	{
		title_shader = std::make_unique<ShaderSurface>(&manager.getWindow(),
													   &manager.getInput(),
													   &oGL.getShader("title_screen"));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl << "Exiting 42Run" << std::endl;
		return (false);
	}
	audio.playTheme(Audio::THEME_3);
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			loop_input(world.isGamepadEnabled(), manager, world.getTickRate(),
					   validate, selection);
			if (validate)
				return (true);
			oGL_module::oGL_clear_buffer(0.0f, 0.0f, 0.0f);
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				world.update();
			}
			title_shader->draw();
			ui.update();
			ui.drawText("roboto", "42 Run",
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 85.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 100.0f,
								  1.0f));
			ui.drawText("roboto", "Press enter/cross to start",
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2 - 330.f),
								  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
								  1.0f));
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	return (false);
}