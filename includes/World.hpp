/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   World.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/09/23 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_HPP
# define WORLD_HPP

# include "oGL_module.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "IEntity.hpp"
# include "IInteractive.hpp"
# include "ICollidable.hpp"
# include "Simple_box.hpp"
# include "Cubemap.hpp"
# include "Prop.hpp"
# include "CollisionBox.hpp"
# include "Shader.hpp"
# include "ThirdPersonCamera.hpp"
# include "Player.hpp"
# include "CollidableBox.hpp"
# include "CollidableProp.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class World
{
	public :

		World(Input const &input, GLFW_Window const &win, Gamepad &gamepad,
			  glm::vec3 cam_pos, float max_fps, size_t max_frame_skip);
		virtual ~World(void);
		World(World const &src) = delete;
		World &operator=(World const &rhs) = delete;

		void update(void);
		void render(void);
		IEntity *add_Simple_box(Shader const *shader, glm::vec3 const &pos,
								glm::vec3 const &scale);
		IEntity *add_Cubemap(Shader const *shader,
							 std::vector<std::string> const &files,
							 glm::vec3 const &pos, glm::vec3 const &scale);
		IEntity *add_Prop(Shader const *shader, Model const *model,
						  glm::vec3 const &pos, glm::vec3 const &orientation,
						  glm::vec3 const &scale, glm::vec3 const &offset);
		IInteractive *add_Player(Player::Params &params);
		IEntity *add_CollidableBox(Shader const *shader, glm::vec3 const &pos,
								   glm::vec3 const &size, Texture const *tex,
								   ICollidable::Damages dmg, bool passthrough,
								   int score_modifier);
		IEntity *add_CollidableProp(Shader const *shader, Model const *model,
									glm::vec3 const &pos, glm::vec3 const &orientation,
									glm::vec3 const &scale, glm::vec3 const &offset,
									glm::vec3 const &half_size, ICollidable::Damages dmg,
									bool passthrough, int score_modifier);
		void setActiveInteractive(IInteractive *ptr);
		void updatePerspective(float fov);
		void reset_update_timer(float time);
		void reset_skip_loop(void);
		bool should_be_updated(float time);

		std::string const &getScore(void);
		std::string const &getStrPlayerHP(void);

		class WorldFailException : public GeneralException
		{
			public :

				explicit WorldFailException(void);
				virtual ~WorldFailException(void) throw();
		};

	private :

		std::vector<IEntity *>     _entity_list;
		std::vector<ICollidable *> _collision_check_list;
		IInteractive               *_active;
		Input const                &_input;
		GLFW_Window const          &_window;
		Gamepad                    &_gamepad;
		glm::mat4                  _perspective;
		ThirdPersonCamera          _camera;
		glm::mat4                  _perspec_mult_view;
		float                      _fov;
		float                      _max_fps;
		size_t                     _max_frame_skip;
		float                      _tick;
		float                      _next_update_tick;
		float                      _last_update_tick;
		float                      _delta_tick;
		size_t                     _skip_loop;
		float                      _input_timer;
		float                      _input_mouse_timer;
		glm::vec3                  _gravity;
		std::string                _str_hp;
		std::string                _str_score;
		bool                       _first_run_theme;
		bool                       _enabled_gamepad;

		void _check_collisions(void);
		void _resolve_sweep_collision(Player *player, CollisionBox const &box,
									  CollisionBox::SweepResolution const &res,
									  ICollidable::Damages dmg_taked);
};

#endif
