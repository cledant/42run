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
# include <random>

# define RUNNER_FOV_MIN 0.1f
# define RUNNER_FOV_MAX 40.0f
# define DEBUG_FORCE_ROOM 6
# define MAX_STUCK_FRAME 10

class RunnerWorld
{
	public :

		RunnerWorld(Input const &input, GLFW_Window const &win, Gamepad &gamepad,
					glm::vec3 cam_pos, float max_fps, size_t max_frame_skip,
					long int high_score);
		virtual ~RunnerWorld(void);
		RunnerWorld(RunnerWorld const &src) = delete;
		RunnerWorld &operator=(RunnerWorld const &rhs) = delete;

		void update(void);
		void render(void);
		Room *addRoomTemplate(std::string const &name, Room::Params &params);
		IEntity *add_CollidableBox(CollidableBox::Params &params, std::string const &name);
		void addCollidableToRoomTemplate(std::string const &room_name,
										 std::string const &slot_name,
										 CollidableProp::Params &params);
		IInteractive *add_Player(Player::Params &params);
		void deletePlayer(void);
		void updatePerspective(float fov);
		void reset_update_timer(float time);
		void reset_skip_loop(void);
		bool should_be_updated(float time);
		void updateLastGameScore(void);
		void updateHighScore(void);

		/*
		 * Room generation
		 */

		void initRoomList(void);
		void generateInitialRoomList(size_t nb);
		void generateBeginEndRoomList(size_t nb);
		void generateMiddleRoomList(size_t nb);
		void generateDebug(size_t nb, size_t room_type, bool has_prop);

		/*
		 * Getter
		 */

		std::string const &getStrCurrentScore(void);
		std::string const &getStrLastScore(void);
		std::string const &getStrHighScore(void);
		long int getCurrentScore(void);
		long int getLastGameScore(void);
		long int getHighScore(void);
		std::string const &getStrPlayerHP(void);
		bool getShouldEnd(void) const;
		float getTickRate(void) const;
		bool isPlayerAlive(void) const;

		/*
		 * Setter
		 */

		void setActiveInteractive(IInteractive *ptr);
		void resetInputTimer(void);
		void setLaps(size_t laps);

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

		std::map<std::string, Room>          _room_template_list;
		std::vector<Room *>                  _room_list;
		IInteractive                         *_active;
		Input const                          &_input;
		GLFW_Window const                    &_window;
		Gamepad                              &_gamepad;
		glm::mat4                            _perspective;
		ThirdPersonCamera                    _camera;
		glm::mat4                            _perspec_mult_view;
		float                                _fov;
		float                                _max_fps;
		size_t                               _max_frame_skip;
		float                                _tick;
		float                                _next_update_tick;
		float                                _last_update_tick;
		float                                _delta_tick;
		size_t                               _skip_loop;
		float                                _input_timer;
		float                                _input_mouse_timer;
		glm::vec3                            _gravity;
		std::string                          _str_hp;
		std::string                          _str_score;
		std::string                          _str_last_score;
		std::string                          _str_high_score;
		long int                             _score_modifier;
		bool                                 _first_run_theme;
		bool                                 _enabled_gamepad;
		bool                                 _should_end;
		long int                             _current_score;
		long int                             _last_game_score;
		long int                             _high_score;
		std::map<std::string, CollidableBox> _list_collidable_box;
		std::random_device                   _rd;
		size_t                               _laps;
		std::vector<float>                   _check_stuck;

		inline void _check_collisions(void);
		inline void _resolve_sweep_collision(Player *player, CollisionBox const &box,
											 CollisionBox::SweepResolution const &res,
											 ICollidable const *ptr);
		inline void _check_collidable_box(CollidableBox const &cb,
										  CollisionBox::SweepResolution *nearest,
										  glm::vec3 const &inv_delta,
										  ICollidable const **ptr);
		inline void _add_pos_for_check(glm::vec3 const &pos);

		static size_t list_size;
		static size_t nb_prop;
};

#endif
