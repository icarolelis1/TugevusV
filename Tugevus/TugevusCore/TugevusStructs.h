#pragma once
#include "VulkanDefinition.h"
#include <glm/mat4x4.hpp>
#include <array>
#include <map>
#include <vector>
#include <string.h>

enum PipelineType {
	GEOMETRY_ONLY,
	PBR_COOK_TORRANCE,
	COOK_TORRANCE2,
	QUAD_PIPELINE,
	WATER_DISPLACEMENT,
	NONE,
	DUMMY,
	PARTICLE,
};


struct Pbr_Material {
	alignas(16) glm::vec3	albedo;
	alignas(16)	glm::vec3	textureScaleFactor;
	alignas(16)	glm::vec3 metallicRoughness;
	alignas(16) glm::vec3  parallax;

	bool hasNormalMap;

};

struct Pass {
	VkRenderPass renderpass;
};
struct Buffer {
	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	void destroy(const VkDevice device) {
		std::cout << "Buffer destroyed\n";
		vkFreeMemory(device, bufferMemory, nullptr);
		vkDestroyBuffer(device, buffer, nullptr);
	}
};


struct SwapChainQuery {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct SwapChainExt {
	VkSurfaceFormatKHR format;
	VkPresentModeKHR presentMode;
	VkSurfaceCapabilitiesKHR capabilities;
	uint32_t minImageCount;
	VkExtent2D extent;
	VkSharingMode sharingMode;
	VkSwapchainKHR spChain;
};

struct QueueSharingMode{
	VkSharingMode sharingMode;
	uint32_t queueFamiliyIndexCount;
	const uint32_t* queueFamilies;
};

struct ImagesExt {
	std::vector<VkImage> images;
	std::vector<VkImageView> views;
	std::vector<VkDeviceMemory> memories;
};

struct ImageStr {
	VkImage image;
	VkImageView view;
	VkDeviceMemory memory;
};

struct InstanceData {
	alignas(16) glm::vec3 position;
	alignas(16) glm::vec3 rotation;
	alignas(16) glm::vec3 scale;

};

struct PipelineStc {
	VkPipeline pipeline;
	VkPipelineLayout pipelinelayout;
	std::vector<VkDescriptorSetLayout> descriptorSetLayout;
};
struct UniformSkybox {

	glm::mat4 projection;
	glm::mat4 view;
	float time;
};

struct skyUniform {
	alignas(16) glm::vec3 cameraDir;
	alignas(16) glm::vec3 sunDir;
};
struct PbrTextureFiles {
	const char *DIFFUSE;
	const char *NORMAL;
	const char *ROUGHNES;
	const char *METALLIC;
	const char *EMISSIVE;
};

struct Texture {
	VkImage image;
	VkImageView view;
	VkDeviceMemory imageMemory;
	uint32_t numMips;
	VkSampler sampler;

	void createSampler(const VkDevice device) {

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.maxLod = static_cast<uint32_t>(numMips);
		samplerInfo.minLod = 0;
		samplerInfo.mipLodBias = .0f;
		if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}


	void destroy(const VkDevice device) {

		vkDestroyImageView(device, view, nullptr);
		vkDestroyImage(device, image, nullptr);
		vkFreeMemory(device, imageMemory, nullptr);

		vkDestroySampler(device, sampler, nullptr);
	}
};

struct DeviceWraper {
	const VkDevice* device;
	const VkPhysicalDevice* physicalDevice;

	const VkQueue *graphicsQueue;
	const VkQueue *transferQueue;
	const VkQueue *computeQueue;

	const VkCommandPool* graphicsPool;
	const VkCommandPool* transferPool;
	const VkCommandPool* computePool;
	
	const VkDescriptorPool *descriptorPool;

	int numImages;

};



struct Mesh {
	uint32_t vertexOffset;
	VkDescriptorSet *material;
	uint32_t indexBase;
	uint32_t indexCount;
};



struct MVPMatrices {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 lightMatrix;
	float time ;
};

struct ProjView {

