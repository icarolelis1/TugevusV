#pragma once

#ifndef MATERIAL
#define MATERIAL

#include "TugevusStructs.h"
#include <iostream>
#include "RenderTools.h"

namespace TUGEV {


	enum class MaterialType {
		PBR,
		COLOR,

	};

	struct PbrParameters {
		alignas(16) glm::vec3	albedo;
		alignas(16)	glm::vec3	textureScaleFactor;
		alignas(16)	glm::vec3   metallicRoughness = glm::vec3(1);
		bool hasnormalMap = 1;
	};

	class Material {

	public:

		Material(const char* name, unsigned int numTextures);
		PbrParameters pbrParameters;
		MaterialType materialType;
		const char* textureBase;
		const char* name;
		virtual void loadResource(DeviceWraper *wraper,unsigned int imageCount, VkDescriptorSetLayout& layout,VkSampler sampler) = 0;
		VkDescriptorSet descriptorSet;
		std::vector<Texture> textures;
		virtual void destroy(VkDevice device) ;
		std::vector<Buffer> uniformBuffers;
		virtual ~Material();
		const VkDevice *device;

	protected:
		bool cleaned = false;
		std::vector<std::string>texturePaths;
	};

	


	class  Material_PBR : public Material {
	public:

		Material_PBR(const char* name, std::vector<std::string>textures);
		void loadResource(DeviceWraper *wraper, unsigned int imageCount, VkDescriptorSetLayout& layout, VkSampler sampler);

		~Material_PBR();
	};

	
	class ColorMaterial :public Material {
	public:

		ColorMaterial(const char* name, const char* texturePath);

		void loadResource(DeviceWraper* wraper, unsigned int imageCount, VkDescriptorSetLayout& layout, VkSampler sampler);

		~ColorMaterial();

	};
	
}

#endif