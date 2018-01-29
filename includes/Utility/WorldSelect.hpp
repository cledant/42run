/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WorldSelect.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLDSELECT_HPP
# define WORLDSELECT_HPP

# include "WindowManager/Glfw_manager.hpp"
# include "Platformer/World.hpp"
# include "Runner/RunnerWorld.hpp"
# include "UI/Ui.hpp"

void run_debug_world(Glfw_manager &manager, bool vsync);
void run_runner_world(Glfw_manager &manager, bool vsync);
int char_selection_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui,
						oGL_module &oGL, Audio &audio);

#endif
