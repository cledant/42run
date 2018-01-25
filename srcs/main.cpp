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
//	run_debug_world(manager);
	if (argc >= 2 && std::string(argv[1]).compare("--Vsync=on") == 0)
		run_runner_world(manager, true);
	else
		run_runner_world(manager, false);
	std::cout << "Close manager" << std::endl;
	Glfw_manager::close_manager();
	return (0);
}
