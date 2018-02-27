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
# include "UI/ShaderSurface/TextureShaderSurface.hpp"

# define BLOCK_FRAME_RATE 59.95

/*
 * Inits
 */

void run_debug_world(Glfw_manager &manager, bool vsync, bool force_keyboard);
int run_runner_world(Glfw_manager &manager, bool vsync, bool force_keyboard);

/*
 * Runner game loops
 */

bool char_selection_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui,
						 oGL_module &oGL, Audio &audio);
bool title_screen_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui,
					   oGL_module &oGL, Audio &audio);
bool game_over_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui,
					oGL_module &oGL, Audio &audio);
bool main_loop(RunnerWorld &world, Glfw_manager &manager, Ui &ui, oGL_module &oGL,
			   bool vsync);
void loop_input(bool enabled_gamepad, Glfw_manager &manager, float tick_rate,
				bool &trigger_selection, int &char_type);

/*
 * Score
 */

long int load_score(std::string const &path);
void save_score(std::string const &path, long int to_save);

#endif
