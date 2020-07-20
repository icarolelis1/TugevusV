#pragma once

#include <iostream>
#include <string>
#include "RenderTools.h"
#include "Transform.h"
#include "RenderTools.h"
#include "Attachment/Component.h"
#include <list>
#include <map>

struct ComponentContainer {

public:
	ComponentContainer() {};
	std::map<std::string, Component*> components;

	Component *getComponent(std::string name) {
		std::map<std::string, Component*> ::iterator it;
			return components[name];
	}
	void addComponenet(Component *component, std::string name) {
		if (components.count(name) > 0) {
			name.append(std::to_string(components.count(name)+1));
		}
		components[name] = component;
	}
	bool checkIfExists(std::string n) {
		return components.find(n) != components.end();
	}


};
class Entity
{
public:

	Entity(const char *_name, PipelineType _pipelineType = NONE): name(_name),pipelineType(_pipelineType) {};
	Entity(const Entity& other) {};
	~Entity() {  };

	virtual void  start() {

		std::map<std::string, Component*>::iterator it;
		it = componentContainer.components.begin();
		while (it != componentContainer.components.end()) {
			it->second->start();
			it++;
		}
	};

	virtual void  update(float timeStep) { 

		std::map<std::string, Component*>::iterator it;
		it = componentContainer.components.begin();
		while (it != componentContainer.components.end()) {
			it->second->update(timeStep);
			it++;
		}


	};

	virtual void  updateUniformBuffers(unsigned int currentImage, glm::mat4 worldMatrix, glm::mat4 *view, glm::mat4 projection,glm::mat4 lightMatrix) { return; };

	virtual void  destroy() = 0;

	virtual void  draw(VkCommandBuffer cmd, VkPipeline&,VkPipelineLayout&, unsigned int imageIndex, std::vector<VkDescriptorSet>& descriptorSets) { return; }

	virtual void initiateResources(unsigned int imageCount, DeviceWraper wraper, VkDescriptorSetLayout uniformLayout) { return; };

	virtual void initiateResources() { return; };

	virtual void  drawGeometryOnly(VkCommandBuffer cmd, PipelineStc pipeline, int imageIndex, std::vector<VkDescriptorSet>&shadowDescriptorSets) { return; };
	virtual void attachComponent(Component* component) { const char* n = component->name; componentContainer.addComponenet(component, n);}
	virtual Component* getComponent(std::string name) { return componentContainer.getComponent(name); };
	virtual Component* getComponentIfExists(std::string name) { if(!componentContainer.checkIfExists(name))return nullptr; return componentContainer.getComponent(name); };

	const char *name;
	void virtual printName() { std::cout << "Name : " <<name << std::endl; };
	Transform transform;
	PipelineType getPipelineType() {
		return pipelineType;
	}

protected:
	PipelineType pipelineType;
	ComponentContainer componentContainer;
};
	
