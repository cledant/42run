/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TemplatedRoom.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEMPLATEDROOM_HPP
# define TEMPLATEDROOM_HPP

# include "Runner/RunnerWorld.hpp"

void NormalRoomEmpty(RunnerWorld &world, oGL_module &oGL);
void NormalRoomBonusOnly(RunnerWorld &world, oGL_module &oGL);
void NormalRoomObstacleOnly(RunnerWorld &world, oGL_module &oGL);
void NormalRoomBonusAndObstacle(RunnerWorld &world, oGL_module &oGL);
void FallRightRoomEmpty(RunnerWorld &world, oGL_module &oGL);
void FallRightRoomObstacle(RunnerWorld &world, oGL_module &oGL);
void FallLeftRoomEmpty(RunnerWorld &world, oGL_module &oGL);
void FallLeftRoomObstacle(RunnerWorld &world, oGL_module &oGL);
void FallFrontRoomEmpty(RunnerWorld &world, oGL_module &oGL);
void FallFrontRoomObstacle(RunnerWorld &world, oGL_module &oGL);

#endif
