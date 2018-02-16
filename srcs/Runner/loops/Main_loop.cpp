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
						bool &trigger_pause, int &char_type)
{
	static float type_delay = 0.0f;

	if (enabled_gamepad)
	{
		if (manager.getGamepad().isGamepadConnected(GLFW_JOYSTICK_1))
		{
			manager.getGamepad().pollGamepads();
			if (type_delay > 0.25f &&
				((manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).axes[GLFW_GAMEPAD_AXIS_LEFT_Y]
				  >= MOV_DEAD_ZONE) ||
				 (manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).axes[GLFW_GAMEPAD_AXIS_LEFT_Y]
				  <= -MOV_DEAD_ZONE)))
			{
				char_type = (char_type + 1) % 2;
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
		if (type_delay > 0.25f && ((manager.getInput().p_key[GLFW_KEY_RIGHT] ||
									manager.getInput().p_key[GLFW_KEY_LEFT])))
		{
			char_type  = (char_type + 1) % 2;
			type_delay = 0.0f;
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
	bool       trigger_pause = false;
	static int selection     = 0;

	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				world_input(world.isGamepadEnabled(), manager, world.getTickRate(),
							trigger_pause, selection);
				if (!trigger_pause)
					world.update();
			}
			if (!world.isPlayerAlive())
			{
				world.updateHighScore();
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
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	return (true);
}