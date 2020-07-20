#pragma once
#include <iostream>
#include "Entity.h"
#include "Attachment/Component.h"
#include "Input/Input.h"


class Script : public Component
{

public:


protected:

	Script(const char *name);
	void setEntityReference(Entity *e);
	virtual void update(float timeStep);
	virtual void start();

	~Script();
	

	Entity * baseReference;
};

