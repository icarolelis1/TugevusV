#include "Model.h"

/*
Model::Model(int imageCount, const char* name, const char* path, const VkDevice* dev, const VkPhysicalDevice* phy, const VkCommandPool* pool, const VkQueue* queue, VkDescriptorSetLayout setLayout, PipelineType p,const VkDescriptorPool *descPool):Entity(name,p), device(dev), physicalDevice(phy), transientPool(pool), transferQueue(queue), descriptorPool(descPool), uniformDescriptorSetLayout(setLayout)
{
	uniforms.resize(imageCount);
	descriptorSets.resize(imageCount);
	diskFile = path;
	this->pipelineType = p;
	std::cout << diskFile << std::endl;
	assert(device);
	assert(transientPool);
}*/

/*

Model::Model(int imageCount, const char* name, const char* path, DeviceWraper  wraper, VkDescriptorSetLayout layout, PipelineType p) : Entity(name, p),deviceWraper(wraper),uniformDescriptorSetLayout(layout)
{
	uniforms.resize(imageCount);
	descriptorSets.resize(imageCount);
	diskFile = path;
	this->pipelineType = p;

}

*/

Model::Model(const char* name, const char* path, PipelineType pipeline):Entity(name,pipeline),diskFile(path )
{

}

Model::~Model()
{
	destroy();
}

void Model::initiateResources(unsigned int imageCount, DeviceWraper wraper, VkDescriptorSetLayout uniformLayout)
{

	deviceWraper = wraper;
	uniformDescriptorSetLayout = uniformLayout;

	uniforms.resize(imageCount);
	descriptorSets.resize(imageCount);

	assert(deviceWraper.device != nullptr);
	
	static const int assimpFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices ;
	scene = importer.ReadFile(diskFile, assimpFlags);
	assert(scene != nullptr);

	loadMeshes();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers();
	allocateDescriptorSet();

}

void Model::createVertexBuffer()
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;
	const VkPhysicalDevice* physicalDevice = deviceWraper.physicalDevice;
	const VkDevice* device = deviceWraper.device;

	VkDeviceSize size = sizeof(Vertex) * vertices.size();
	graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

	void *data;
	vkMapMemory(*device, stagingMemory, 0, size, 0, &data);
	memcpy(data, vertices.data(), (size_t)size);
	vkUnmapMemory(*device, stagingMemory);

	graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer.buffer, vertexBuffer.bufferMemory);
	
	graphicsToolKit::copyBuffer(*deviceWraper.transferPool, *device, stagingBuffer, vertexBuffer.buffer, size,deviceWraper.transferQueue);

	vkFreeMemory(*device, stagingMemory, nullptr);
	vkDestroyBuffer(*device, stagingBuffer, nullptr);


}

void Model::createIndexBuffer()
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;

	const VkPhysicalDevice* physicalDevice = deviceWraper.physicalDevice;
	const VkDevice* device = deviceWraper.device;


	VkDeviceSize size = sizeof(indices[0]) * indices.size();
	graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

	void *data;
	vkMapMemory(*device, stagingMemory, 0, size, 0, &data);
	memcpy(data, indices.data(), (size_t)size);
	vkUnmapMemory(*device, stagingMemory);

	graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer.buffer, indexBuffer.bufferMemory);

	graphicsToolKit::copyBuffer(*deviceWraper.transferPool, *device, stagingBuffer, indexBuffer.buffer, size, deviceWraper.transferQueue);

	vkFreeMemory(*device, stagingMemory, nullptr);
	vkDestroyBuffer(*device, stagingBuffer, nullptr);
}

