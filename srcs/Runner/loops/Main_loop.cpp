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
						e_main_loop_option &option)
{
	static float type_delay = 0.0f;

	if (enabled_gamepad)
	{
		if (manager.getGamepad().isGamepadConnected(GLFW_JOYSTICK_1))
		{
			manager.getGamepad().pollGamepads();
			if (type_delay > 0.25f && option.trigger_pause &&
				(manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).buttons[GLFW_GAMEPAD_BUTTON_BACK]
				 == GLFW_PRESS))
			{
				option.trigger_reset = !option.trigger_reset;
				type_delay = 0.0f;
			}
			if (type_delay > 0.25f &&
				(manager.getGamepad().getGamepadState(GLFW_JOYSTICK_1).buttons[GLFW_GAMEPAD_BUTTON_START]
				 == GLFW_PRESS))
			{
				option.trigger_pause = !option.trigger_pause;
				type_delay = 0.0f;
			}
		}
	}
	else
	{
		if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_END]))
		{
			option.trigger_reset = !option.trigger_reset;
			type_delay = 0.0f;
		}
		if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_HOME]))
		{
			option.trigger_pause = !option.trigger_pause;
			type_delay = 0.0f;
		}
	}
	if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_PAGE_UP]))
	{
		option.trigger_grayscale = !option.trigger_grayscale;
		type_delay = 0.0f;
	}
	if (type_delay < 2.0f)
		type_delay += tick_rate;
}

static void changeFBSurfaceShader(TextureShaderSurface *tss, oGL_module &oGL)
{
	if (!tss->getShader()->getName().compare("texture_window"))
	{
		tss->setShader(&oGL.getShader("texture_window_grayscale"));
		return;
	}
	tss->setShader(&oGL.getShader("texture_window"));
}

static void blockFramerate(Glfw_manager &manager)
{
	static double old_time = 0.0;

	if (old_time == 0.0)
		old_time = manager.getTime();
	while (manager.getTime() - old_time < (1.0 / BLOCK_FRAME_RATE));
	old_time = manager.getTime();
}

bool main_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui, oGL_module &oGL, bool vsync)
{
	e_main_loop_option                    options;
	std::unique_ptr<TextureShaderSurface> fb_surface = nullptr;

	std::memset(&options, 0, sizeof(e_main_loop_option));
	try
	{
		fb_surface = std::make_unique<TextureShaderSurface>(&manager.getWindow(),
															&manager.getInput(),
															&oGL.getShader("texture_window"),
															oGL.getFramebuffer("render").getTextureColorBuffer());
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl << "Error allocating Surface" << std::endl;
		return (false);
	}
	world.playPlayerTheme();
	manager.reset_fps_counter();
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				world_input(world.isGamepadEnabled(), manager, world.getTickRate(), options);
				if (!options.trigger_pause)
					world.update();
				else if (options.trigger_pause && manager.getWindow().resized)
				{
					world.updatePerspective(world.getFov());
					world.updateMatrix();
				}
				ui.update();
			}
			if (!world.isPlayerAlive() || options.trigger_reset)
			{
				world.updateHighScore();
				world.stopPlayerTheme();
				world.deletePlayer();
				return (false);
			}
			if (options.trigger_grayscale)
				changeFBSurfaceShader(fb_surface.get(), oGL);

			//Rending World into a custom framebuffer
			oGL.getFramebuffer("render").useFramebuffer();
			oGL_module::oGL_enable_depth();
			oGL_module::oGL_clear_buffer(0.2f, 0.2f, 0.2f);
			world.render();
			oGL_module::oGL_disable_depth();

			//Displaying world into default framebuffer
			oGL.getFramebuffer("render").defaultFramebuffer();
			oGL_module::oGL_clear_buffer(0.2f, 0.2f, 0.2f);
			oGL_module::oGL_update_viewport(manager.getWindow().max_win_w,
											manager.getWindow().max_win_h);
			fb_surface->draw();

			//Displaying text into default framebuffer
			manager.calculate_fps();
			oGL_module::oGL_update_viewport(manager.getWindow().cur_win_w,
											manager.getWindow().cur_win_h);
			ui.drawText("roboto", "42Run : " + manager.getStrFps() + " fps",
						glm::vec3(0.4f, 0.4f, 0.4f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 40.0f,
								  0.5f));
			ui.drawText("roboto", "High Score : " + world.getStrHighScore(),
						glm::vec3(0.0f, 1.0f, 0.0f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 80.0f,
								  0.5f));
			ui.drawText("roboto", "Score : " + world.getStrCurrentScore(),
						glm::vec3(0.0f, 0.0f, 1.0f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 120.0f,
								  0.5f));
			ui.drawText("roboto", "Speed : " + world.getStrSpeed(),
						glm::vec3(0.8f, 0.1f, 0.1f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 160.0f,
								  0.5f));
			if (options.trigger_pause)
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
			if (vsync)
				blockFramerate(manager);
			manager.swap_buffers();
			options.trigger_grayscale = false;
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	world.stopPlayerTheme();
	return (true);
}