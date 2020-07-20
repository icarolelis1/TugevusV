#include "SceneGraph.h"

namespace TUGEV {


	SceneGraph::~SceneGraph() { std::cout << "SceneGraph destroyed\n"; }


	SceneGraph::SceneGraph() { root = new SceneNode(); }

	void 	SceneGraph::addEntity(Entity* entity, SceneNode* _parent = nullptr ) {

	//	if (_parent == nullptr)_parent = root;

		root->addChild(std::move(std::unique_ptr<TUGEV::SceneNode>(std::make_unique<SceneNode>(entity))));
	}



}