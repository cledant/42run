/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:34:42 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:47:46 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThirdPersonCamera.hpp"

ThirdPersonCamera::ThirdPersonCamera(Input const &input, Gamepad const &gamepad,
									 glm::vec3 const &target_pos, float dist_to_target,
									 glm::vec3 const &world_up, glm::vec3 const &front,
									 GLfloat yaw, GLfloat pitch) :
		Camera(input, glm::vec3({0.0f, 0.0f, 0.0f}), world_up, front, yaw, pitch),
		_gamepad(gamepad)
{
	this->_dist_to_target = (dist_to_target < 0.001f) ? 0.001f : dist_to_target;
	this->update_third_person(true, target_pos,
							  gamepad.isGamepadConnected(GLFW_JOYSTICK_1));
}

ThirdPersonCamera::~ThirdPersonCamera(void)
{
}

void ThirdPersonCamera::setDistToTarget(float dist)
{
	if (dist < 0.001f)
		dist = 0.001f;
	this->_dist_to_target = dist;
}

void ThirdPersonCamera::update_third_person(bool mouse_exclusive_to_manager,
											glm::vec3 const &target_pos,
											bool gamepad)
{
	this->_pos        = target_pos;
	this->_update_cam = mouse_exclusive_to_manager;
	if (!gamepad)
	{
		if (this->_update_cam)
		{
			if (this->_input.mouse_refreshed)
				this->_update_from_mouse_input();
		}
		this->_update_from_keyboard_input();
		this->_view = glm::lookAt(this->_pos - this->_front * this->_dist_to_target,
								  this->_pos + this->_front, this->_up);
		return;
	}
	if (this->_gamepad.isGamepadConnected(GLFW_JOYSTICK_1))
		this->_update_from_gamepad_input();
	this->_view = glm::lookAt(this->_pos - this->_front * this->_dist_to_target, this->_pos + this->_front,
							  this->_up);
}

void ThirdPersonCamera::_update_from_keyboard_input(void)
{
	float velocity            = this->_movement_speed;

	if (this->_input.p_key[GLFW_KEY_E] == PRESSED)
		this->_dist_to_target += velocity;
	if (this->_input.p_key[GLFW_KEY_Q] == PRESSED)
		this->_dist_to_target -= velocity;
	if (this->_dist_to_target < 0.001f)
		this->_dist_to_target = 0.001f;
}

void ThirdPersonCamera::_update_from_mouse_input(void)
{
	this->_yaw += (this->_input.x_offset * this->_mouse_sensitivity);
	this->_pitch += (this->_input.y_offset * this->_mouse_sensitivity);
	if (this->_pitch > 89.0f)
		this->_pitch = 89.0f;
	if (this->_pitch < -89.0f)
		this->_pitch = -89.0f;
	this->_update_vector_matrix();
}

void ThirdPersonCamera::_update_from_gamepad_input(void)
{
	float velocity            = this->_movement_speed;

	if (this->_gamepad.getGamepadState(GLFW_JOYSTICK_1).buttons
		[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] == GLFW_PRESS)
		this->_dist_to_target += velocity;
	if (this->_gamepad.getGamepadState(GLFW_JOYSTICK_1).buttons
		[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] == GLFW_PRESS)
		this->_dist_to_target -= velocity;
	if (this->_dist_to_target < 0.001f)
		this->_dist_to_target = 0.001f;
	if (std::abs(this->_gamepad.getGamepadState(GLFW_JOYSTICK_1).axes
				 [GLFW_GAMEPAD_AXIS_RIGHT_X]) > CAM_DEAD_ZONE)
		this->_yaw += (this->_gamepad.getGamepadState(GLFW_JOYSTICK_1).axes
					   [GLFW_GAMEPAD_AXIS_RIGHT_X] * this->_mouse_sensitivity *
					   THIRD_CAM_GAMEPAD_SENSITIVITY);
	if (std::abs(this->_gamepad.getGamepadState(GLFW_JOYSTICK_1).axes
				 [GLFW_GAMEPAD_AXIS_RIGHT_Y]) > CAM_DEAD_ZONE)
		this->_pitch += (this->_gamepad.getGamepadState(GLFW_JOYSTICK_1).axes
						 [GLFW_GAMEPAD_AXIS_RIGHT_Y] * this->_mouse_sensitivity *
						 THIRD_CAM_GAMEPAD_SENSITIVITY);
	if (this->_pitch > 89.0f)
		this->_pitch          = 89.0f;
	if (this->_pitch < -89.0f)
		this->_pitch          = -89.0f;
	this->_update_vector_matrix();
}

ThirdPersonCamera::ThirdPersonCameraFailException::ThirdPersonCameraFailException(void)
{
	this->_msg = "ThirdPersonCamera : Something failed";
}

ThirdPersonCamera::ThirdPersonCameraFailException::~ThirdPersonCameraFailException(void) throw()
{
}