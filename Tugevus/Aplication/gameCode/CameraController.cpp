#include "CameraController.h"

CameraController::CameraController(const char * name, TUGEV::Camera* _camera) : Script(name) ,camera(_camera)
{

}

void CameraController::start()
{

	camera->transform.setPosition(-7.63, 13.859, -15.7);
	updateDirections();

}

void CameraController::update(float timeStep)
{

	if (Window::inputManager.getKeyPressing(TUGEV_KEY_W)) {
		camera->transform.increasePos(camera->eulerDir.front * timeStep * .04f);
	}
	
	if (Window::inputManager.getKeyPressing(TUGEV_KEY_S)) {
		camera->transform.increasePos(-camera->eulerDir.front * timeStep * .04f);
	}
	if (Window::inputManager.getKeyPressing(TUGEV_KEY_A)) {
		YAW += .04f * timeStep;
	}
	if (Window::inputManager.getKeyPressing(TUGEV_KEY_D)) {
		YAW -= .04f * timeStep;
	}

	if (Window::inputManager.getKeyPressing(TUGEV_KEY_R)) {
		PITCH += .04f* timeStep;
	}

	if (Window::inputManager.getKeyPressing(TUGEV_KEY_F)) {
		PITCH -= .04f * timeStep;
	}
	
	if (Window::inputManager.getKeyPressing(TUGEV_KEY_P)) {
		cameraControlling = !cameraControlling;
	}
	TUGEV::CursorPos cursorPos = Window::mouse.getCursorPos();

	if (Window::inputManager.getKeyPressing(TUGEV_KEY_X)) {
		std::cout << "Yaw : " << YAW << std::endl;
		std::cout << "Pitch :  " << PITCH << std::endl;

	}


	//if (Window::Window::mouse.getMouseActionStatus(TUGEV_MOUSE_RIGHT, MOUSE_PRESS)) {
	//		glm::vec3 p = getWorldMouse();
	//		std::cout << "Ray\n";
	//		generalUtiliy::printVec3(p);
	//			rayCast(p);
	//	
	//}
	auto offset = 40;
	auto camVelocity = .005f * timeStep;

	if ( Window::mouse.getMouseActionStatus(TUGEV_MOUSE_RIGHT,MOUSE_PRESS)) {
		if (cursorPos.x >= WIDTH - offset) { camera->transform.increasePos(camVelocity * camera->eulerDir.right); }
		if (cursorPos.x <= offset) camera->transform.increasePos(-camVelocity * camera->eulerDir.right);

		if (cursorPos.y >= HEIGHT - offset) camera->transform.increasePos(-camVelocity * glm::normalize(glm::vec3(camera->eulerDir.front.x,0, camera->eulerDir.front.z)* glm::cos(glm::radians(-90 - PITCH))));
		if (cursorPos.y <= (offset+45)) camera->transform.increasePos(camVelocity* glm::normalize(glm::vec3(camera->eulerDir.front.x, 0, camera->eulerDir.front.z) * glm::cos(glm::radians(-90 - PITCH))));
	}


	updateDirections();

}



void CameraController::setSceneGraphRef(std::shared_ptr<TUGEV::SceneGraph> scene)
{
	this->sceneGraph = scene;
}

glm::vec4 CameraController::getWorldMouse()
{
	TUGEV::CursorPos cursor = Window::mouse.getCursorPos();
	glm::vec4 ndc = glm::vec4((2 * cursor.x) / WIDTH - 1,1.f- (2*cursor.y)/HEIGHT,-1.0,1.0);
	glm::vec4 ray_eye = glm::inverse(camera->getProjectionMatrix()) * ndc;
	ray_eye = glm::vec4(ray_eye.x,ray_eye.y, -1., 0.);

	ray_eye = glm::inverse(camera->viewMatrix() ) * ray_eye;


	return normalize(ray_eye);
	
}

bool hit_sphere(const glm::vec3& center, float radius, const glm::vec3& r) {

	glm::vec3 oc = r - center;
	float a = glm::dot(r, r);
	float b = 2. * glm::dot(oc, r);
	float c = glm::dot(oc, oc) - (radius * radius);

	float d = b * b - 4 * a * c;

	return d>0;


}
void CameraController::rayCast(glm::vec3 &rd)
{
	Entity* e = sceneGraph->root->childs[2]->entity.get();

		for (int i = 1; i < sceneGraph->root->childs.size(); i++) {
			Entity* e = sceneGraph->root->childs[i]->entity.get();
			TUGEV::SphereCollisor* collisor = static_cast<TUGEV::SphereCollisor*> (e->getComponentIfExists("Collisor"));
			if (collisor == nullptr)continue;
			glm::vec3 ro = camera->transform.getPosition();
			glm::vec3 s = collisor->getCollisorPosition();
			float t = glm::dot(s - ro, rd);
			glm::vec3 p = ro + (rd * t);
			float y = glm::length(s -p);

			if (y < collisor->radius) {
				std::cout << "Ro : \n";
				generalUtiliy::printVec3(ro);
				//generalUtiliy::printVec3(e->transform.getPosition());
			std::cout << "Collision for object : " << e->name << std::endl;
			std::cout << "Distane : " << t << std::endl;
			std::cout << "Collisor Center : ";
			generalUtiliy::printVec3(collisor->getCollisorPosition());
			std::cout << "Final Point : ";
			generalUtiliy::printVec3(p) ;
			}
			else {
				continue;
				std::cout << "No Collision-----------------------------------------------\n";
				//generalUtiliy::printVec3(e->transform.getPosition());
				std::cout << "Collision for object : " << e->name << std::endl;
				std::cout << "Distane : " << y << std::endl;
				std::cout << "Collisor Center : ";
				generalUtiliy::printVec3(collisor->getCollisorPosition());
				std::cout << "Final Point : ";
				generalUtiliy::printVec3(p);

				std::cout << "No Collision-----------------------------------------------\n";

			}
			 
		}

	
}

void CameraController::updateDirections()
{
	glm::vec3 front;
	front.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	front.y = sin(glm::radians(PITCH));
	front.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	camera->eulerDir.front = glm::normalize(front);

	glm::vec3 WorldUp = glm::vec3(0, -1, 0);
	camera->eulerDir.right = glm::normalize(glm::cross(camera->eulerDir.front, WorldUp));
	camera->eulerDir.up = glm::normalize(glm::cross(camera->eulerDir.right, camera->eulerDir.front));
}


