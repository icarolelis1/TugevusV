#pragma once
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>
#include <glm/gtc/matrix_transform.hpp>
struct EulerDirections {
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

};

class Transform
{

public:
	Transform();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getRotation();

	void setPosition(glm::vec3 pos);
	void setPosition(float x, float y, float z);
	void setScale(glm::vec3);
	void setScale(float x, float y, float z);

	void setRotation(glm::vec3);
	void setRotation(float x,float y,float z);

	void increasePos(glm::vec3 aditional);
	void increaseRotation(glm::vec3 rotation);

	glm::mat4 getModelMatrix();

	~Transform();
	glm::vec3 position;

private:
	glm::vec3 scale;
	glm::vec3 rotation;

};

