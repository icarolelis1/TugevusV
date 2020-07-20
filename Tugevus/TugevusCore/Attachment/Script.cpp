#include "Script.h"






Script::Script(const char * name) :Component(name, SCRIPT)
{
}


void Script::setEntityReference(Entity * e)
{
	baseReference = e;
}

void Script::update(float timeStep)
{
	//inputManager.handleInputs();
}


void Script::start()
{
}

Script::~Script()
{
}
