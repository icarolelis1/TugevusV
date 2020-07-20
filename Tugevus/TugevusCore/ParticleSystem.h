#pragma once

#include "TugevusStructs.h"
#include "Entity.h"
#include <random>

struct Particle
{

	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 velocity;
	alignas(16) glm::vec3 color;


};

enum EmmitterType {
	CIRCLE,
	VERTICAL,
	HORIZONTAL

};

struct Emmiter {
	glm::vec3 position = glm::vec3(0,1,0);
	glm::vec3 velocity;

	EmmitterType type = VERTICAL;


};

class ParticleSystem : public Entity{



public:
	ParticleSystem(DeviceWraper, const char* d, PipelineType p, int numberOfParticles, VkDescriptorSetLayout layout);
	~ParticleSystem();
	void initParticles();
	void updateParticles(float timeStep);

private:
	Buffer particleBuffers;
	std::vector<Buffer> uniformBuffers;
	VkDescriptorSetLayout layout;

	std::default_random_engine rndEngine;
	int numParticles;
	glm::vec3 emitterPosition = glm::vec3(0,1,0);
	std::vector<Particle> particles;
	Emmiter emitter;
	std::vector<VkDescriptorSet> uniformDescriptorSets;

	VkDescriptorSet * particleMaterialSet;
	DeviceWraper deviceWraper;



	float genRandomNumber(float range);
	void createParticleBuffers();
	void allocateDescriptorSet();

	void initiateResources();


protected:
	void start();
	void update(float timeStep);
	void  updateUniformBuffers(unsigned int currentImage, glm::mat4 worldMatrix, glm::mat4* view, glm::mat4 projection, glm::mat4 lightMatrix);
	void  draw(VkCommandBuffer cmd, VkPipeline&, VkPipelineLayout&, unsigned int imageIndex, std::vector<VkDescriptorSet>& descriptorSets);
	void destroy();
};

