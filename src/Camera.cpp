//
// Created by Ronan on 13/01/2021.
//

#include "Camera.h"
#include "glm/gtx/transform.hpp"

glm::mat4 Camera::LookAt() {
	return glm::lookAt(_camPos, _camCenter, _camUp);
}

glm::mat4 Camera::LookAtFront() {
	return glm::lookAt(_camPos, _camPos + _camFront, _camUp);
}

void Camera::init() {
	//_camCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	_camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	_camPos = glm::vec3(0.0f, 0.0f, 3.0f);
	_camFront = glm::vec3(0.0f,0.0f,-1.0f);
	_camSpeed = 3.5f;
}

void Camera::moveForward(float speed) {
	_camPos += speed * _camFront;
}

void Camera::moveBackward(float deltaTime) {
	_camPos -= (_camSpeed * deltaTime) * _camFront;
}

void Camera::moveLeftward(float deltaTime) {
	_camPos -= glm::normalize(glm::cross(_camFront, _camUp)) * (_camSpeed*deltaTime);
}

void Camera::moveRightward(float deltaTime) {
	_camPos += glm::normalize(glm::cross(_camFront, _camUp)) * (_camSpeed*deltaTime);
}

void Camera::mouse_callback(GLFWwindow *window, double xpos, double ypos){
	if (_firstMouse)
	{
		_lastX = (float)xpos;
		_lastY = (float)ypos;
		_firstMouse = false;
	}

	float xoffset = (float)xpos - _lastX;
	float yoffset = _lastY -(float)ypos;
	_lastX = (float)xpos;
	_lastY = (float)ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw   += xoffset;
	_pitch += yoffset;

	if(_pitch > 89.0f)
		_pitch = 89.0f;
	if(_pitch < -89.0f)
		_pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_camFront = glm::normalize(direction);
}