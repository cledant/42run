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
# include "Simple_box.hpp"
# include "Cubemap.hpp"
# include "Prop.hpp"
# include "CollisionBox.hpp"
# include "Shader.hpp"
# include "Camera.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include <iostream>
# include <vector>
# include <fstream>

class World
{
	public :

		World(Input const &input, GLFW_Window const &win, glm::vec3 cam_pos,
			  float max_fps, size_t max_frame_skip);
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
						  glm::vec3 const &scale);
		void setActiveInteractive(IInteractive *ptr);
		void updatePerspective(float fov);
		void reset_update_timer(float time);
		void reset_skip_loop(void);
		bool should_be_updated(float time);

		class WorldFailException : public GeneralException
		{
			public :

				explicit WorldFailException(void);
				virtual ~WorldFailException(void) throw();
		};

		/*
		 *  Set of functions for collision unit_test
		 */
		CollisionBox *debug_add_cbox(Shader const *shader, glm::vec3 const &pos,
									 glm::vec3 const &min, glm::vec3 const &max,
									 std::string const &name);
		CollisionBox *debug_add_target(Shader const *shader, glm::vec3 const &pos,
									   glm::vec3 const &min, glm::vec3 const &max,
									   std::string const &name);
		void debug_checkCollision(void) const;
		void debug_collision_test_1(Shader const *shader);
		void debug_update(void);
		void debug_render(void);


	private :

		std::vector<IEntity *>      _entity_list;
		IInteractive                *_active;
		Input const                 &_input;
		GLFW_Window const           &_window;
		glm::mat4                   _perspective;
		Camera                      _camera;
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

		/*
		 * Set of variables for collision unit_test
		 */
		CollisionBox                *_debug_target;
		std::vector<CollisionBox *> _debug_collision;
};

#endif
