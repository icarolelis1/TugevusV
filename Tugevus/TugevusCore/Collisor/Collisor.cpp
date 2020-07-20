#include "Collisor.h"

namespace TUGEV {

	Collisor::Collisor(Entity* _entity ,glm::vec3 posOffset,  const char* name):Component(name,COLLISOR),entity(_entity),localPosition(posOffset), collisorType(CollisorType::SPHERE_COLLISOR)
	{
	}

	void Collisor::start()
	{
	}

	void Collisor::update(float deltaTime)
	{
		//Position the collisor in reference to it's entity
		collisorPosition = entity->transform.getPosition() + localPosition;

	}

	CollisorType Collisor::getCollisorType()
	{
		return collisorType;
	}

	glm::vec3 Collisor::getCollisorPosition()
	{
		return collisorPosition;
	}


	Collisor::~Collisor()
	{
	}

	SphereCollisor::SphereCollisor(Entity* t, float _radius  , const char* name ) :Collisor(t), radius(_radius)
	{
	}

	bool SphereCollisor::testCollision(Collisor& c)
	{
		switch (c.getCollisorType()) {

		case CollisorType::SPHERE_COLLISOR:
			SphereCollisor* s = static_cast<SphereCollisor*>(&c);
			return this->testCollisionAgainstSphere(*s);
			break;
		
	}
	
	}

	bool SphereCollisor::testCollisionAgainstSphere(SphereCollisor& c)
	{
		return (glm::distance(collisorPosition, c.getCollisorPosition()) < this->radius);

	}

	



}
