/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop_input.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility/WorldSelect.hpp"

void loop_input(bool enabled_gamepad, Glfw_manager &manager, float tick_rate,
				bool &trigger_selection, int &char_type)
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
				char_type  = (char_type + 1) % 2;
				type_delay = 0.0f;
			}
			if (type_delay > 0.25f &&
				(manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).buttons[GLFW_GAMEPAD_BUTTON_CROSS]
				 == GLFW_PRESS))
			{
				trigger_selection = true;
				type_delay        = 0.0f;
				return;
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
		if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_ENTER]))
		{
			trigger_selection = true;
			type_delay        = 0.0f;
			return;
		}
		if (type_delay < 2.0f)
			type_delay += tick_rate;
	}
}