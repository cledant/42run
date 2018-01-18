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

# include "OpenGL/oGL_module.hpp"
# include "Cubemap.hpp"
# include "Sprite.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/gtx/norm.hpp"
# include "Interfaces/IEntity.hpp"
# include "Interfaces/IInteractive.hpp"
# include "Interfaces/ICollidable.hpp"
# include "Interfaces/IGravity.hpp"
# include "Cameras/ThirdPersonCamera.hpp"
# include "WindowManager/Window.hpp"
# include "WindowManager/Input.hpp"
# include "Audio/Audio.hpp"
# include <iostream>
# include <vector>
# include <fstream>
# include <string>

# define BLINK_CD 3
# define THEME_VOLUME 100.0f
# define EFFECT_VOLUME 75.0f
# define INPUT_REPEAT_TIMER 1.0f
# define DOUBLE_JUMP_FRAME 2
# define DOUBLE_JUMP_REPEAT_TIMER (0.01667f * DOUBLE_JUMP_FRAME)

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
			int                     hp;
			float                   max_immunity;
			Audio                   *audio;
			Audio::theme_list       theme;
			std::string             pick_up;
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
		void setVelocityYtoZero(void);
		void setVelocityXZtoZero(void);
		void setSpriteYaw(float yaw);
		void setCurJumpToMax(void);
		void setCurHooverTimeToMax(void);
		void lowerHP(ICollidable::Damages type);
		void setImmunityTimerToMax(void);
		void setScale(glm::vec3 const &scale);

		glm::vec3 const &getDelta(void) const;
		glm::vec3 const &getPos(void) const;
		bool getOnSurface(void) const;
		size_t getMaxJump(void) const;
		bool getCanHoover(void) const;
		float getMaxHooverTime(void) const;
		float getTotalWalked(void) const;
		int getHP(void) const;
		bool isAlive(void) const;
		bool isImmune(void) const;
		void playSound(std::string const &name);
		void playTheme(Audio::theme_list slot);
		void playSetTheme(void);
		void stopSetTheme(void);

		void update_model(float time);

		/*
		 * Interface ICollidable
		 */

		void setPassthrough(bool value);
		void setActive(bool value);

		CollisionBox const &getCollisionBox(void) const;
		Damages getDamages(void) const;
		int getScoreModifier(void) const;
		bool getPassthrough(void) const;
		bool getActive(void) const;
		std::string const &getPickUpSound(void) const;

		/*
 		* Interface IInteractive
 		*/
		bool update_keyboard_interaction(Input const &input,
										 float input_timer);
		bool update_mouse_interaction(Input const &input,
									  GLFW_Window const &win, glm::vec3 const &origin,
									  std::vector<glm::vec3 const *> const &axes,
									  float input_timer);
		bool update_gamepad_interaction(GamepadState const &state, float input_timer);

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
		bool                    _display_sprite;
		Audio                   *_audio;
		Audio::theme_list       _theme;
		bool                    _last_jump;
		std::string             _pick_up;

		void _set_sprite_direction(void);
};

#endif