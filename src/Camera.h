//
// Created by Ronan on 13/01/2021.
//

#ifndef PHONG_CAMERA_H
#define PHONG_CAMERA_H
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

class Camera {
public:
	void init();

	glm::mat4 LookAt();

	glm::mat4 LookAtFront();

	void moveForward(float deltaTime = 1.0f);

	void moveBackward(float deltaTime = 1.0f);

	void moveLeftward(float deltaTime = 1.0f);

	void moveRightward(float deltaTime = 1.0f);

	void mouse_callback(GLFWwindow *window, double xpos, double ypos);

	inline void setCenter(glm::vec3 c) {
		_camCenter = c;
	}

	inline void setUp(glm::vec3 u) {
		_camUp = u;
	}

	inline void setPos(glm::vec3 p) {
		_camPos = p;
	}

	inline void setFront(glm::vec3 f) {
		_camFront = f;
	}

	inline glm::vec3 getCenter() {
		return _camCenter;
	}

	inline glm::vec3 getUp() {
		return _camUp;
	}

	inline glm::vec3 getPos() {
		return _camPos;
	}

	inline glm::vec3 getFront() {
		return _camFront;
	}

	inline float getSpeed() const {
		return _camSpeed;
	}

private:
	glm::vec3 _direction;
	glm::vec3 _camCenter;
	glm::vec3 _camUp;
	glm::vec3 _camPos;
	glm::vec3 _camFront;

	bool _firstMouse = true;
	float _lastX;
	float _lastY;
	float _camSpeed;
	float _yaw;
	float _pitch;

};


#endif //PHONG_CAMERA_H
