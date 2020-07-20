#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(DeviceWraper deviceW, const char* d, PipelineType p,  int numberOfParticles, VkDescriptorSetLayout l): Entity(d,p),numParticles(numberOfParticles),deviceWraper(deviceW),layout(l)
{
}

ParticleSystem::~ParticleSystem()
{
	destroy();
}

void ParticleSystem::initParticles()
{


	for (int i = 0; i < numParticles; i++) {

		if (emitter.type == VERTICAL) {
			Particle p;
			p.Position = glm::vec3(-genRandomNumber(1),genRandomNumber(4),genRandomNumber(1));
			p.velocity = glm::vec3(-genRandomNumber(.1),genRandomNumber(.9),genRandomNumber(.1));
			p.color = glm::vec3(200,40,40);

			p.Position += emitter.position;
			particles.push_back(p);
		}
	}
}

void ParticleSystem::updateParticles(float timeStep)
{
	emitter.position = transform.getPosition();

	for (auto& particle :particles) {
		particle.Position += particle.velocity * timeStep * .0018f;

		if ((abs(particle.Position.y- emitter.position.y)) > 7) {
			particle.Position = emitter.position;
		}

	}

	void* p;
	VkDeviceSize size = sizeof(Particle) * numParticles;

	vkMapMemory( *deviceWraper.device, particleBuffers.bufferMemory, 0, size, 0, &p);
	memcpy(p, particles.data(), size);
	vkUnmapMemory(*deviceWraper.device, particleBuffers.bufferMemory);

} 

float ParticleSystem::genRandomNumber(float range)
{
	std::uniform_real_distribution<float> rndDist(0.0f, range);
	return rndDist(rndEngine);
}

void ParticleSystem::createParticleBuffers()
{


	VkDeviceSize size = sizeof(Particle) * numParticles;

	graphicsToolKit::createBuffer(*deviceWraper.physicalDevice, deviceWraper.device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, particleBuffers.buffer,particleBuffers.bufferMemory);

	VkDeviceSize sizeUbo = sizeof(ProjView);
	uniformBuffers.resize(deviceWraper.numImages);

	for (unsigned int i = 0; i < uniformBuffers.size(); i++) {

		graphicsToolKit::createBuffer(*deviceWraper.physicalDevice, deviceWraper.device, sizeUbo, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, uniformBuffers[i].buffer, uniformBuffers[i].bufferMemory);

	}
}



void ParticleSystem::allocateDescriptorSet()
{
	
	uniformDescriptorSets.resize(deviceWraper.numImages);

	std::vector<VkDescriptorSetLayout> layouts(deviceWraper.numImages , layout);
	VkDescriptorSetAllocateInfo allocInfo = initializers::descriptorSetAllocateInfo(*deviceWraper.descriptorPool, layouts.data(), layouts.size());
	VkResult result = vkAllocateDescriptorSets(*deviceWraper.device, &allocInfo, uniformDescriptorSets.data());
	vkTools::checkVkresult(result, "FTC Allocate(PARTICLES)");

	for (int i = 0; i < uniformDescriptorSets.size(); i++) {

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i].buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(ProjView);

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

							initializers::writeDescriptorSet(uniformDescriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &bufferInfo),

		};

		vkUpdateDescriptorSets(*deviceWraper.device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}

}


void ParticleSystem::initiateResources()
{
	createParticleBuffers();
	allocateDescriptorSet();
}

void ParticleSystem::start()
{
	Entity::start();
	initParticles();
	initiateResources();
	
}

void ParticleSystem::update(float timeStep)
{
	Entity::update(timeStep);
	updateParticles(timeStep);
}

void ParticleSystem::updateUniformBuffers(unsigned int currentImage, glm::mat4 worldMatrix, glm::mat4* view, glm::mat4 projection, glm::mat4 lightMatrix)
{
	ProjView proj{};
	proj.view = view[0];
	proj.projection = projection;
	void* p;

	vkMapMemory(*deviceWraper.device, uniformBuffers[currentImage].bufferMemory, 0, sizeof(ProjView), 0, &p);
	memcpy(p, &proj, sizeof(ProjView));
	vkUnmapMemory(*deviceWraper.device, uniformBuffers[currentImage].bufferMemory);

}

void ParticleSystem::draw(VkCommandBuffer cmd, VkPipeline& pipeline, VkPipelineLayout& layout, unsigned int imageIndex, std::vector<VkDescriptorSet>& descriptorSets)
{
	VkDeviceSize offsets[1] = { 0 };

	VkDescriptorSet sets[2] = { uniformDescriptorSets[imageIndex],descriptorSets[0] };

	vkCmdBindVertexBuffers(cmd, 0, 1, &particleBuffers.buffer ,offsets);
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0,2, sets, 0, NULL);
	vkCmdDraw(cmd, numParticles, 1, 0, 0);

}

void ParticleSystem::destroy()
{
	vkFreeMemory(*deviceWraper.device, particleBuffers.bufferMemory, nullptr);
	vkDestroyBuffer(*deviceWraper.device, particleBuffers.buffer, nullptr);

	for (int i = 0; i < uniformBuffers.size(); i++) {

		vkFreeMemory(*deviceWraper.device, uniformBuffers[i].bufferMemory, nullptr);
		vkDestroyBuffer(*deviceWraper.device, uniformBuffers[i].buffer, nullptr);
	}
}
