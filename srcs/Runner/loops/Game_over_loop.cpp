/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game_over_loop.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility/WorldSelect.hpp"

bool game_over_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui, oGL_module &oGL)
{
	int   selection_type    = 0;
	float type_delay        = 0.0f;
	bool  trigger_selection = false;

	static_cast<void>(oGL);
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			oGL_module::oGL_clear_buffer(0.0f, 0.0f, 0.0f);
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				if (type_delay > 0.25f && ((manager.getInput().p_key[GLFW_KEY_UP] ||
											manager.getInput().p_key[GLFW_KEY_DOWN])))
				{
					selection_type = (selection_type + 1) % 2;
					type_delay     = 0.0f;
				}
				if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_SPACE]))
					trigger_selection = true;
				if (type_delay < 2.0f)
					type_delay += world.getTickRate();
			}
			ui.update();
			ui.drawText("roboto", "Game Over !",
						glm::vec3(1.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 180.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) * 0.8,
								  1.0f));
			if (!selection_type)
			{
				ui.drawText("roboto", "Retry",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 90.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.2,
									  1.0f));
				ui.drawText("roboto", "Quit",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 90.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
									  0.5f));
			}
			else
			{
				ui.drawText("roboto", "Retry",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 90.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.2,
									  0.5f));
				ui.drawText("roboto", "Quit",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 90.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
									  1.0f));
			}
			manager.swap_buffers();
			if (trigger_selection && !selection_type)
				return (true);
			else if (trigger_selection)
				return (false);
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	return (true);
}