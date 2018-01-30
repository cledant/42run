/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_loop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility/WorldSelect.hpp"

bool main_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui)
{
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				world.update();
			}
			if (!world.isPlayerAlive())
			{
				world.updateHighScore();
				return (false);
			}
			manager.calculate_fps();
			world.render();
			ui.update();
			ui.drawText("roboto", "42Run : " + manager.getStrFps() + " fps",
						glm::vec3(0.4f, 0.4f, 0.4f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 40.0f,
								  0.5f));
			ui.drawText("roboto", "Player Life : " + world.getStrPlayerHP(),
						glm::vec3(0.4f, 0.4f, 0.4f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 80.0f,
								  0.5f));
			ui.drawText("roboto", "Score : " + world.getStrCurrentScore(),
						glm::vec3(0.4f, 0.4f, 0.4f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 120.0f,
								  0.5f));
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	return (true);
}