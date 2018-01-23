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

#include "Utility/WorldSelect.hpp"


static void main_loop(World &world, Glfw_manager &manager, Ui &ui)
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
	oGL.add_model("TestBox", Cubemap::vertices, Cubemap::nb_faces,
				  {"./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png"},
				  Texture::TEX_CUBE, Texture::TEX_DIFFUSE);
	oGL.add_model("Skybox", Cubemap::vertices, Cubemap::nb_faces,
				  {"./textures/skybox/right.jpg", "./textures/skybox/left.jpg", "./textures/skybox/top.jpg", "./textures/skybox/bottom.jpg", "./textures/skybox/back.jpg", "./textures/skybox/front.jpg"},
				  Texture::TEX_CUBE, Texture::TEX_DIFFUSE);
	oGL.add_model("Alice", "./models/Alice/Alice.obj");
	oGL.add_model("Sakuya", "./models/Sakuya/Sakuya_Izayoi.obj");
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
	params.pos                    = glm::vec3({0.0f, 15.0f, 0.0f});
	params.size                   = glm::vec3({0.1f, 0.2f, 0.1f});
	params.cb_model               = &(oGL.getModel("TestBox"));
	params.tex                    = &(oGL.getTexture("sprite_marisa"));
	params.sprite_tex_size_offset = glm::vec4(0.093f, 0.125f, 0.0f, 0.0f);
	params.nb_walk_frame          = 4;
	params.draw_cb                = false;
	params.max_jump               = 2;
	params.max_hoover_time        = 2.0f;
	params.audio                  = &audio;
	params.theme                  = Audio::THEME_2;
}

static void load_debug_level(Glfw_manager &manager, oGL_module &oGL,
							 World **world, Audio &audio)
{
	Player::Params         player_params;
	CollidableProp::Params bonus_params;
	CollidableProp::Params damage_box_params;
	CollidableBox::Params  box_1;
	CollidableBox::Params  box_2;
	CollidableBox::Params  box_3;
	CollidableBox::Params  box_4;
	Room::Params           room_params;
	Room                   *room;

	(*world) = new World(manager.getInput(), manager.getWindow(), manager.getGamepad(),
						 glm::vec3(0.0f, 0.0f, 10.0f), 60.0f, 10);
	(*world)->add_Cubemap(&(oGL.getShader("cubemap")), &(oGL.getModel("Skybox")),
						  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f));
	(*world)->add_Simple_box(&(oGL.getShader("simple_box")),
							 glm::vec3(10.0f, 3.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	set_player_params(player_params, oGL, audio);
	(*world)->add_Player(player_params);

	box_1.shader = &(oGL.getShader("cubemap"));
	box_1.model  = &(oGL.getModel("TestBox"));
	box_1.pos    = glm::vec3({0.0f, 0.0f, 0.0f});
	box_1.size   = glm::vec3({50.0f, 0.2f, 50.0f});
	(*world)->add_CollidableBox(box_1);

	box_2.shader = &(oGL.getShader("cubemap"));
	box_2.model  = &(oGL.getModel("TestBox"));
	box_2.pos    = glm::vec3({0.0f, 13.0f, 0.0f});
	box_2.size   = glm::vec3({1.0f, 1.0f, 1.0f});
	(*world)->add_CollidableBox(box_2);

	box_3.shader         = &(oGL.getShader("cubemap"));
	box_3.model          = &(oGL.getModel("TestBox"));
	box_3.pos            = glm::vec3({0.0f, 0.0f, 5.0f});
	box_3.size           = glm::vec3({1.0f, 1.0f, 0.2f});
	box_3.passthrough    = true;
	box_3.pick_up        = std::string("malus");
	box_3.score_modifier = -500;
	(*world)->add_CollidableBox(box_3);

	box_4.shader = &(oGL.getShader("cubemap"));
	box_4.model  = &(oGL.getModel("TestBox"));
	box_4.pos    = glm::vec3({0.0f, 0.0f, 2.0f});
	box_4.size   = glm::vec3({1.0f, 1.0f, 0.2f});
	(*world)->add_CollidableBox(box_4);

	damage_box_params.prop_params.shader = &(oGL.getShader("prop"));
	damage_box_params.prop_params.model  = &(oGL.getModel("Sakuya"));
	damage_box_params.prop_params.pos    = glm::vec3(-5.0f, 0.7f, 0.0f);
	damage_box_params.prop_params.scale  = glm::vec3(0.005f, 0.005f, 0.005f);
	damage_box_params.prop_params.offset = glm::vec3(0.0f, 0.1f, 0.0f);
	damage_box_params.cb                 = CollisionBox(damage_box_params.prop_params.pos,
														glm::vec3(0.4f, 0.35f, 0.1f));
	damage_box_params.dmg                = ICollidable::Damages::HALF;
	(*world)->add_CollidableProp(damage_box_params);

	bonus_params.prop_params.shader      = &(oGL.getShader("prop"));
	bonus_params.prop_params.model       = &(oGL.getModel("cola"));
	bonus_params.prop_params.pos         = glm::vec3(0.0f, 0.5f, 0.0f);
	bonus_params.prop_params.scale       = glm::vec3(0.015f, 0.015f, 0.015f);
	bonus_params.prop_params.orientation = glm::vec3(0.0f, 20.0f, 0.0f);
	bonus_params.cb                      = CollisionBox(bonus_params.prop_params.pos,
														glm::vec3(0.07f, 0.07f, 0.07f));
	bonus_params.dmg                     = ICollidable::Damages::NONE;
	bonus_params.pick_up                 = std::string("bonus");
	bonus_params.passthrough             = true;
	bonus_params.score_modifier          = 1000;
	bonus_params.auto_rotation           = true;
	(*world)->add_CollidableProp(bonus_params);

	room_params.floor.shader      = &(oGL.getShader("cubemap"));
	room_params.floor.model       = &(oGL.getModel("TestBox"));
	room_params.roof.shader       = &(oGL.getShader("cubemap"));
	room_params.roof.model        = &(oGL.getModel("TestBox"));
	room_params.right_wall.shader = &(oGL.getShader("cubemap"));
	room_params.right_wall.model  = &(oGL.getModel("TestBox"));
	room_params.left_wall.shader  = &(oGL.getShader("cubemap"));
	room_params.left_wall.model   = &(oGL.getModel("TestBox"));
	room_params.front_wall.shader = &(oGL.getShader("cubemap"));
	room_params.front_wall.model  = &(oGL.getModel("TestBox"));
	room = dynamic_cast<Room *>((*world)->add_Room(room_params));
	room->translateObject(glm::vec3(4.0f, 2.0f, 2.0f));
	room->scaleObject(glm::vec3(1.0f, 1.0f, 1.0f));
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

static void init_program(World **world, oGL_module &oGL, Glfw_manager &manager,
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
	load_debug_level(manager, oGL, world, audio);
}

void run_debug_world(Glfw_manager &manager)
{

	oGL_module oGL;
	Audio      audio;
	Ui         *ui    = nullptr;
	World      *world = nullptr;

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
