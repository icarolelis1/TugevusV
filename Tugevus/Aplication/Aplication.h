#pragma once

#include <iostream>
#include <vector>
#include "window/Window.h"
#include "gameCode/CameraController.h"
#include "material/Material.h"
#include "Collisor/Collisor.h"

class Aplication
{
public:
	Aplication(const char name[15]);
	void start();
	void update();
	~Aplication();


private:

	std::shared_ptr<TUGEV::SceneGraph> sceneGraph;
	std::vector < std::unique_ptr<TUGEV::Material> >materials;

	std::shared_ptr<Window> window;
	std::string aplicationName;
	
	std::shared_ptr<Render> render;

	void initiateResources();
	void createSceneGraph();
	void createSceneMaterials();

	void cleanupResources();
	void run();

};
