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

static int title_screen_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui, oGL_module &oGL)
{
	std::unique_ptr<ShaderSurface> title_shader;

	try
	{
		title_shader = std::make_unique<ShaderSurface>(&manager.getWindow(), &manager.getInput(),
													   &oGL.getShader("title_screen"));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl << "Exiting 42Run" << std::endl;
		return (0);
	}
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			if (manager.getInput().p_key[GLFW_KEY_SPACE])
				return (1);
			oGL_module::oGL_clear_buffer(0.2f, 0.3f, 0.3f);
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()))
				manager.update_events();
			title_shader->draw();
			ui.update();
			ui.drawText("roboto", "42 Run",
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2) - 85.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 100.0f,
								  1.0f));
			ui.drawText("roboto", "Press space to start",
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3((static_cast<float>(manager.getWindow().cur_win_w) / 2 - 250.f),
								  static_cast<float>(manager.getWindow().cur_win_h) * 0.1,
								  1.0f));
			manager.swap_buffers();
			if (world.getShouldEnd())
				manager.triggerWindowClose();
			if (manager.should_window_be_closed())
				manager.destroy_window();
		}
	}
	return (1);
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
							   glm::vec3(0.0f, 0.0f, 10.0f), 60.0f, 10);
	NormalRoomEmpty(**world, oGL);
	NormalRoomBonusOnly(**world, oGL);
	NormalRoomObstacleOnly(**world, oGL);
	NormalRoomBonusAndObstacle(**world, oGL);
	(*world)->initRoomList();
	(*world)->generateRoomListNorth();
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
	(*ui) = new Ui(manager.getWindow(), manager.getInput());
	(*ui)->addFontSet(&(oGL.getShader("fontset")), "roboto",
					  "./fonts/Roboto-Light.ttf", 60);
	load_runner(manager, oGL, world);
}

void run_runner_world(Glfw_manager &manager, bool vsync)
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
	if (vsync)
		manager.enableVsync();
	world->reset_update_timer(Glfw_manager::getTime());
	manager.reset_fps_counter();
	if (title_screen_loop(*world, manager, *ui, oGL))
	{
		if (char_selection_loop(*world, manager, *ui, oGL, audio))
			main_loop(*world, manager, *ui);
	}
	std::cout << "Delete Ui" << std::endl;
	delete ui;
	std::cout << "Delete world" << std::endl;
	delete world;
}