	glm::mat4 projection;
	glm::mat4 view;
};

struct ShadowMatrix {
	glm::mat4 lightMatrix;
};

struct toneMappingUbo {
	float exposure;
};

struct lightUbo {
alignas(16)	glm::vec3 lightPos;
alignas(16)	glm::vec3 lightColor;
float type;
float factor = .02;

};

struct lightContainer {

	glm::mat4 lightMatrix;
	glm::mat4 invView;
	glm::mat4 invProjection;
	glm::vec3 eyePos;
	lightUbo lights[5];
	int numberOfLights;

};
struct {
	alignas(16)glm::vec3 samples[100];
}rndsamplesStc;

enum Atributes {
	FLOAT,
	VEC4,
	VEC2,
	VEC3

};

struct RenderAttachment {

	VkImage image;
	VkImageView view;
	VkDeviceMemory memory;
	VkFormat format;
	VkSampler sampler;
	uint32_t numMips;
	void createSampler(const VkDevice device) {
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.maxLod = static_cast<uint32_t>(numMips);

		if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}
	void destroy(const VkDevice device) {
		vkFreeMemory(device, memory, nullptr);
		vkDestroyImageView(device, view, nullptr);
		vkDestroyImage(device, image, nullptr);
	}
};


struct VertexLayout {
	public:std::vector<Atributes> atributes;

		VertexLayout(std::vector<Atributes> atribs, std::vector<uint32_t>  vertexOffsets , uint32_t vertexBindingCount = 1) :numberOfBindings(vertexBindingCount) {
		atributes.reserve(atribs.size());
		offsets = (vertexOffsets);

		assert(offsets.size() == vertexBindingCount);

		for (auto t : atribs) {
			atributes.push_back(t);
		}
	}
	uint32_t numberOfBindings;
	std::vector<uint32_t> offsets;


	uint32_t getVertexStride() {
		size_t res = 0;
		uint32_t firstOffset = 0;

		for (uint32_t  c  = 0; c < numberOfBindings; c ++) {

			for (uint32_t i = firstOffset; i < offsets[c]; i++) {

				switch (atributes[i])
				{
				case VEC2:
					res += 2 * sizeof(float);
					break;
				case VEC4:
					res += 4 * sizeof(float);
					break;
				case VEC3:
					res += 3 * sizeof(float);
					break;

				}
			}
			firstOffset = offsets[c];
		}

		/*for (auto& component : atributes)
		{
			switch (component)
			{
			case VEC2:
				res += 2 * sizeof(float);
				break;
			case VEC4:
				res += 4 * sizeof(float);
				break;
			case VEC3:
				res += 3 * sizeof(float);
				break;

			}
		}*/

		return res;
	}


	VkVertexInputBindingDescription getBinding(int bindingNumber) {
		VkVertexInputBindingDescription binding;
		binding.binding =static_cast<uint32_t>(bindingNumber);
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		binding.stride = getVertexStride();

		return binding;
	}
	void getAttributeDescription(uint32_t bindingNumber, std::vector<VkVertexInputAttributeDescription> &atributeDescriptions) {
		uint32_t offset = 0;
		int i = 0;
		for (auto attrib : atributes) {
			VkVertexInputAttributeDescription atribute{};
			atribute.binding = bindingNumber;

			switch (attrib) {
			case(VEC2):
				atribute.format = VK_FORMAT_R32G32_SFLOAT;
				atribute.location = static_cast<uint32_t>(i++);
				atribute.offset = offset;
				atributeDescriptions.push_back(atribute);
				offset += sizeof(float) * 2;
				break;
			case(VEC3):
				atribute.format = VK_FORMAT_R32G32B32_SFLOAT;
				atribute.location = static_cast<uint32_t>(i++);
				atribute.offset = offset;
				atributeDescriptions.push_back(atribute);
				offset += sizeof(float) * 3;
				break;
			case(VEC4):
				atribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
				atribute.location = static_cast<uint32_t>(i++);
				atribute.offset = offset;
				atributeDescriptions.push_back(atribute);
				offset += sizeof(float) * 4;
				break;

		}
	}

	}
};


enum AttachmentType {
	SCRIPT,
	COLLISOR
};

