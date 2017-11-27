/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 15:03:35 by cledant           #+#    #+#             */
/*   Updated: 2017/11/15 17:30:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# define GLM_ENABLE_EXPERIMENTAL

# include "oGL_module.hpp"
# include "Cubemap.hpp"
# include "Sprite.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/gtx/norm.hpp"
# include "IEntity.hpp"
# include "IInteractive.hpp"
# include "ICollidable.hpp"
# include "IGravity.hpp"
# include "ThirdPersonCamera.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class Player : public IInteractive, public IEntity, public ICollidable,
			   public IGravity
{
	public :

		typedef enum e_player_dir
		{
			BACK        = 0,
			BACK_RIGHT  = 1,
			RIGHT       = 2,
			FRONT_RIGHT = 3,
			FRONT       = 4,
			FRONT_LEFT  = 5,
			LEFT        = 6,
			BACK_LEFT   = 7,
		} t_player_dir;

		struct Params
		{
			Params(void);
			~Params(void);

			Shader const            *cb_shader;
			Shader const            *shader;
			glm::mat4 const         *perspec_mult_view;
			glm::vec3               pos;
			glm::vec3               size;
			ThirdPersonCamera const *cam;
			Texture const           *cb_tex;
			Texture const           *tex;
			glm::vec4               sprite_tex_size_offset;
			size_t                  nb_walk_frame;
			bool                    draw_cb;
			size_t                  max_jump;
			float                   max_hoover_time;
			size_t                  hp;
			float                   max_immunity;
		};

		Player(Player::Params const &params);
		virtual ~Player(void);
		Player(const Player &src) = delete;
		Player &operator=(const Player &rhs) = delete;

		void setPos(glm::vec3 const &new_pos);
		void setDelta(glm::vec3 const &new_delta);
		void setSurfaceCollisionBox(CollisionBox const &cb);
		void setOnSurface(bool flag);
		void setVelocity(glm::vec3 const &vel);
		void setSpriteYaw(float yaw);
		void setCurJumpToMax(void);
		void setCurHooverTimeToMax(void);
		void lowerHP(ICollidable::Damages type);

		glm::vec3 const &getDelta(void) const;
		glm::vec3 const &getPos(void) const;
		bool getOnSurface(void) const;
		size_t getMaxJump(void) const;
		bool getCanHoover(void) const;
		float getMaxHooverTime(void) const;
		int getHP(void) const;
		bool isAlive(void) const;

		void update_model(float time);

		/*
		 * Interface ICollidable
		 */
		CollisionBox const &getCollisionBox(void) const;
		ICollidable::Damages getDamages(void) const;

		/*
 		* Interface IInteractive
 		*/
		bool update_keyboard_interaction(Input const &input,
										 float input_timer);
		bool update_mouse_interaction(Input const &input,
									  GLFW_Window const &win, glm::vec3 const &origin,
									  std::vector<glm::vec3 const *> const &axes,
									  float input_timer);

		/*
		 * Interface IEntity
		 */
		void update(float time);
		void draw(void);

		/*
		 * Interface IGravity
		 */
		void update_gravity(glm::vec3 const &vec_gravity, float delta);

	private :

		ThirdPersonCamera const *_cam;
		Sprite                  _model;
		Cubemap                 _cb_model;
		CollisionBox            _cb;
		glm::vec3               _delta;
		glm::vec3               _pos;
		glm::vec3               _vel;
		glm::vec3               _acc;
		bool                    _on_surface;
		CollisionBox            _surface_cb;
		bool                    _delay_jump;
		float                   _friction;
		float                   _force;
		bool                    _draw_cb;
		t_player_dir            _dir;
		glm::ivec2              _axis;
		float                   _total_walked;
		size_t                  _cur_jump;
		size_t                  _max_jump;
		bool                    _hoover;
		float                   _cur_hoover_time;
		float                   _max_hoover_time;
		int                     _hp;
		float                   _cur_immunity;
		float                   _max_immunity;

		void _set_sprite_direction(void);
};

#endif