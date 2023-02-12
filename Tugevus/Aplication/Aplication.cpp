#include "Aplication.h"


Aplication::Aplication(const char name[15]) :aplicationName(name)
{
}

void Aplication::start()
{
	//That's silly
	run();

}

void Aplication::update()
{
	window->update();
}

Aplication::~Aplication()
{
	cleanupResources();
}

void Aplication::initiateResources()
{
	//Create window abstraction object and the Render inside window
	window = std::make_shared<Window>(1600,1024,aplicationName.c_str());

	assert(window->glfw_window != nullptr);
	

	//create materials
	createSceneMaterials();

	//Create the sceneGraph
	createSceneGraph();

	//Send scene graph to render
 	window->render->setSceneGraph(sceneGraph);

	//Send Materials to render
	window->render->setSceneMaterials(materials);

	//initialize all the render needed resources
	window->render->initiateResources();
}

void Aplication::createSceneGraph()
{
	sceneGraph = std::make_shared<TUGEV::SceneGraph>();

	//Add glfw_window to static input manager;
	Window::inputManager.glfw_window = window->glfw_window;
	Window::mouse.glfw_window = window->glfw_window;


	//create Camera Entity and add an CameraController component to it
	Entity* camera = new TUGEV::Camera("mainCamera", glm::vec3(0, 3, 0), glm::vec3(0, 0, 0))	;
	TUGEV::Camera* cameraEntity = static_cast<TUGEV::Camera*>(camera);
	Component * cameraController = new CameraController("cameraController" , cameraEntity);
	CameraController* c = static_cast<CameraController*>(cameraController);
	c->setSceneGraphRef(sceneGraph);
	camera->attachComponent(cameraController);
	sceneGraph->addEntity(camera,nullptr);


	//Create model entity with pipeline PBR_COOK_TORRANCE
	Entity* p0 = new Model("p0","Aplication\\assets\\scene\\scene.obj" ,PBR_COOK_TORRANCE);
	Model* piece0 = static_cast<Model*>(p0);
	piece0->materialset = &materials[1]->descriptorSet;
	p0->transform.setScale(glm::vec3(.5));
	p0->transform.setPosition(-8, 1.4, -7.0);
	Component* sphereCollisor = new TUGEV::SphereCollisor(p0, 3.0);
	p0->attachComponent(sphereCollisor);
	sceneGraph->addEntity(p0,nullptr);

	std::cout << "Size of Model : " << sizeof(p0) << std::endl;

	Entity* p1 = new Model("p1", "Aplication\\assets\\scene\\scene.obj", PBR_COOK_TORRANCE);
	Model* piece1 = static_cast<Model*>(p1);
	piece1->materialset = &materials[0]->descriptorSet;
	p1->transform.setScale(glm::vec3(.5));
	p1->transform.setPosition(-1, 1.4, -7.0);
	sceneGraph->addEntity(p1, nullptr);

	Entity* modular1 = new Model("modular1", "Aplication\\assets\\column\\modular1.fbx", PBR_COOK_TORRANCE);
	Model* m1 = static_cast<Model*>(modular1);
	m1->materialset = &materials[1]->descriptorSet;
	m1->transform.setScale(glm::vec3(7, 4, .1));
	m1->transform.setPosition(4, 1.1, -8.0);
	m1->transform.setRotation(glm::vec3(90, 0, 0));
	sceneGraph->addEntity(modular1, nullptr);

	Entity* modular2 = new Model("modular2", "Aplication\\assets\\column\\modular1.fbx", PBR_COOK_TORRANCE);
	Model* m2 = static_cast<Model*>(modular2);
	m2->materialset = &materials[1]->descriptorSet;
	m2->transform.setScale(glm::vec3(5, 4, .2));
	m2->transform.setPosition(6, 1.5, -8.0);
	m2->transform.setRotation(glm::vec3(90, 0, 0));
	sceneGraph->addEntity(modular2, nullptr);


	Entity* modular3 = new Model("modular3", "Aplication\\assets\\column\\modular1.fbx", PBR_COOK_TORRANCE);
	Model* m3 = static_cast<Model*>(modular3);
	m3->materialset = &materials[1]->descriptorSet;
	m3->transform.setScale(glm::vec3(3, 4, .2));
	m3->transform.setPosition(8, 1.9, -8.0);
	m3->transform.setRotation(glm::vec3(90, 0, 0));
	sceneGraph->addEntity(modular3, nullptr);

	Entity* modular4 = new Model("modular4", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m4 = static_cast<Model*>(modular4);
	m4->materialset = &materials[1]->descriptorSet;
	m4->transform.setScale(glm::vec3(3, .2, 7));
	m4->transform.setPosition(-9, 1.3, 5.0);
	m4->transform.setRotation(glm::vec3(0, 0, 0));
	sceneGraph->addEntity(modular4, nullptr);

	Entity* modular5 = new Model("modular5", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m5 = static_cast<Model*>(modular5);
	m5->materialset = &materials[1]->descriptorSet;
	m5->transform.setScale(glm::vec3(3, .2, 5));
	m5->transform.setPosition(-9, 1.8, 7.0);
	m5->transform.setRotation(glm::vec3(0, 0, 0));
	sceneGraph->addEntity(modular5, nullptr);

	Entity* modular6 = new Model("modular6", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m6 = static_cast<Model*>(modular6);
	m6->materialset = &materials[1]->descriptorSet;
	m6->transform.setScale(glm::vec3(3, .2, 3));
	m6->transform.setPosition(-9, 2.3, 9.0);
	m6->transform.setRotation(glm::vec3(0, 0, 0));
	sceneGraph->addEntity(modular6, nullptr);


	Entity* modular7 = new Model("modular7", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m7 = static_cast<Model*>(modular7);
	m7->materialset = &materials[1]->descriptorSet;
	m7->transform.setScale(glm::vec3(2, .1, 3));
	m7->transform.setPosition(10, 1.1, 1);
	m7->transform.setRotation(glm::vec3(0, 0, 0));
	sceneGraph->addEntity(modular7, nullptr);

	Entity* water = new Model("water", "Aplication\\assets\\water\\water.fbx", WATER_DISPLACEMENT);
	Model* w = static_cast<Model*>(water);
	w->transform.setScale(glm::vec3(12, 12, 1));
	w->transform.setPosition(-0, 1, .0);
	w->transform.setRotation(glm::vec3(-90, 0, 0));
	sceneGraph->addEntity(water, nullptr);


	Entity* border = new Model("border", "Aplication\\assets\\column\\border.fbx", DUMMY);
	Model* br = static_cast<Model*>(border);
	br->materialset = &materials[2]->descriptorSet;
	br->transform.setPosition(glm::vec3(0));
	br->transform.setScale(glm::vec3(12, 12, 1));
	br->transform.setRotation(-90, 0, 0);
	sceneGraph->addEntity(border, nullptr);


	Entity* modular8 = new Model("modular8", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m8 = static_cast<Model*>(modular8);
	m8->materialset = &materials[1]->descriptorSet;
	m8->transform.setScale(glm::vec3(3, .1, 3));
	m8->transform.setPosition(-9, 1.2, -9.0);
	m8->transform.setRotation(glm::vec3(0, 0, 0));
	sceneGraph->addEntity(modular8, nullptr);

	Entity* modular9 = new Model("modular9", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m9 = static_cast<Model*>(modular9);
	m9->materialset = &materials[0]->descriptorSet;
	m9->transform.setScale(glm::vec3(3, .1, 3));
	m9->transform.setPosition(9, 1, 9.0);
	m9->transform.setRotation(glm::vec3(0, 0, 0));
	sceneGraph->addEntity(modular9, nullptr);


	Entity* baseBoss = new Model("baseBoss", "Aplication\\assets\\column\\baseBoss.fbx", PBR_COOK_TORRANCE);
	Model* base = static_cast<Model*>(baseBoss);
	base->materialset = &materials[1]->descriptorSet;
	base->transform.setScale(glm::vec3(1.4));	
	base->transform.setPosition(0, 1, 3);
	base->transform.setRotation(glm::vec3(-90, 0, 0));
	sceneGraph->addEntity(baseBoss, nullptr);

	Entity* Boss = new Model("Boss", "Aplication\\assets\\boss\\boss.fbx", PBR_COOK_TORRANCE);
	Model* boss = static_cast<Model*>(Boss);
	boss->materialset = &materials[1]->descriptorSet;
	boss->transform.setScale(glm::vec3(.01));
	boss->transform.setPosition(0, 1.5, 3);
	boss->transform.setRotation(glm::vec3(90, 90, 90));
	sceneGraph->addEntity(Boss, nullptr);

	Entity* piece2 = new Model("p2", "Aplication\\assets\\scene\\scene.obj", PBR_COOK_TORRANCE);
	Model* p2 = static_cast<Model*>(piece2);
	p2->materialset = &materials[0]->descriptorSet;
	p2->transform.setScale(glm::vec3(.5));
	p2->transform.setPosition(-1, 1.4, -10.0);
	sceneGraph->addEntity(piece2, nullptr);


	Entity* piece3 = new Model("p3", "Aplication\\assets\\scene\\scene.obj", PBR_COOK_TORRANCE);
	Model* p3 = static_cast<Model*>(piece3);
	p3->materialset = &materials[0]->descriptorSet;
	p3->transform.setScale(glm::vec3(.5));
	p3->transform.setPosition(7, 1.1, 10.0);
	p3->transform.setRotation(glm::vec3(0, 180, 0));
	sceneGraph->addEntity(piece3, nullptr);

	Entity* piece4 = new Model("p4", "Aplication\\assets\\scene\\scene.obj", PBR_COOK_TORRANCE);
	Model* p4 = static_cast<Model*>(piece4);
	p4->materialset = &materials[0]->descriptorSet;
	p4->transform.setScale(glm::vec3(.5));
	p4->transform.setPosition(9, 1.4, 1.0);
	p4->transform.setRotation(0, 180, 0);
	sceneGraph->addEntity(piece4, nullptr);

	Entity* piece5 = new Model("p5", "Aplication\\assets\\scene\\scene.obj", PBR_COOK_TORRANCE);
	Model* p5 = static_cast<Model*>(piece5);
	p5->materialset = &materials[0]->descriptorSet;
	p5->transform.setScale(glm::vec3(.5));
	p5->transform.setPosition(-5, 1.1, -10.0);
	sceneGraph->addEntity(piece5, nullptr);

	Entity* gem1 = new Model("Blue Orb", "Aplication\\assets\\gem\\gem.fbx", DUMMY);
	Model* g1 = static_cast<Model*>(gem1);
	g1->materialset = &materials[3]->descriptorSet;
	g1->transform.setScale(glm::vec3(.5));
	g1->transform.setPosition(-3, 2.1, -10.0);
	g1->transform.setRotation(-90, 0, 0);
	sceneGraph->addEntity(gem1, nullptr);

	Entity* gem2 = new Model("Red Orb", "Aplication\\assets\\gem\\gem.fbx", DUMMY);
	Model* g2 = static_cast<Model*>(gem2);
	g2->materialset = &materials[4]->descriptorSet;
	g2->transform.setScale(glm::vec3(.3));
	g2->transform.setPosition(-7, 4, 9.0);
	g2->transform.setRotation(-90, 0, 0);
	sceneGraph->addEntity(gem2, nullptr);


	Entity* gem3 = new Model("Green Orb", "Aplication\\assets\\gem\\gem.fbx", DUMMY);
	Model* g3 = static_cast<Model*>(gem3);
	g3->materialset = &materials[5]->descriptorSet;
	g3->transform.setScale(glm::vec3(.3));
	g3->transform.setPosition(11, 2, 3.0);
	g3->transform.setRotation(-90, 0, 0);
	sceneGraph->addEntity(gem3, nullptr);

	Entity* gem4 = new Model("Purple Orb", "Aplication\\assets\\gem\\gem.fbx", DUMMY);
	Model* g4 = static_cast<Model*>(gem4);
	g4->materialset = &materials[6]->descriptorSet;
	g4->transform.setScale(glm::vec3(.3));
	g4->transform.setPosition(9, 2, 11);
	g4->transform.setRotation(-90, 0, 0);
	sceneGraph->addEntity(gem4, nullptr);

	Entity* modular10 = new Model("Wall", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m10 = static_cast<Model*>(modular10);
	m10->materialset = &materials[0]->descriptorSet;
	m10->transform.setScale(glm::vec3(3, .1, 1.5));
	m10->transform.setPosition(-9	, 2, -12.0);
	m10->transform.setRotation(glm::vec3(90, 90, 90));
	sceneGraph->addEntity(modular10, nullptr);

	Entity* modular11 = new Model("Wall", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* m11 = static_cast<Model*>(modular11);
	m11->materialset = &materials[0]->descriptorSet;
	m11->transform.setScale(glm::vec3(3, .1, 1.5));
	m11->transform.setPosition(-12, 2, -9.0);
	m11->transform.setRotation(glm::vec3(0, 90, 90));
	sceneGraph->addEntity(modular11, nullptr);

	Entity* c1 = new Model("box1", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* c11 = static_cast<Model*>(c1);
	c11->materialset = &materials[0]->descriptorSet;
	c11->transform.setScale(glm::vec3(.3));
	c11->transform.setPosition(-10, 1.6, -11.0);
	c11->transform.setRotation(glm::vec3(0, 90, 90));
	sceneGraph->addEntity(c1, nullptr);

	Entity* c2 = new Model("box2", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* c22 = static_cast<Model*>(c2);
	c22->materialset = &materials[0]->descriptorSet;
	c22->transform.setScale(glm::vec3(.3));
	c22->transform.setPosition(-10, 1.6, -10.3);
	c22->transform.setRotation(glm::vec3(0, 90, 90));
	sceneGraph->addEntity(c2, nullptr);

	Entity* c3 = new Model("box4", "Aplication\\assets\\column\\wall1.fbx", PBR_COOK_TORRANCE);
	Model* c33 = static_cast<Model*>(c3);
	c33->materialset = &materials[0]->descriptorSet;
	c33->transform.setScale(glm::vec3(.4));
	c33->transform.setPosition(-12, 2, -9.0);
	c33->transform.setRotation(glm::vec3(0, 90, 90));
	sceneGraph->addEntity(c3, nullptr);


}

void Aplication::createSceneMaterials()
{
	materials.reserve(7);
	std::vector<std::string>textures;
	textures.resize(7);

	textures[0] = "Aplication\\assets\\materials\\gold\\albedo.png";
	textures[1] = "Aplication\\assets\\materials\\gold\\normal.png";
	textures[2] = "Aplication\\assets\\materials\\gold\\metallic.png";
	textures[3] = "Aplication\\assets\\materials\\gold\\emissionMap.png";
	textures[4] = "Aplication\\assets\\materials\\gold\\aoMap.png";
	textures[5] = "Aplication\\assets\\materials\\gold\\roughnessMap.png";
	textures[6] = "Aplication\\assets\\materials\\gold\\displacementMap.png";

	std::unique_ptr<TUGEV::Material> goldMaterial(new TUGEV::Material_PBR("GoldMaterial", textures));
	goldMaterial->pbrParameters.albedo = glm::vec3(0);
	goldMaterial->pbrParameters.textureScaleFactor = glm::vec3(2);
	materials.push_back(std::move(goldMaterial));

	//	std::unique_ptr<TUGEV::SceneNode> camera(new TUGEV::SceneNode(new Camera("MainCamera", window, &gameMode)));

	textures[0] = "Aplication\\assets\\materials\\blackFloor\\albedo.png";
	textures[1] = "Aplication\\assets\\materials\\blackFloor\\normal.png";
	textures[2] = "Aplication\\assets\\materials\\blackFloor\\metallic.png";
	textures[3] = "Aplication\\assets\\materials\\blackFloor\\emissionMap.png";
	textures[4] = "Aplication\\assets\\materials\\blackFloor\\aoMap.png";
	textures[5] = "Aplication\\assets\\materials\\blackFloor\\roughnessMap.png";
	textures[6] = "Aplication\\assets\\materials\\blackFloor\\displacementMap.png";

	std::unique_ptr<TUGEV::Material> tileMaterial(new TUGEV::Material_PBR("Tiles", textures));
	tileMaterial->pbrParameters.textureScaleFactor = glm::vec3(1, 2, 0);
	tileMaterial->pbrParameters.albedo = glm::vec3(0);
	tileMaterial->pbrParameters.hasnormalMap = 1;
	tileMaterial->pbrParameters.metallicRoughness = glm::vec3(1);
	materials.push_back(std::move(tileMaterial));

	std::unique_ptr<TUGEV::Material> sideMaterial(new TUGEV::ColorMaterial("BOARD_MATERIAL", "Aplication\\assets\\materials\\border\\border.png"));
	sideMaterial->pbrParameters.albedo = glm::vec3(-.2);
	sideMaterial->pbrParameters.textureScaleFactor = glm::vec3(1 , 20, 0);
	materials.push_back(std::move(sideMaterial));	

	std::unique_ptr<TUGEV::Material> gem1Material(new TUGEV::ColorMaterial("EMISSIVE  BLUE ", "Aplication\\assets\\materials\\dummy\\dummy.png"));
	gem1Material->pbrParameters.albedo = glm::vec3(10, 10, 80);
	gem1Material->pbrParameters.textureScaleFactor = glm::vec3(2);
	materials.push_back(std::move(gem1Material));

	std::unique_ptr<TUGEV::Material> gem2Material(new TUGEV::ColorMaterial("EMISSIVE RED", "Aplication\\assets\\materials\\dummy\\dummy.png"));
	gem2Material->pbrParameters.albedo = glm::vec3(80, 10, 10);
	gem2Material->pbrParameters.textureScaleFactor = glm::vec3(2);
	materials.push_back(std::move(gem2Material));

	std::unique_ptr<TUGEV::Material> gem3Material(new TUGEV::ColorMaterial("EMISSIVE GREEN", "Aplication\\assets\\materials\\dummy\\dummy.png"));
	gem3Material->pbrParameters.albedo = glm::vec3(10, 80, 20);
	gem3Material->pbrParameters.textureScaleFactor = glm::vec3(2);
	materials.push_back(std::move(gem3Material));

	std::unique_ptr<TUGEV::Material> gem4Material(new TUGEV::ColorMaterial("EMISSIVE PURPLE", "Aplication\\assets\\materials\\dummy\\dummy.png"));
	gem4Material->pbrParameters.albedo = glm::vec3(40, 10, 40);
	gem4Material->pbrParameters.textureScaleFactor = glm::vec3(2);
	materials.push_back(std::move(gem4Material));




}


void Aplication::cleanupResources()
{
	std::cout << "Aplication Ended \n";
}

void Aplication::run()
{
	initiateResources();
	update();
	cleanupResources();
}
