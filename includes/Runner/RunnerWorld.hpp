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

# include "OpenGL/oGL_module.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "Interfaces/IEntity.hpp"
# include "Interfaces/IInteractive.hpp"
# include "Interfaces/ICollidable.hpp"
# include "Utility/CollisionBox.hpp"
# include "OpenGL/Shader.hpp"
# include "Cameras/ThirdPersonCamera.hpp"
# include "GameEntities/Player.hpp"
# include "GameEntities/CollidableBox.hpp"
# include "GameEntities/CollidableProp.hpp"
# include "GameEntities/Room.hpp"
# include "WindowManager/Window.hpp"
# include "WindowManager/Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class RunnerWorld
{
	public :

		enum Direction
		{
			NORTH,
			EAST,
			WEST
		};

		RunnerWorld(Input const &input, GLFW_Window const &win, Gamepad &gamepad,
					glm::vec3 cam_pos, float max_fps, size_t max_frame_skip);
		virtual ~RunnerWorld(void);
		RunnerWorld(RunnerWorld const &src) = delete;
		RunnerWorld &operator=(RunnerWorld const &rhs) = delete;

		void update(void);
		void render(void);
		Room &addRoomTemplate(std::string const &name, Room::Params &params);
		void addBonusToRoomTemplate(std::string const &room_name,
									std::string const &slot_name,
									CollidableProp::Params &params);
		void addObstacleToRoomTemplate(std::string const &room_name,
									   std::string const &slot_name,
									   CollidableProp::Params &params);
		IInteractive *add_Player(Player::Params &params);
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

		/*
		 * Setter
		 */

		void setActiveInteractive(IInteractive *ptr);
		void setActiveRoom(enum RunnerWorld::Direction dir);

		class RunnerWorldFailException : public GeneralException
		{
			public :

				explicit RunnerWorldFailException(void);
				virtual ~RunnerWorldFailException(void) throw();
		};

		class RoomNotFoundException : public GeneralException
		{
			public :

				explicit RoomNotFoundException(void);
				virtual ~RoomNotFoundException(void) throw();
		};

	private :

		std::map<std::string, Room> _room_template_list;
		std::vector<Room *>         _room_list_north;
		std::vector<Room *>         _room_list_east;
		std::vector<Room *>         _room_list_west;
		std::vector<Room *>         *_active_room;
		IInteractive                *_active;
		Input const                 &_input;
		GLFW_Window const           &_window;
		Gamepad                     &_gamepad;
		glm::mat4                   _perspective;
		ThirdPersonCamera           _camera;
		glm::mat4                   _perspec_mult_view;
		float                       _fov;
		float                       _max_fps;
		size_t                      _max_frame_skip;
		float                       _tick;
		float                       _next_update_tick;
		float                       _last_update_tick;
		float                       _delta_tick;
		size_t                      _skip_loop;
		float                       _input_timer;
		float                       _input_mouse_timer;
		glm::vec3                   _gravity;
		std::string                 _str_hp;
		std::string                 _str_score;
		long int                    _score_modifier;
		bool                        _first_run_theme;
		bool                        _enabled_gamepad;
		bool                        _should_end;

		void _check_collisions(void);
		void _resolve_sweep_collision(Player *player, CollisionBox const &box,
									  CollisionBox::SweepResolution const &res,
									  ICollidable *ptr);
		void _check_collidable_box(CollidableBox &cb,
								   CollisionBox::SweepResolution *nearest,
								   glm::vec3 const &inv_delta,
								   ICollidable **ptr);
};

#endif
