#pragma once
#include "Attachment/Component.h"
#include "Entity.h"
namespace TUGEV {

	enum class CollisorType {
		SPHERE_COLLISOR = 0,
		BOX_COLLISOR = 1
	};
	

	
	class Collisor : public Component
	{
	public:
		Collisor(Entity* transform, glm::vec3 posOffset = glm::vec3(0) ,const char* name  = "Collisor");
		void start();
		void update(float deltaTime);

		virtual bool testCollision(Collisor& c) = 0;
		CollisorType getCollisorType();
		virtual glm::vec3 getCollisorPosition();

		virtual ~Collisor();
		 Entity* entity;
	protected:
		glm::vec3 localPosition;
		CollisorType collisorType;
		glm::vec3 collisorPosition;


	};

	class SphereCollisor :public Collisor {
	public:
		SphereCollisor(Entity* t, float _radius, const char* name = "Collisor");

		bool testCollision( Collisor& c);

		float radius;
	private:
		bool testCollisionAgainstSphere( SphereCollisor& c);

		//bool testCollisionAgainstBox( BoxCollisor& c);
	};


}