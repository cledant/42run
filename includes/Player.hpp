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
# include "CollisionBox.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "IEntity.hpp"
# include "IInteractive.hpp"
# include "ThirdPersonCamera.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class Player : public IInteractive, public IEntity
{
	public :

		Player(Shader const *shader, glm::mat4 const *perspec_mult_view,
			   glm::vec3 const &pos, glm::vec3 const &size,
			   ThirdPersonCamera const *cam);
		virtual ~Player();
		Player(const Player &src) = delete;
		Player &operator=(const Player &rhs) = delete;

		glm::vec3 const &getPos(void) const;

		bool update_keyboard_interaction(Input const &input,
										 float input_timer);
		bool update_mouse_interaction(Input const &input,
									  GLFW_Window const &win, glm::vec3 const &origin,
									  std::vector<glm::vec3 const *> const &axes,
									  float input_timer);
		void update(float time);
		void draw(void);

	private :

		ThirdPersonCamera const *_cam;
		Cubemap                 _model;
		CollisionBox            _cb;
		glm::vec3               _pos;
		glm::vec3               _vel;
		glm::vec3               _acc;
		float                   _mass;
};

#endif