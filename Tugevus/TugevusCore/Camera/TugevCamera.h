#pragma once
#include "Entity.h"


namespace TUGEV {

	class Camera : public Entity
	{
	public:
		glm::mat4 viewMatrix();
		void start();
		void update(float timeStep);


		Camera(const char* name, glm::vec3 initialPosition,glm::vec3 target);
		EulerDirections eulerDir;
		glm::mat4 getProjectionMatrix();
		std::array<glm::vec3, 8> calculateFrustumConers();	
		std::array<float, 6> calculateFrustumInLightSpace(glm::mat4 lightMatrix);

	private:

		glm::mat4 projection;
		float nearPlane;
		float farPlane;
		float fov;
		~Camera() {  };
		void destroy() {};

		
	};

}


