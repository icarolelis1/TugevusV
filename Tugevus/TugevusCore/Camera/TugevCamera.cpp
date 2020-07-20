#include "TugevCamera.h"

namespace TUGEV {

	Camera::Camera(const char* name, glm::vec3 initialPosiiton, glm::vec3 target) :Entity(name, NONE) {
		transform.setPosition(initialPosiiton);
		eulerDir.front = glm::normalize(target - initialPosiiton);
		nearPlane = 1.f;
		farPlane = 150.f;
	};


	void Camera::start() {
		Entity::start();
	}

	void Camera::update(float timeStep) { Entity::update(timeStep); }

	glm::mat4 Camera::getProjectionMatrix() {

		return glm::perspective(glm::radians(45.f), (float)WIDTH / HEIGHT, nearPlane, farPlane);
	}

	std::array<glm::vec3,8> Camera::calculateFrustumConers()
	{
		fov = 45.0f;
		float tan = glm::tan(glm::radians(fov / 2.));
		float aspectRatio = WIDTH / HEIGHT;
		float farPlane = transform.getPosition().y + 4.f;
		float heightNear = 2 * tan * nearPlane;
		float widthNear =  heightNear * aspectRatio;
		float heightFar = 2 * tan * farPlane ;
		float widthFar = heightFar * aspectRatio;

		glm::vec3 centerNear = transform.getPosition() + eulerDir.front * nearPlane;
		glm::vec3 centerFar = transform.getPosition() + eulerDir.front * farPlane;

		std::array<glm::vec3, 8> corners;

		//Near corners
		glm::vec3 up = eulerDir.up;

		corners[0] = centerNear + up * heightNear / 2.f - eulerDir.right * widthNear / 2.f;// near top left
		corners[1] = centerNear + up * heightNear / 2.f + eulerDir.right * widthNear / 2.f;//near top right
		corners[2] = centerNear - up * heightNear / 2.f - eulerDir.right * widthNear / 2.f;// near bottom left
		corners[3] = centerNear - up * heightNear / 2.f + eulerDir.right * widthNear / 2.f;// near bottom right
		//Far Corners
		corners[4] = centerFar + up * heightFar / 2.f - eulerDir.right * widthFar / 2.f;// far top left
		corners[5] = centerFar + up * heightFar / 2.f + eulerDir.right * widthFar / 2.f;//far top right
		corners[6] = centerFar - up * heightFar / 2.f - eulerDir.right * widthFar / 2.f;// far bottom left
		corners[7] = centerFar - up * heightFar / 2.f + eulerDir.right * widthFar / 2.f;// far bottom right

		//
		//std::cout << "-------------------------------------------------------\n";
		//std::cout << corners[0].x << "  " << corners[0].y << " " << corners[0].z << std::endl;
		//std::cout << corners[1].x << "  " << corners[1].y << " " << corners[1].z << std::endl;
		//std::cout << corners[2].x << "  " << corners[2].y << " " << corners[3].z << std::endl;
		//std::cout << corners[3].x << "  " << corners[3].y << " " << corners[3].z << std::endl;
		//std::cout << "-------------------------------------------------------\n";
		//
		return corners;

	}

	std::array<float, 6> Camera::calculateFrustumInLightSpace(glm::mat4 lightMatrix)
	{
		std::array<glm::vec3, 8> corners = calculateFrustumConers();
		float maxX, maxY, maxZ ;
		float minX, minY, minZ ;
		maxX = maxY = maxZ = -10000;
		minX = minY = minZ = 10000;

		for (glm::vec3 point : corners) {
			glm::vec3 frustumPoint  = lightMatrix *glm::vec4(point,1.0);

			if (frustumPoint.x > maxX)maxX = frustumPoint.x;
			if (frustumPoint.x < minX)minX = frustumPoint.x;

			if (frustumPoint.y > maxY)maxY = frustumPoint.y;
			if (frustumPoint.y < minY)minY = frustumPoint.y;

			if (frustumPoint.z > maxZ)maxZ = frustumPoint.z;
			if (frustumPoint.z < minZ)minZ = frustumPoint.z;


		}

		std::array<float, 6> boundries = { minX,maxX,minY,maxY,minZ,maxZ };
		return boundries;
	}


	glm::mat4 Camera::viewMatrix() {
		auto pos = transform.getPosition();
		glm::mat4 lookat = glm::lookAt(pos, pos + eulerDir.front, eulerDir.up);
		return lookat;
	};
}