void Model::loadMeshes()
{
	meshes.resize(scene->mNumMeshes);
	uint32_t indexBase = 0;
	uint32_t vertexOffset = 0;
	vertices.clear();
	for (unsigned int i = 0; i < meshes.size(); i++) {
		aiMesh *aMesh = scene->mMeshes[i];

		meshes[i].indexBase = indexBase;
		meshes[i].vertexOffset = vertexOffset;
		meshes[i].indexCount = aMesh->mNumFaces * 3;
		//meshes[i].material = materials;
		if (aMesh->HasBones()) {
			std::cout << "TEM OSSO\n";
		}
		//bool hasUV = aMesh->HasTextureCoords(i);
		//bool hasNormals = aMesh->HasNormals();
		bool hasUV = aMesh->HasTextureCoords(0);

		for (size_t j = 0; j < aMesh->mNumVertices; j++){

			Vertex vertex;
			vertex.pos = glm::make_vec3(&aMesh->mVertices[j].x);
		//	vertex.pos.y = -vertex.pos.y;
			vertex.uv = hasUV ? glm::make_vec2(&aMesh->mTextureCoords[0][j].x) : glm::vec2(0.0f);


			//if(!hasUV) std::cout << "No uv" << std::endl;

			vertex.uv.y *= -1;
			//vertex.uv.y *= -1;
			
			vertex.normal = glm::make_vec3(&aMesh->mNormals[j].x);
			//vertex.normal.y = -vertex.normal.y;
			//vertex.color = hasColor ? glm::make_vec3(&aMesh->mColors[0][v].r) : glm::vec3(1.0f);

			vertices.push_back(vertex);
		}
		vertexOffset += aMesh->mNumVertices;

		for (size_t f = 0; f < aMesh->mNumFaces; f++) {

			for (uint32_t j = 0; j < 3; j++)
			{
				indices.push_back(aMesh->mFaces[f].mIndices[j]);
			}
		}
		indexBase += aMesh->mNumFaces * 3;
	}
	//loadBones 
	//...
}

