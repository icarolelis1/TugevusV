#pragma once
#include "RenderTools.h"
#include "Entity.h"
#include "Model.h"

enum  EntityType {
	DRAWBLE_MESH,
	EMPTY,
	CAMERA
};
namespace TUGEV {



	struct SceneNode {

		SceneNode(Entity* e) :entity(std::move(e)) { parent = nullptr; }

		SceneNode(/*root*/) {}

		std::unique_ptr<Entity> entity;
		std::unique_ptr<SceneNode> parent;

		void update(unsigned int currentImage, glm::mat4 worldMatrix, glm::mat4* view, glm::mat4& projection, glm::mat4& lightMatrix, float timeStep) {

			glm::mat4 w;
			if (parent != nullptr) {
				w = worldMatrix * parent->entity->transform.getModelMatrix();
			}
			else {
				w = worldMatrix;
			}
			entity->updateUniformBuffers(currentImage, w, view, projection, lightMatrix);
			entity->update(timeStep);
		}
		std::vector<std::shared_ptr<SceneNode> > childs;

		void render(VkCommandBuffer cmd, PipelineStc pipeline);

		void addChild(std::shared_ptr<SceneNode> node) {
			childs.push_back(std::move(node));
		}

		~SceneNode() {


		}

	};


	class SceneGraph
	{
	public:


		SceneNode* root;
		void addEntity(Entity* entity,SceneNode * _parent );

		SceneGraph();


		~SceneGraph();
	};


}