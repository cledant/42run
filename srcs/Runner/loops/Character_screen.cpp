/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Character_screen.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility/WorldSelect.hpp"

static void set_player_params(Player::Params &params, oGL_module &oGL, Audio &audio,
							  int player_type)
{
	params.cb_shader              = &(oGL.getShader("cubemap"));
	params.shader                 = &(oGL.getShader("sprites"));
	params.pos                    = glm::vec3({-3.5f + (13.2f * 0), -2.1f, 0.0f});
	params.size                   = glm::vec3({0.3f, 0.5f, 0.3f});
	params.cb_model               = &(oGL.getModel("TestBox"));
	params.tex                    = &(oGL.getTexture("sprite_reimu"));
	params.sprite_tex_size_offset = glm::vec4(0.093f, 0.125f, 0.0f, 0.0f);
	params.nb_walk_frame          = 4;
	params.draw_cb                = false;
	params.audio                  = &audio;
	params.hp                     = 1;
	if (!player_type)
	{
		params.tex             = &(oGL.getTexture("sprite_reimu"));
		params.max_jump        = 1;
		params.max_hoover_time = 2.0f;
		params.theme           = Audio::THEME_1;
	}
	else
	{
		params.tex             = &(oGL.getTexture("sprite_marisa"));
		params.max_jump        = 2;
		params.max_hoover_time = 0.0f;
		params.theme           = Audio::THEME_2;
	}
}

static void set_trigger_tp_params(CollidableBox::Params &params, oGL_module &oGL)
{
	params.shader = &(oGL.getShader("cubemap"));
	params.model  = &(oGL.getModel("TestBox"));
	params.pos    = glm::vec3({13.2f * 10, 0.0f, 0.0f});
	params.size   = glm::vec3({0.1f * 6, 0.9f * 3, 0.9f * 3});
	params.active = true;
}

bool char_selection_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui, oGL_module &oGL,
						 Audio &audio)
{
	std::unique_ptr<Sprite> character_sprites[2];
	Player::Params          player_params;
	int                     char_type         = 0;
	float                   type_delay        = 0.0f;
	glm::mat4               matrix            = glm::mat4(1.0f);
	bool                    trigger_selection = false;
	CollidableBox::Params   trigger_tp_params;

	try
	{
		character_sprites[0] = std::make_unique<Sprite>(&(oGL.getShader("sprites")), &matrix,
														&(oGL.getTexture("sprite_reimu")),
														glm::vec3(0.6f, 0.2f, 0.0f),
														glm::vec3(0.1f, 0.2f, 0.1f),
														glm::vec4(0.093f, 0.125f, 0.0f, 0.0f), 4);
		character_sprites[1] = std::make_unique<Sprite>(&(oGL.getShader("sprites")), &matrix,
														&(oGL.getTexture("sprite_marisa")),
														glm::vec3(-0.6f, 0.2f, 0.0f),
														glm::vec3(0.1f, 0.2f, 0.1f),
														glm::vec4(0.093f, 0.125f, 0.0f, 0.0f), 4);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl << "Exiting 42Run" << std::endl;
		return (false);
	}
	character_sprites[0]->setYaw(90.0f);
	character_sprites[1]->setYaw(90.0f);
	set_trigger_tp_params(trigger_tp_params, oGL);
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			oGL_module::oGL_clear_buffer(0.0f, 0.0f, 0.0f);
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
			{
				manager.update_events();
				world.update();
				if (type_delay > 0.25f && ((manager.getInput().p_key[GLFW_KEY_RIGHT] ||
											manager.getInput().p_key[GLFW_KEY_LEFT])))
				{
					char_type  = (char_type + 1) % 2;
					type_delay = 0.0f;
				}
				if (type_delay > 0.25f && (manager.getInput().p_key[GLFW_KEY_ENTER]))
					trigger_selection = true;
				if (type_delay < 2.0f)
					type_delay += world.getTickRate();
			}
			if (trigger_selection)
				break;
			ui.update();
			character_sprites[(char_type) % 2]->setScale(glm::vec3(0.2f, 0.4f, 0.2f));
			character_sprites[(char_type + 1) % 2]->setScale(glm::vec3(0.1f, 0.2f, 0.1f));
			for (size_t i = 0; i < 2; ++i)
			{
				character_sprites[i]->update(0.0f);
				character_sprites[i]->draw();
			}
			ui.drawText("roboto", "Choose your character !",
						glm::vec3(1.0f, 1.0f, 1.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 250.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 100.0f,
								  0.8f));
			if (char_type)
			{
				ui.drawText("roboto", "Marisa Kirisame",
							glm::vec3(1.0f, 1.0f, 0.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 170.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.3,
									  0.8f));
				ui.drawText("roboto", "Double Jump",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 135.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.2,
									  0.8f));
				ui.drawText("roboto", "Normal fall",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 110.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
									  0.8f));
			}
			else
			{
				ui.drawText("roboto", "Hakurei Reimu",
							glm::vec3(0.863f, 0.078f, 0.235f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 160.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.3,
									  0.8f));
				ui.drawText("roboto", "Single Jump",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 135.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.2,
									  0.8f));
				ui.drawText("roboto", "Slow fall",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 105.0f,
									  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
									  0.8f));
			}
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	set_player_params(player_params, oGL, audio, char_type);
	try
	{
		world.add_Player(player_params);
		world.add_CollidableBox(trigger_tp_params, "tp_trigger");
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl << "Exiting 42Run" << std::endl;
		return (false);
	}
	return (true);
}
