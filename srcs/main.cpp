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

#include "WindowManager/Glfw_manager.hpp"
#include "Utility/WorldSelect.hpp"

static void parse_args(int argc, char **argv, Glfw_manager &manager)
{
	bool vsync          = false;
	int  type           = 0;
	bool force_keyboard = false;

	for (int i = 1; i < argc; ++i)
	{
		if (!std::string(argv[i]).compare("--vsync"))
			vsync = true;
		else if (!std::string(argv[i]).compare("--debugLevel"))
			type = 1;
		else if (!std::string(argv[i]).compare("--forceKeyboard"))
			force_keyboard = true;
	}
	return ((type == 1) ? run_debug_world(manager, vsync, force_keyboard) :
			(void) run_runner_world(manager, vsync, force_keyboard));
}

int main(int argc, char **argv)
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
	parse_args(argc, argv, manager);
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
	return (0);
}
