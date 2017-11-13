/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glfw_manager.hpp"
#include "World.hpp"
#include "Ui.hpp"

static void main_loop(World &world, Glfw_manager &manager, Ui &ui)
{
	while (Glfw_manager::getActiveWindowNumber())
	{
		if (manager.getWindow().win != nullptr)
		{
			world.reset_skip_loop();
			while (world.should_be_updated(Glfw_manager::getTime()) == true)
			{
				manager.update_events();
				world.update();
				world.debug_update();
				world.debug_checkCollision();
			}
			manager.calculate_fps();
			world.render();
			world.debug_render();
			ui.update();
			ui.drawText("roboto", "42Run : " + manager.getStrFps() + " fps",
						glm::vec3(0.4f, 0.4f, 0.4f),
						glm::vec3(10.0f,
								  static_cast<float>(manager.getWindow().cur_win_h) - 40.0f,
								  0.5f));
			manager.swap_buffers();
			if (manager.should_window_be_closed() == true)
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
	oGL.add_model("Alice", "./models/Alice/Alice.obj");
	oGL.add_model("Sakuya", "./models/Sakuya/Sakuya_Izayoi.obj");
}

static void init_program(World **world, oGL_module &oGL,
						 Glfw_manager &manager, Ui **ui)
{
	std::vector<std::string> const skybox_files
										   {
												   "./textures/skybox/right.jpg",
												   "./textures/skybox/left.jpg",
												   "./textures/skybox/top.jpg",
												   "./textures/skybox/bottom.jpg",
												   "./textures/skybox/back.jpg",
												   "./textures/skybox/front.jpg",
										   };

	manager.create_resizable_window("42Run", 4, 1, 1000, 1000);
	manager.init_input_callback();
	init_oGL(oGL);
	(*world) = new World(manager.getInput(), manager.getWindow(),
						 glm::vec3(0.0f, 0.0f, 10.0f), 60.0f, 10);
	(*ui)    = new Ui(manager.getWindow());
	(*world)->add_Cubemap(&(oGL.getShader("cubemap")), skybox_files,
						  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f));
/*	(*world)->add_Simple_box(&(oGL.getShader("simple_box")),
							 glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	(*world)->add_Prop(&(oGL.getShader("prop")), &(oGL.getModel("Alice")),
					   glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3({0.0f, 0.0f, 0.0f}),
					   glm::vec3(0.05f, 0.05f, 0.05f));
	(*world)->add_Prop(&(oGL.getShader("prop")), &(oGL.getModel("Sakuya")),
					   glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3({0.0f, 0.0f, 0.0f}),
					   glm::vec3(0.05f, 0.05f, 0.05f));*/
	(*ui)->addFontSet(&(oGL.getShader("fontset")), "roboto",
					  "./fonts/Roboto-Light.ttf", 60);
	(*world)->debug_collision_test_1(&(oGL.getShader("cubemap")));
}

static void run_program(Glfw_manager &manager)
{

	oGL_module oGL;
	Ui         *ui    = nullptr;
	World      *world = nullptr;

	try
	{
		init_program(&world, oGL, manager, &ui);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		return;
	}
	world->reset_update_timer(Glfw_manager::getTime());
	manager.reset_fps_counter();
	manager.toogle_mouse_exclusive();
	main_loop(*world, manager, *ui);
	std::cout << "Delete Ui" << std::endl;
	delete ui;
	std::cout << "Delete world" << std::endl;
	delete world;
}

int main(void)
{
	Glfw_manager manager;

	try
	{
		Glfw_manager::run_manager();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}
	run_program(manager);
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
	return (0);
}
