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
#include "FontSet.hpp"

static void main_loop(World &world, Glfw_manager &manager, FontSet &font)
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
				manager.calculate_and_display_fps();
			}
			world.render();
			font.drawText("Hey : This Is A Test!", {0.5f, 0.8f, 0.2f},
						  {25.0f, 25.0f, 0.5f});
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
						 Glfw_manager &manager, FontSet **font)
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
	(*world)->add_Cubemap(&(oGL.getShader("cubemap")), skybox_files,
						  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f));
	(*world)->add_Simple_box(&(oGL.getShader("simple_box")),
							 glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	(*world)->add_Prop(&(oGL.getShader("prop")), &(oGL.getModel("Alice")),
					   glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3({0.0f, 0.0f, 0.0f}),
					   glm::vec3(0.05f, 0.05f, 0.05f));
	(*world)->add_Prop(&(oGL.getShader("prop")), &(oGL.getModel("Sakuya")),
					   glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3({0.0f, 0.0f, 0.0f}),
					   glm::vec3(0.05f, 0.05f, 0.05f));
	(*font) = new FontSet(&(oGL.getShader("fontset")), "./fonts/Roboto-Light.ttf",
						  manager.getWindow().cur_win_w, manager.getWindow().cur_win_h);
}

static void run_program(Glfw_manager &manager)
{

	oGL_module oGL;
	World      *world = nullptr;
	FontSet    *font  = nullptr;

	try
	{
		init_program(&world, oGL, manager, &font);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		delete world;
		delete font;
		return;
	}
	world->reset_update_timer(Glfw_manager::getTime());
	manager.reset_fps_counter();
	manager.toogle_mouse_exclusive();
	main_loop(*world, manager, *font);
	std::cout << "Delete world" << std::endl;
	delete world;
	std::cout << "Delete font" << std::endl;
	delete font;
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
