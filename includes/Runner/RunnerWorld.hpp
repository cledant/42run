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
# include "GameEntities/utility/CollisionBox.hpp"
# include "OpenGL/Shader.hpp"
# include "Cameras/ThirdPersonCamera.hpp"
# include "GameEntities/composed/Player.hpp"
# include "GameEntities/composed/CollidableBox.hpp"
# include "GameEntities/composed/CollidableProp.hpp"
# include "GameEntities/composed/Room.hpp"
# include "UI/OrientableShaderSurface.hpp"
# include "WindowManager/Window.hpp"
# include "WindowManager/Input.hpp"
# include <iostream>
# include <iomanip>
# include <vector>
# include <fstream>
# include <random>

# define RUNNER_FOV_MIN 0.1f
# define RUNNER_FOV_MAX 40.0f
# define DEBUG_FORCE_ROOM 2
# define DEBUG_FORCE_PROP true
# define MAX_STUCK_FRAME 15
# define INITIAL_MAX_SPAWN_CHANCE 100
# define PLAYER_SHADOW_HEIGHT -2.81f
# define CAMERA_HEIGHT -2.0f

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
		IEntity *add_PlayerShadow(OrientableShaderSurface::Params &params);
		void deletePlayer(void);
		void deletePlayerShadow(void);
		void updatePerspective(float fov);
		void updateMatrix(void);
		void reset_update_timer(float time);
		void reset_skip_loop(void);
		bool should_be_updated(float time);
		void updateLastGameScore(void);
		void updateHighScore(void);
		void playPlayerTheme(void);
		void stopPlayerTheme(void);

		/*
		 * Room generation
		 */

		void initRoomList(void);
		void generateInitialRoomList(void);
		void generateBeginEndRoomList(void);
		void generateMiddleRoomList(void);
		void generateDebug(size_t room_type, bool has_prop);

		/*
		 * Getter
		 */

		std::string const &getStrCurrentScore(void);
		std::string const &getStrLastScore(void);
		std::string const &getStrHighScore(void);
		std::string const &getStrSpeed(void);
		long int getCurrentScore(void) const;
		long int getLastGameScore(void) const;
		long int getHighScore(void) const;
		std::string const &getStrPlayerHP(void);
		bool getShouldEnd(void) const;
		float getTickRate(void) const;
		bool isPlayerAlive(void) const;
		bool isGamepadEnabled(void) const;
		float getFov(void) const;

		/*
		 * Setter
		 */

		void setActiveInteractive(IInteractive *ptr);
		void resetInputTimer(void);
		void setLaps(size_t laps);
		void forceKeyboard(void);

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
		IEntity                              *_active_shadow;
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
		std::string                          _str_speed;
		long int                             _score_modifier;
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
		inline bool _should_spawn_full_obstacle(void);

		static size_t list_size;
};

#endif
