/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RunnerWorld.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNERWORLD_HPP
# define RUNNERWORLD_HPP

# include "oGL_module.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "IEntity.hpp"
# include "IInteractive.hpp"
# include "ICollidable.hpp"
# include "CollisionBox.hpp"
# include "Shader.hpp"
# include "ThirdPersonCamera.hpp"
# include "Player.hpp"
# include "CollidableBox.hpp"
# include "CollidableProp.hpp"
# include "Room.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class RunnerWorld
{
	public :

		RunnerWorld(Input const &input, GLFW_Window const &win, Gamepad &gamepad,
					glm::vec3 cam_pos, float max_fps, size_t max_frame_skip);
		virtual ~RunnerWorld(void);
		RunnerWorld(RunnerWorld const &src) = delete;
		RunnerWorld &operator=(RunnerWorld const &rhs) = delete;

		void update(void);
		void render(void);
		IEntity *add_Room(Room::Params &params);
		IInteractive *add_Player(Player::Params &params);
		void setActiveInteractive(IInteractive *ptr);
		void updatePerspective(float fov);
		void reset_update_timer(float time);
		void reset_skip_loop(void);
		bool should_be_updated(float time);

		/*
		 * Getter
		 */
		std::string const &getScore(void);
		std::string const &getStrPlayerHP(void);
		bool getShouldEnd(void) const;

		class RunnerWorldFailException : public GeneralException
		{
			public :

				explicit RunnerWorldFailException(void);
				virtual ~RunnerWorldFailException(void) throw();
		};

	private :

		std::vector<IEntity *> _entity_list;
		std::vector<Room *>    _room_list;
		IInteractive           *_active;
		Input const            &_input;
		GLFW_Window const      &_window;
		Gamepad                &_gamepad;
		glm::mat4              _perspective;
		ThirdPersonCamera      _camera;
		glm::mat4              _perspec_mult_view;
		float                  _fov;
		float                  _max_fps;
		size_t                 _max_frame_skip;
		float                  _tick;
		float                  _next_update_tick;
		float                  _last_update_tick;
		float                  _delta_tick;
		size_t                 _skip_loop;
		float                  _input_timer;
		float                  _input_mouse_timer;
		glm::vec3              _gravity;
		std::string            _str_hp;
		std::string            _str_score;
		long int               _score_modifier;
		bool                   _first_run_theme;
		bool                   _enabled_gamepad;
		bool                   _should_end;

		void _check_collisions(void);
		void _resolve_sweep_collision(Player *player, CollisionBox const &box,
									  CollisionBox::SweepResolution const &res,
									  ICollidable *ptr);
};

#endif
