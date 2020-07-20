#pragma once
#include <iostream>
#include <TugevusStructs.h>

class Component {

public:

	Component(const char *n, AttachmentType t) { name = n; type = t; };
	virtual void start() = 0;
	virtual void update(float timeStep) = 0;
	const char* name;


private:
	AttachmentType type;

};