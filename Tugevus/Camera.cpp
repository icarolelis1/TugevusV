#include "Camera.h"

Camera::Camera(const char *name,GLFWwindow *w):Entity(name),window(w)
{

}

void Camera::start() {

	transform.setPosition(glm::vec3(0));

}

void Camera::update() {

	updateDirections();

}

void Camera::updateDirections() {

	euler.front.x = glm::cos(glm::radians(YAW) *glm::cos(glm::radians(PITCH)));
	euler.front.y = glm::sin(glm::radians(PITCH));
	euler.front.z = glm::sin(glm::radians(YAW) *glm::cos(glm::radians(PITCH)));
	glm::vec3 WorldUp = glm::vec3(0, 1, 0);
	euler.right = glm::normalize(glm::cross(euler.front, WorldUp));
	euler.up = glm::normalize(glm::cross(euler.right, euler.front));
	
}

void Camera::keyBoardNavigation(Directions direction, float deltaTime)
{
	switch (direction) {
	case FORWARD:
		transform.setPosition(transform.getPosition()+euler.front * speed *deltaTime);
	case DOWN:
		transform.setPosition(transform.getPosition()+euler.front * speed *deltaTime);
	case BACKWARD:
		transform.setPosition(transform.getPosition() + euler.front * speed *deltaTime);
	case RIGHT:
		transform.setPosition(transform.getPosition() + euler.front * speed *deltaTime);
	case LEFT:
		transform.setPosition(transform.getPosition() + euler.front * speed *deltaTime);
	case UP:
		transform.setPosition(transform.getPosition() + euler.front * speed *deltaTime);


	}
}

glm::mat4 Camera::getViewMatrix() {

	glm::mat4 lookat = glm::lookAt(transform.getPosition(), transform.getPosition() + euler.front, euler.up);

	return lookat;
}

Camera::~Camera()
{
}

void Camera::processInput()
{

}
