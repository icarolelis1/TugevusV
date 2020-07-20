#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0, 0, 3);
	scale = glm::vec3(1);
	rotation = glm::vec3(0,0.0,0.);
}

void Transform::setPosition(glm::vec3 pos) 
{
	position = pos;
}

void Transform::setPosition(float x, float y,float z)
{
	position = glm::vec3(x,y,z);
}

void Transform::setScale(glm::vec3 s)
{
	scale = s;
}
void Transform::setScale(float x,float y ,float z)
{
	scale = glm::vec3(x,y,z);
}
void Transform::setRotation(glm::vec3 r)
{
	rotation = r;
}

void Transform::setRotation(float x, float y, float z)
{
	rotation = glm::vec3(x,y,z);

}

void Transform::increaseRotation(glm::vec3 rot) 
{
	this->rotation += rot;
}

void Transform::increasePos(glm::vec3 aditional)
{
	position += aditional;
}

glm::vec3 Transform::getPosition() 
{
	return position;
}

glm::vec3 Transform::getScale()
{
	return scale;
}

glm::vec3 Transform::getRotation()
{
	return rotation;
}

glm::mat4 Transform::getModelMatrix()
{

	glm::mat4 identity(1.0f);
	identity = glm::translate(identity, position);
	identity = glm::rotate(identity, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	identity = glm::rotate(identity, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	identity = glm::rotate(identity, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	identity = glm::scale(identity, scale);
	identity = glm::translate(identity, glm::vec3(0));

	return identity;

}

Transform::~Transform()
{
}
  