#pragma once
#include "Attachment/Script.h"
#include "Camera/TugevCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "window/Window.h"
#include "Collisor/Collisor.h"
class CameraController : public Script 
{
public:

	CameraController(const char * name, TUGEV::Camera* _camera);
	void start();
	void update(float timeStep);
//	void setEntityReference(Entity* e);
	void setSceneGraphRef(std::shared_ptr<TUGEV::SceneGraph> scene);
	

	glm::vec4 getWorldMouse();

	float YAW = 90;
	float PITCH = -60;
	float ROLL;

private:


	std::shared_ptr<TUGEV::SceneGraph> sceneGraph;
	void rayCast(glm::vec3 &rd);

	bool cameraControlling = 0;
	void updateDirections();

	TUGEV::Camera* camera;
};