void Model::allocateDescriptorSet()
{
	const VkDevice* device = deviceWraper.device;
	const VkDescriptorPool* descriptorPool = deviceWraper.descriptorPool;

	std::vector<VkDescriptorSetLayout> layouts(descriptorSets.size(), uniformDescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo = initializers::descriptorSetAllocateInfo(*descriptorPool, layouts.data(), layouts.size());

	VkResult result = vkAllocateDescriptorSets(*device,&allocInfo, descriptorSets.data());
	vkTools::checkVkresult(result, "FTC Allocate(MODEL)");

	for (unsigned int i = 0; i < uniforms.size(); i++) {

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniforms[i].buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(MVPMatrices);

		std::vector<VkWriteDescriptorSet> writes;
		writes.push_back(initializers::writeDescriptorSet(descriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &bufferInfo));

		vkUpdateDescriptorSets(*device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
	}
}

void Model::createUniformBuffers() {

	VkDeviceSize size = sizeof(MVPMatrices);
	VkDeviceSize size_Material = sizeof(Pbr_Material);
	
	const VkDevice* device = deviceWraper.device;
	const VkPhysicalDevice* physicalDevice = deviceWraper.physicalDevice;

	for (unsigned int i = 0; i < uniforms.size(); i++) {

		graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, uniforms[i].buffer, uniforms[i].bufferMemory);
		
	}

}

void Model::updateUniformBuffers(unsigned int currentImage,glm::mat4 worldMatrix, glm::mat4 *view, glm::mat4 projection, glm::mat4 lightMatrix) {
	
	MVPMatrices ubo;

	ubo.model = worldMatrix *this->transform.getModelMatrix();
	ubo.view = view[0];
	ubo.projection = projection;
	ubo.lightMatrix = lightMatrix;

	float t = fmod((.03f*(float)glfwGetTime()), 1.0);
	ubo.time = t;


	void *data;
	vkMapMemory(*deviceWraper.device, uniforms[currentImage].bufferMemory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(*deviceWraper.device, uniforms[currentImage].bufferMemory);

}



PipelineType Model::getPipelineType() {

	return pipelineType;
}

void Model::draw(VkCommandBuffer commandBuffers, VkPipeline &pipeline, VkPipelineLayout& pipelinelayout,unsigned int imageIndex, std::vector<VkDescriptorSet>& d)
{

	VkDeviceSize offsets[1] = { 0 };

	vkCmdBindIndexBuffer(commandBuffers, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	vkCmdBindVertexBuffers(commandBuffers, 0, 1, &vertexBuffer.buffer, offsets);

	if(instanceCount>1)
	vkCmdBindVertexBuffers(commandBuffers, 1, 1, &instanceBuffer.buffer, offsets);

	if (pipelineType == PBR_COOK_TORRANCE) {
		std::array<VkDescriptorSet, 2>sets = { descriptorSets[imageIndex] , *materialset };
		vkCmdBindPipeline(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdBindDescriptorSets(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);

		for (unsigned int i = 0; i < meshes.size(); i++) {

		
			vkCmdDrawIndexed(commandBuffers, meshes[i].indexCount, 1, static_cast<uint32_t>(meshes[i].indexBase), meshes[i].vertexOffset, 0);

		
		}
	}

	else if (pipelineType == WATER_DISPLACEMENT) {

		for (unsigned int i = 0; i < meshes.size(); i++) {
			std::array<VkDescriptorSet, 3>sets = { descriptorSets[imageIndex],d[0],d[1] };
			vkCmdBindPipeline(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			vkCmdBindDescriptorSets(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
			vkCmdDrawIndexed(commandBuffers, meshes[i].indexCount, 1, static_cast<uint32_t>(meshes[i].indexBase), meshes[i].vertexOffset, 0);

		}

	}
	else if (pipelineType == DUMMY) {

		for (unsigned int i = 0; i < meshes.size(); i++) {

			std::array<VkDescriptorSet, 2>sets = { descriptorSets[imageIndex], *materialset };
			vkCmdBindPipeline(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			vkCmdBindDescriptorSets(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
			vkCmdDrawIndexed(commandBuffers, meshes[i].indexCount, 1, static_cast<uint32_t>(meshes[i].indexBase), meshes[i].vertexOffset, 0);

		}

	}
}

void Model::drawGeometryOnly(VkCommandBuffer commandBuffers, PipelineStc pipeline,int imageIndex, std::vector<VkDescriptorSet>&shadowDescriptorSets)
{
	VkDeviceSize offsets[1] = { 0 };

	vkCmdBindIndexBuffer(commandBuffers, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindVertexBuffers(commandBuffers,0, 1, &vertexBuffer.buffer, offsets);

	for (unsigned int i = 0; i < meshes.size(); i++) {

		std::array<VkDescriptorSet, 2 >sets ={ *materialset	,descriptorSets[imageIndex] };
		vkCmdBindPipeline(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
		vkCmdBindDescriptorSets(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
		vkCmdDrawIndexed(commandBuffers, meshes[i].indexCount, 1, static_cast<uint32_t>(meshes[i].indexBase), meshes[i].vertexOffset, 0);
	}
}

void Model::renderShadowMap(VkCommandBuffer commandBuffers, VkPipeline pipeline, VkPipelineLayout pipelinelayout,int imageIndex, VkDescriptorSet& materialSet)
{
	VkDeviceSize offsets[1] = { 0 };

	vkCmdBindIndexBuffer(commandBuffers, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindVertexBuffers(commandBuffers, 0, 1, &vertexBuffer.buffer, offsets);

	for (unsigned int i = 0; i < meshes.size(); i++) {

		std::array<VkDescriptorSet, 1 >sets = { descriptorSets[imageIndex] };
		vkCmdBindPipeline(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdBindDescriptorSets(commandBuffers, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
		vkCmdDrawIndexed(commandBuffers, meshes[i].indexCount, 1, static_cast<uint32_t>(meshes[i].indexBase), meshes[i].vertexOffset, 0);
	}
}



void Model::update(float timeStep)
{
	Entity::update(timeStep);
}

void Model::start()
{
	Entity::start();
}

void Model::destroy()
{
	unsigned int i = -3;
	printf("Entity %s destroyed", name);
	printf("\n");
	const VkPhysicalDevice* physicalDevice = deviceWraper.physicalDevice;
	const VkDevice* device = deviceWraper.device;

	for (unsigned int i = 0; i < uniforms.size(); i++) {
		vkDestroyBuffer(*device, uniforms[i].buffer, nullptr);
		vkFreeMemory(*device, uniforms[i].bufferMemory, nullptr);

	}

	vkDestroyBuffer(*device, vertexBuffer.buffer, nullptr);
	vkDestroyBuffer(*device, indexBuffer.buffer, nullptr);

	vkFreeMemory(*device ,vertexBuffer.bufferMemory, nullptr);
	vkFreeMemory(*device, indexBuffer.bufferMemory, nullptr);
	

}

