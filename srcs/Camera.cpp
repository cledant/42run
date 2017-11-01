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

#include "Camera.hpp"

Camera::Camera(Input const &input, glm::vec3 const &pos, glm::vec3 const &world_up,
	glm::vec3 const &front, GLfloat yaw, GLfloat pitch) : _input(input),
	_world_up(world_up), _pos(pos), _front(front), _mouse_sensitivity(0.05f),
	_update_cam(true), _yaw(yaw), _pitch(pitch)
{
	this->_movement_speed = 0.075f;
	this->update(true);
}

Camera::~Camera(void)
{
}

Camera		&Camera::operator=(Camera const &rhs)
{
	static_cast<void>(rhs);
	return (*this);
}

void				Camera::update(bool mouse_exclusive_to_manager)
{
	this->_update_cam = mouse_exclusive_to_manager;
	if (this->_update_cam == true)
	{
		if (this->_input.mouse_refreshed == true)
			this->_update_from_mouse_input();
		this->_update_from_keyboard_input();
		this->_view = glm::lookAt(this->_pos, this->_pos + this->_front, this->_up);
	}
}

void				Camera::toggle_update(void)
{
	this->_update_cam = (true) ? false : true;
}

glm::mat4 const		&Camera::getViewMatrix(void) const
{
	return (this->_view);
}

glm::vec3 const		&Camera::getFront(void) const
{
	return (this->_front);
}

glm::vec3 const		&Camera::getUp(void) const
{
	return (this->_up);
}

glm::vec3 const		&Camera::getRight(void) const
{
	return (this->_right);
}

glm::vec3 const		&Camera::getPos(void) const
{
	return (this->_pos);
}

void				Camera::_update_from_keyboard_input(void)
{
	float		velocity;

	velocity = this->_movement_speed;
	if (this->_input.p_key[GLFW_KEY_W] == PRESSED)
		this->_pos += velocity * this->_front;
	if (this->_input.p_key[GLFW_KEY_S] == PRESSED)
		this->_pos -= velocity * this->_front;
	if (this->_input.p_key[GLFW_KEY_D] == PRESSED)
		this->_pos += velocity * this->_right;
	if (this->_input.p_key[GLFW_KEY_A] == PRESSED)
		this->_pos -= velocity * this->_right;
	if (this->_input.p_key[GLFW_KEY_E] == PRESSED)
		this->_pos += velocity * this->_up;
	if (this->_input.p_key[GLFW_KEY_Q] == PRESSED)
		this->_pos -= velocity * this->_up;
}

void				Camera::_update_from_mouse_input(void)
{
	this->_yaw += (this->_input.x_offset * this->_mouse_sensitivity);
	this->_pitch += (this->_input.y_offset * this->_mouse_sensitivity);
	if (this->_pitch > 89.0f)
		this->_pitch = 89.0f;
	if (this->_pitch < -89.0f)
		this->_pitch = -89.0f;
	this->_update_vector_matrix();
}

void				Camera::_update_vector_matrix(void)
{
	this->_front.x = cos(glm::radians(this->_yaw)) *
		cos(glm::radians(this->_pitch));
	this->_front.y = sin(glm::radians(this->_pitch));
	this->_front.z = sin(glm::radians(this->_yaw)) *
		cos(glm::radians(this->_pitch));
	glm::normalize(this->_front);
	this->_right = glm::normalize(glm::cross(this->_front, this->_world_up));
	this->_up = glm::normalize(glm::cross(this->_right, this->_front));
}

Camera::CameraFailException::CameraFailException(void)
{
	this->_msg = "Camera : Something failed";
}

Camera::CameraFailException::~CameraFailException(void) throw()
{
}
