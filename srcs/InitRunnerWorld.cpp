/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugWorld.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WorldSelect.hpp"


static void main_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui)
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
			ui.drawText("roboto", "Score : " + world.getScore(),
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
	oGL_module::oGL_finish();
}

static void init_oGL(oGL_module &oGL)
{
	oGL_module::oGL_enable_depth();
	oGL.add_shader("simple_box", "./shaders/simple_box/simple_box.vs",
				   "./shaders/simple_box/simple_box.fs");
	oGL.add_shader("cubemap", "./shaders/cubemap/cubemap.vs",
				   "./shaders/cubemap/cubemap.fs");
	oGL.add_shader("prop", "./shaders/prop/prop.vs",
				   "./shaders/prop/prop.fs");
	oGL.add_shader("fontset", "./shaders/fontset/fontset.vs",
				   "./shaders/fontset/fontset.fs");
	oGL.add_shader("sprites", "./shaders/sprites/sprites.vs",
				   "./shaders/sprites/sprites.fs");
	oGL.add_texture("TestTex",
					{"./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png"},
					Texture::TEX_CUBE, Texture::TEX_DIFFUSE);
	oGL.add_model("cola", "./models/cola/cola.obj");
	oGL.add_texture("sprite_reimu",
					{"./textures/sprites/reimu.png"},
					Texture::TEX_FLAT, Texture::TEX_DIFFUSE);
	oGL.add_texture("sprite_marisa",
					{"./textures/sprites/marisa.png"},
					Texture::TEX_FLAT, Texture::TEX_DIFFUSE);
}

static void set_player_params(Player::Params &params, oGL_module &oGL, Audio &audio)
{
	params.cb_shader              = &(oGL.getShader("cubemap"));
	params.shader                 = &(oGL.getShader("sprites"));
	params.pos                    = glm::vec3({0.0f, 1.0f, 0.0f});
	params.size                   = glm::vec3({0.1f, 0.2f, 0.1f});
	params.cb_tex                 = &(oGL.getTexture("TestTex"));
	params.tex                    = &(oGL.getTexture("sprite_reimu"));
	params.sprite_tex_size_offset = glm::vec4(0.093f, 0.125f, 0.0f, 0.0f);
	params.nb_walk_frame          = 4;
	params.draw_cb                = false;
	params.max_jump               = 2;
	params.max_hoover_time        = 2.0f;
	params.audio                  = &audio;
	params.theme                  = Audio::THEME_1;
}

static void load_runner(Glfw_manager &manager, oGL_module &oGL,
						RunnerWorld **world, Audio &audio)
{
	Player::Params player_params;
	Room::Params   room_params;
	Room           *room;

	(*world) = new RunnerWorld(manager.getInput(), manager.getWindow(), manager.getGamepad(),
							   glm::vec3(0.0f, 0.0f, 10.0f), 60.0f, 10);

	set_player_params(player_params, oGL, audio);
	(*world)->add_Player(player_params);

	room_params.floor.shader      = &(oGL.getShader("cubemap"));
	room_params.floor.tex         = &(oGL.getTexture("TestTex"));
	room_params.roof.shader       = &(oGL.getShader("cubemap"));
	room_params.roof.tex          = &(oGL.getTexture("TestTex"));
	room_params.right_wall.shader = &(oGL.getShader("cubemap"));
	room_params.right_wall.tex    = &(oGL.getTexture("TestTex"));
	room_params.left_wall.shader  = &(oGL.getShader("cubemap"));
	room_params.left_wall.tex     = &(oGL.getTexture("TestTex"));
	room_params.front_wall.shader = &(oGL.getShader("cubemap"));
	room_params.front_wall.tex    = &(oGL.getTexture("TestTex"));
	room_params.front_wall.active = false;
	room = dynamic_cast<Room *>((*world)->add_Room(room_params));
	room->scaleObject(glm::vec3(3.0f, 3.0f, 3.0f));
	room = dynamic_cast<Room *>((*world)->add_Room(room_params));
	room->scaleObject(glm::vec3(3.0f, 3.0f, 3.0f));
	room->translateObject(glm::vec3(6.6f, 0.0f, 0.0f));
	room = dynamic_cast<Room *>((*world)->add_Room(room_params));
	room->scaleObject(glm::vec3(3.0f, 3.0f, 3.0f));
	room->translateObject(glm::vec3(13.2f, 0.0f, 0.0f));
	room_params.front_wall.active = true;
	room = dynamic_cast<Room *>((*world)->add_Room(room_params));
	room->scaleObject(glm::vec3(3.0f, 3.0f, 3.0f));
	room->translateObject(glm::vec3(19.8f, 0.0f, 0.0f));
}

static void init_audio(Audio &audio)
{
	audio.loadSound("damage", "./sounds/effects/damage.wav", false, EFFECT_VOLUME);
	audio.loadSound("jump", "./sounds/effects/jump.wav", false, EFFECT_VOLUME);
	audio.loadSound("bonus", "./sounds/effects/bonus.wav", false, EFFECT_VOLUME);
	audio.loadSound("malus", "./sounds/effects/malus.wav", false, EFFECT_VOLUME);
	audio.loadTheme("./sounds/musics/reimu_theme.wav", Audio::THEME_1, true, THEME_VOLUME);
	audio.loadTheme("./sounds/musics/marisa_theme.wav", Audio::THEME_2, true, THEME_VOLUME);
}

static void init_program(RunnerWorld **world, oGL_module &oGL, Glfw_manager &manager,
						 Ui **ui, Audio &audio)
{
	manager.create_resizable_window("42Run", 4, 1, 1000, 1000);
	manager.init_input_callback();
	init_oGL(oGL);
	init_audio(audio);
	Gamepad::updateGamepadMapping("./ControllerConfigFiles/gamecontrollerdb_205.txt");
	(*ui) = new Ui(manager.getWindow());
	(*ui)->addFontSet(&(oGL.getShader("fontset")), "roboto",
					  "./fonts/Roboto-Light.ttf", 60);
	load_runner(manager, oGL, world, audio);
}

void run_runner_world(Glfw_manager &manager)
{

	oGL_module  oGL;
	Audio       audio;
	Ui          *ui    = nullptr;
	RunnerWorld *world = nullptr;

	try
	{
		init_program(&world, oGL, manager, &ui, audio);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		return;
	}
	world->reset_update_timer(Glfw_manager::getTime());
	manager.reset_fps_counter();
	main_loop(*world, manager, *ui);
	std::cout << "Delete Ui" << std::endl;
	delete ui;
	std::cout << "Delete world" << std::endl;
	delete world;
}
