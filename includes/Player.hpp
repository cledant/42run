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

# include "oGL_module.hpp"
# include "Cubemap.hpp"
# include "Sprite.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
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

		Player(Shader const *cb_shader, Shader const *shader,
			   glm::mat4 const *perspec_mult_view,
			   glm::vec3 const &pos, glm::vec3 const &size,
			   ThirdPersonCamera const *cam, Texture const *cb_tex,
			   Texture const *tex, glm::ivec2 sprite, bool draw_cb);
		virtual ~Player(void);
		Player(const Player &src) = delete;
		Player &operator=(const Player &rhs) = delete;

		void setPos(glm::vec3 const &new_pos);
		void setDelta(glm::vec3 const &new_delta);
		void setSurfaceCollisionBox(CollisionBox const &cb);
		void setOnSurface(bool flag);
		void setVelocity(glm::vec3 const &vel);

		glm::vec3 const &getDelta(void) const;
		glm::vec3 const &getPos(void) const;
		bool getOnSurface(void) const;

		void update_model(float time);

		/*
		 * Interface ICollidable
		 */
		CollisionBox const &getCollisionBox(void) const;

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
};

#endif