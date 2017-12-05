/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThirdPersonCamera.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:13:19 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:47:48 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THIRDPERSONCAMERA_HPP
# define THIRDPERSONCAMERA_HPP

# include "Input.hpp"
# include "Gamepad.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "Camera.hpp"
# include <iostream>

# define THIRD_CAM_GAMEPAD_SENSITIVITY 15.0f

class ThirdPersonCamera : public Camera
{
	public :

		ThirdPersonCamera(Input const &input, Gamepad const &gamepad,
						  glm::vec3 const &target_pos, float dist_to_target,
						  glm::vec3 const &world_up, glm::vec3 const &front,
						  GLfloat yaw, GLfloat pitch);
		virtual ~ThirdPersonCamera(void);
		ThirdPersonCamera(const ThirdPersonCamera &src) = delete;
		ThirdPersonCamera &operator=(ThirdPersonCamera const &rhs) = delete;

		void setDistToTarget(float dist);

		void update_third_person(bool mouse_exclusive_to_manager, glm::vec3 const &target_pos,
								 bool gamepad);

		class ThirdPersonCameraFailException : public GeneralException
		{
			public :

				explicit ThirdPersonCameraFailException(void);
				virtual ~ThirdPersonCameraFailException(void) throw();
		};

	protected :

		float         _dist_to_target;
		Gamepad const &_gamepad;

	private :

		virtual void _update_from_keyboard_input(void);
		virtual void _update_from_mouse_input(void);
		virtual void _update_from_gamepad_input(void);
};

#endif
