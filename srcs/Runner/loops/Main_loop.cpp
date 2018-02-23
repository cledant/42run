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

static void world_input(bool enabled_gamepad, Glfw_manager &manager, float tick_rate,
						bool &trigger_pause, bool &reset_trigger)
{
	static float type_delay = 0.0f;

	if (enabled_gamepad)
	{
		if (manager.getGamepad().isGamepadConnected(GLFW_JOYSTICK_1))
		{
			manager.getGamepad().pollGamepads();
			if (type_delay > 0.25f && trigger_pause &&
				(manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).buttons[GLFW_GAMEPAD_BUTTON_BACK]
				 == GLFW_PRESS))
			{
				reset_trigger = !reset_trigger;
				type_delay    = 0.0f;
			}
			if (type_delay > 0.25f &&
				(manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).buttons[GLFW_GAMEPAD_BUTTON_START]
				 == GLFW_PRESS))
			{
				trigger_pause = !trigger_pause;
				type_delay    = 0.0f;
			}
			if (type_delay < 2.0f)
				type_delay += tick_rate;
		}
	}
	else
	{
		if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_END]))
		{
			reset_trigger = !reset_trigger;
			type_delay    = 0.0f;
		}
		if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_HOME]))
		{
			trigger_pause = !trigger_pause;
			type_delay    = 0.0f;
		}
		if (type_delay < 2.0f)
			type_delay += tick_rate;
	}
}

bool main_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui)
{
	bool trigger_pause = false;
	bool trigger_reset = false;

	world.playPlayerTheme();
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				world_input(world.isGamepadEnabled(), manager, world.getTickRate(),
							trigger_pause, trigger_reset);
				if (!trigger_pause)
					world.update();
				else if (trigger_pause && manager.getWindow().resized)
				{
					world.updatePerspective(world.getFov());
					world.updateMatrix();
				}
			}
			if (!world.isPlayerAlive() || trigger_reset)
			{
				world.updateHighScore();
				world.stopPlayerTheme();
				world.deletePlayer();
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
						glm::vec3(1.0f, 0.4f, 0.4f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 80.0f,
								  0.5f));
			ui.drawText("roboto", "High Score : " + world.getStrHighScore(),
						glm::vec3(0.0f, 1.0f, 0.0f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 120.0f,
								  0.5f));
			ui.drawText("roboto", "Score : " + world.getStrCurrentScore(),
						glm::vec3(0.0f, 0.0f, 1.0f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 160.0f,
								  0.5f));
			ui.drawText("roboto", "Speed : " + world.getStrSpeed(),
						glm::vec3(0.8f, 0.1f, 0.1f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 200.0f,
								  0.5f));
			if (trigger_pause)
			{
				ui.drawText("roboto", "Pause",
							glm::vec3(1.0f, 0.0f, 0.0f),
							glm::vec3(static_cast<float>(manager.getWindow().cur_win_w) / 2 - 125.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) / 2,
									  1.5f));
				ui.drawText("roboto", "Press End / Select to start again",
							glm::vec3(0.9f, 0.9f, 0.9f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 300.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) / 2 - 70.0f,
									  0.7f));
			}
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	world.stopPlayerTheme();
	return (true);
}