#include "Material.h"

namespace TUGEV {


	Material::Material(const char * _name,unsigned int numTextures ):name(_name)  {
		textures.reserve(numTextures);
		textureBase = "assets\\materials\\textureBase\\base.png";


	}
	Material::~Material() {
		if (!cleaned) {
			destroy(*device);
			
		}
	};


	void Material::destroy(VkDevice device) {
		cleaned = true;

		for (int i = 0; i < uniformBuffers.size(); i++) {
			vkFreeMemory(device, uniformBuffers[i].bufferMemory, nullptr);
			vkDestroyBuffer(device, uniformBuffers[i].buffer, nullptr);
		}

		for (int i = 0; i < textures.size(); i++) {
			textures[i].destroy(device);
			//vkDestroyImageView(device, textures[i].view, nullptr);
			//vkDestroyImage(device, textures[i].image, nullptr);
			//vkFreeMemory(device, textures[i].imageMemory, nullptr);
		}

		std::cout << "\n Material : " << name << " destroyed sucessfully\n";
	}

	Material_PBR::Material_PBR(const char* name, std::vector<std::string>_textures) : Material(name, 7) {
		texturePaths = std::move(_textures);
		textures.resize(texturePaths.size());
		materialType = MaterialType::PBR;
	}


	void Material_PBR::loadResource(DeviceWraper* wraper, unsigned int imageCount, VkDescriptorSetLayout& layout, VkSampler sampler) {

		VkDeviceSize size = sizeof(TUGEV::PbrParameters);
		uniformBuffers.resize(imageCount);

		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
		for (unsigned int i = 0; i < imageCount; i++)
			graphicsToolKit::createBuffer(*wraper->physicalDevice, wraper->device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, uniformBuffers[i].buffer, uniformBuffers[i].bufferMemory);


		for (int i = 0; i < texturePaths.size(); i++) {

			graphicsToolKit::createTextureImage(texturePaths[i].c_str(), wraper->physicalDevice, wraper->device, format, VK_IMAGE_TILING_OPTIMAL, textures[i].image, textures[i].imageMemory, 0, 1, *wraper->transferQueue,*wraper->graphicsQueue ,*wraper->graphicsPool,*wraper->transferPool ,textures[i].numMips);
			graphicsToolKit::createImageView(*wraper->device, textures[i].view, textures[i].image, format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, 1, textures[i].numMips);
			textures[i].createSampler(*wraper->device);
		}


		const VkDescriptorSetLayout* materialDescriptorSetLayout = &layout;
		VkDescriptorSetAllocateInfo allocInfo{};

		for (int i = 0; i < 1; i++) {
			allocInfo = initializers::descriptorSetAllocateInfo(*wraper->descriptorPool, materialDescriptorSetLayout, 1);

			VkResult result = vkAllocateDescriptorSets(*wraper->device, &allocInfo, &descriptorSet);
			vkTools::checkVkresult(result, "FTC Allocate(MATERIAL)");

			VkDescriptorImageInfo diffuseTexture = {};
			diffuseTexture.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			diffuseTexture.imageView = textures[0].view;
			diffuseTexture.sampler = textures[0].sampler;

			VkDescriptorImageInfo normalMap = {};
			normalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			normalMap.imageView = textures[1].view;
			normalMap.sampler = textures[1].sampler;

			VkDescriptorImageInfo metallic = {};
			metallic.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			metallic.imageView = textures[2].view;
			metallic.sampler = textures[2].sampler;

			VkDescriptorImageInfo Emission = {};
			Emission.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			Emission.imageView = textures[3].view;
			Emission.sampler = textures[3].sampler;

			VkDescriptorImageInfo aoMap = {};
			aoMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			aoMap.imageView = textures[4].view;
			aoMap.sampler = textures[4].sampler;

			VkDescriptorImageInfo roughnessMap = {};
			roughnessMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			roughnessMap.imageView = textures[5].view;
			roughnessMap.sampler = textures[5].sampler;

			VkDescriptorImageInfo displacementMap = {};
			displacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			displacementMap.imageView = textures[6].view;
			displacementMap.sampler = textures[6].sampler;


			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffers[i].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(TUGEV::PbrParameters);

			std::vector<VkWriteDescriptorSet> writes;
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &diffuseTexture));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &normalMap));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, &metallic));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3, &roughnessMap));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4, &Emission));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 5, &bufferInfo));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 6, &aoMap));
			writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 7, &displacementMap));

			vkUpdateDescriptorSets(*wraper->device, static_cast<uint32_t>(writes.size()), writes.data(), 0, NULL);
		}
	}

	Material_PBR::~Material_PBR()
	{
		if (!cleaned) {
			destroy(*device);
		}
	}


	ColorMaterial::ColorMaterial(const char* name, const char* texture) : Material(name,1)
	{
		this->materialType = TUGEV::MaterialType::COLOR;
		textures.resize(1);
		texturePaths.resize(1);
		texturePaths[0] = texture;

	}

	void ColorMaterial::loadResource(DeviceWraper* wraper, unsigned int imageCount, VkDescriptorSetLayout& layout, VkSampler sampler)
	{

		uniformBuffers.resize(imageCount);

		VkDescriptorSetAllocateInfo allocInfo = initializers::descriptorSetAllocateInfo(*wraper->descriptorPool, &layout,1);

		VkResult result = vkAllocateDescriptorSets(*wraper->device, &allocInfo, &descriptorSet);
		vkTools::checkVkresult(result, "FTC Allocate(Dummymaterial)");

		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
		graphicsToolKit::createTextureImage(texturePaths[0].c_str(),wraper->physicalDevice, wraper->device, format, VK_IMAGE_TILING_OPTIMAL, textures[0].image, textures[0].imageMemory, 0, 1, *wraper->transferQueue, *wraper->graphicsQueue,	*wraper->graphicsPool, *wraper->transferPool,textures[0].numMips);
		graphicsToolKit::createImageView(*wraper->device, textures[0].view, textures[0].image, format, VK_IMAGE_ASPECT_COLOR_BIT,VK_IMAGE_VIEW_TYPE_2D,1,textures[0].numMips);
		textures[0].createSampler(*wraper->device);
		VkDeviceSize size = sizeof(TUGEV::PbrParameters);
		for(unsigned int i=0;i<imageCount;i++)
		graphicsToolKit::createBuffer(*wraper->physicalDevice, wraper->device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, uniformBuffers[i].buffer, uniformBuffers[i].bufferMemory);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[0].buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(TUGEV::PbrParameters);

		VkDescriptorImageInfo col = {};
		col.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		col.imageView = textures[0].view;
		col.sampler = textures[0].sampler;

		std::vector<VkWriteDescriptorSet> writes;
		writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &col));
		writes.push_back(initializers::writeDescriptorSet(descriptorSet, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, &bufferInfo));

		vkUpdateDescriptorSets(*wraper->device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
	}

	ColorMaterial::~ColorMaterial()
	{
		if (!cleaned)destroy(*device);
	}

}