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

# include "Glfw_manager.hpp"
# include "World.hpp"
# include "RunnerWorld.hpp"
# include "Ui.hpp"

void run_debug_world(Glfw_manager &manager);
void run_runner_world(Glfw_manager &manager);

#endif
