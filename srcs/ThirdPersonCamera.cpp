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

ThirdPersonCamera::ThirdPersonCamera(Input const &input, glm::vec3 const &target_pos,
									 float dist_to_target, glm::vec3 const &world_up,
									 glm::vec3 const &front, GLfloat yaw, GLfloat pitch) :
		Camera(input, glm::vec3({0.0f, 0.0f, 0.0f}), world_up, front, yaw, pitch),
		_dist_to_target(dist_to_target)
{
	this->update_third_person(true, target_pos);
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
											glm::vec3 const &target_pos)
{
	glm::vec3	tr = this->_front * this->_dist_to_target;

	this->_pos = target_pos;
	this->update(mouse_exclusive_to_manager);
	this->_view = glm::translate(this->_view, tr);
}

void ThirdPersonCamera::_update_from_keyboard_input(void)
{
	float velocity = this->_movement_speed;

	if (this->_input.p_key[GLFW_KEY_E] == PRESSED)
		this->_dist_to_target += velocity;
	if (this->_input.p_key[GLFW_KEY_Q] == PRESSED)
		this->_dist_to_target -= velocity;
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

ThirdPersonCamera::ThirdPersonCameraFailException::ThirdPersonCameraFailException(void)
{
	this->_msg = "ThirdPersonCamera : Something failed";
}

ThirdPersonCamera::ThirdPersonCameraFailException::~ThirdPersonCameraFailException(void) throw()
{
}