#pragma once
#include<iostream>
#include "RenderTools.h"

#include "Entity.h"
#include <array>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/vec4.hpp>
#include <assimp/postprocess.h>
#include "TugevusStructs.h"
#include "stb_image.h"
#include <assimp/pbrmaterial.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <algorithm>

#ifndef GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#endif

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;


};




class Model:public Entity
{
public:
	//Model(int imageCount, const char* name, const char* path, DeviceWraper wraper, VkDescriptorSetLayout layout, PipelineType p);
	Model(const char* name,  const char* path, PipelineType pipeline);
	void updateUniformBuffers(unsigned int currentImage, glm::mat4 worldMatrix, glm::mat4 *view, glm::mat4 projection, glm::mat4 lightMatrix);
	PipelineType getPipelineType();
	~Model();
	void  renderShadowMap(VkCommandBuffer cmd, VkPipeline pipeline, VkPipelineLayout pipelinelayout, int imageIndex, VkDescriptorSet& lightDescriptorSet);
	void  renderAO(VkCommandBuffer cmd, VkPipeline pipeline, VkPipelineLayout pipelinelayout, int imageIndex, VkDescriptorSet& gBufferSet);

	VkDescriptorSet* materialset;


private:

	int instanceCount = 0;
	DeviceWraper deviceWraper;
	Buffer vertexBuffer;
	Buffer instanceBuffer;
	Buffer indexBuffer;

	Model* modelSource;

	std::vector<Buffer> uniforms;


	//const VkDevice *device;
	//const VkPhysicalDevice *physicalDevice;
	//const VkQueue *transferQueue;
	//const VkCommandPool *transientPool;
	//const VkDescriptorPool *descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	VkDescriptorSetLayout uniformDescriptorSetLayout;

	const VkCommandPool *commandPool;

	void initiateResources(unsigned int imageCount, DeviceWraper wraper,VkDescriptorSetLayout layout);
	void createVertexBuffer();
	void createIndexBuffer();
	void loadMeshes();
	void allocateDescriptorSet();
	void createUniformBuffers();
	void buildDrawCmdBuffer(VkCommandBuffer commandBuffers, PipelineStc &pipeline,unsigned int imageIndex, std::vector<VkDescriptorSet>&lightDescriptorSets);
	void draw(VkCommandBuffer commandBuffers, PipelineStc pipeline,unsigned int imageIndex, std::vector<VkDescriptorSet>&lightDescriptorSets, VkDescriptorSet &shadowMapDescriptorSet);
	void draw(VkCommandBuffer cmd, VkPipeline& r, VkPipelineLayout& pl, unsigned int imageIndex, std::vector<VkDescriptorSet>& descriptorSets);
	void  drawGeometryOnly(VkCommandBuffer cmd, PipelineStc pipeline, int imageIndex, std::vector<VkDescriptorSet>&shadowDescriptorSets);


	void update(float timeStep);
	void start();
	void destroy();

	const aiScene	*scene;
	Assimp::Importer	importer;
	const char *diskFile;
	std::vector<Mesh> meshes;
	std::vector<Vertex> vertices;
	std::vector<uint32_t>indices;
	std::vector<std::string> texturePaths;
};

