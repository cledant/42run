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
#include "Runner/TemplatedRoom.hpp"

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
	oGL.add_shader("title_screen", "./shaders/title_screen/title_screen.vs",
				   "./shaders/title_screen/title_screen.fs");
	oGL.add_model("TestBox", Cubemap::vertices, Cubemap::nb_faces,
				  {"./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png", "./textures/testTex/testTex.png"},
				  Texture::TEX_CUBE, Texture::TEX_DIFFUSE);
	oGL.add_model("Skybox", Cubemap::vertices, Cubemap::nb_faces,
				  {"./textures/skybox/right.jpg", "./textures/skybox/left.jpg", "./textures/skybox/top.jpg", "./textures/skybox/bottom.jpg", "./textures/skybox/back.jpg", "./textures/skybox/front.jpg"},
				  Texture::TEX_CUBE, Texture::TEX_DIFFUSE);
	oGL.add_model("cola", "./models/cola/cola.obj");
	oGL.add_model("Sakuya", "./models/Sakuya/Sakuya_Izayoi.obj");
	oGL.add_model("cola_machine", "./models/cola_machine/cola_machine.3ds");
	oGL.add_texture("sprite_reimu",
					{"./textures/sprites/reimu.png"},
					Texture::TEX_FLAT, Texture::TEX_DIFFUSE);
	oGL.add_texture("sprite_marisa",
					{"./textures/sprites/marisa.png"},
					Texture::TEX_FLAT, Texture::TEX_DIFFUSE);
}

static void load_runner(Glfw_manager &manager, oGL_module &oGL,
						RunnerWorld **world)
{
	Room::Params room_params;

	(*world) = new RunnerWorld(manager.getInput(), manager.getWindow(), manager.getGamepad(),
							   glm::vec3(0.0f, 0.0f, 10.0f), 60.0f, 10, load_score("./highscore"));
	NormalRoomEmpty(**world, oGL);
	NormalRoomBonusOnly(**world, oGL);
	NormalRoomObstacleOnly(**world, oGL);
	NormalRoomBonusAndObstacle(**world, oGL);
	FallRightRoomEmpty(**world, oGL);
	FallRightRoomObstacle(**world, oGL);
	FallLeftRoomEmpty(**world, oGL);
	FallLeftRoomObstacle(**world, oGL);
	FallFrontRoomEmpty(**world, oGL);
	FallFrontRoomObstacle(**world, oGL);
	FallMiddleRoomEmpty(**world, oGL);
	FallMiddleRoomObstacle(**world, oGL);
	(*world)->initRoomList();
	(*world)->generateInitialRoomList();
//	(*world)->generateDebug(DEBUG_FORCE_ROOM, DEBUG_FORCE_PROP);
}

static void init_audio(Audio &audio)
{
	audio.loadSound("damage", "./sounds/effects/damage.wav", false, EFFECT_VOLUME);
	audio.loadSound("jump", "./sounds/effects/jump.wav", false, EFFECT_VOLUME);
	audio.loadSound("bonus", "./sounds/effects/bonus.wav", false, EFFECT_VOLUME);
	audio.loadSound("malus", "./sounds/effects/malus.wav", false, EFFECT_VOLUME);
	audio.loadTheme("./sounds/musics/reimu_theme.wav", Audio::THEME_1, true, THEME_VOLUME);
	audio.loadTheme("./sounds/musics/marisa_theme.wav", Audio::THEME_2, true, THEME_VOLUME);
	audio.loadTheme("./sounds/musics/title_screen.wav", Audio::THEME_3, true, THEME_VOLUME);
	audio.loadTheme("./sounds/musics/game_over.wav", Audio::THEME_4, true, THEME_VOLUME);
}

static void init_program(RunnerWorld **world, oGL_module &oGL, Glfw_manager &manager,
						 Ui **ui, Audio &audio)
{
	manager.create_resizable_window("42Run", 4, 1, 1000, 1000);
	manager.init_input_callback();
	init_oGL(oGL);
	init_audio(audio);
	Gamepad::updateGamepadMapping("./ControllerConfigFiles/gamecontrollerdb_205.txt");
	(*ui) = new Ui(manager.getWindow(), manager.getInput());
	(*ui)->addFontSet(&(oGL.getShader("fontset")), "roboto",
					  "./fonts/Roboto-Light.ttf", 60);
	load_runner(manager, oGL, world);
}

static int cleanup(RunnerWorld *world, Ui *ui)
{
	oGL_module::oGL_finish();
	save_score("./highscore", world->getHighScore());
	std::cout << "Delete Ui" << std::endl;
	delete ui;
	std::cout << "Delete world" << std::endl;
	delete world;
	return (1);
}

int run_runner_world(Glfw_manager &manager, bool vsync, bool force_keyboard)
{
	oGL_module  oGL;
	Audio       audio;
	Ui          *ui    = nullptr;
	RunnerWorld *world = nullptr;
	bool        loop   = true;

	try
	{
		init_program(&world, oGL, manager, &ui, audio);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		return (0);
	}
	if (vsync)
		manager.enableVsync();
	world->reset_update_timer(Glfw_manager::getTime());
	if (force_keyboard)
		world->forceKeyboard();
	manager.reset_fps_counter();
	if (!title_screen_loop(*world, manager, *ui, oGL, audio))
		return (cleanup(world, ui));
	while (loop)
	{
		if (!char_selection_loop(*world, manager, *ui, oGL, audio))
			return (cleanup(world, ui));
		if (!main_loop(*world, manager, *ui))
			loop = game_over_loop(*world, manager, *ui, oGL, audio);
		else
			return (cleanup(world, ui));
		world->updateLastGameScore();
	}
	return (cleanup(world, ui));
}
