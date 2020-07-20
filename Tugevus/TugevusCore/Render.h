#pragma once

#include <iostream>
#include <memory>
#include "Debuger.h"
#include <stdexcept>
#include "TugevusStructs.h"
#include <array>
#include "PresentationEngine.h"
#include "Model.h"
#include "SceneGraph.h"
#include "Device.h"
#include "InputCallBack.h"
#include "imgui.h"

#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"
#include "gameCode/GameMaster.h"
#include <random>
#include "Camera/TugevCamera.h"
#include "material/Material.h"
#include "glm/gtx/quaternion.hpp"

#define RENDERDOC
const bool enableValidationlayer = true;
const int MAX_FRAMES = 2;
static  int currentFrame = 0;

using Scenegraph = std::shared_ptr<TUGEV::SceneGraph>;
using Materials = std::vector<std::unique_ptr<TUGEV::Material> >;

class Render
{
public:
	Render(const char* appName, GLFWwindow * window);
	void initiateResources();
	void waitIdle();
	void setSceneGraph(Scenegraph  &_sceneGraph);
	void setSceneMaterials(Materials& _materials);
	void recreateSwapChain();
	float frameTime;

	~Render();
	void draw();

private:
	VkSampler textSampler;
	float lastFrameTIme = .0;


	glm::vec3 lightOffset;
	Materials materials;

	std::vector<RenderAttachment> frameBufferAttachments;
	std::vector<RenderAttachment> lumenColorImage;
	std::vector<RenderAttachment> verticalBlurImage;

	RenderAttachment depthMap;
	VkSampler samplerBloom;
	VkSampler shadowMapSampler;
	glm::vec3 viewPos = glm::vec3(5, 20, 40);
	glm::vec3 offsetPos = glm::vec3(5, 20, 40);
	glm::vec4 ortho = glm::vec4(-20.f, 20.f, -20.f, 20.f);
	glm::vec2 nearFar = glm::vec2(1.f, 100.0f);

	//Bloom
	VkRenderPass verticalBlurRenderPass;
	std::vector< RenderAttachment>colorImageAttachment;
	std::vector<VkFramebuffer> verticalBlurFrameBuffer;
	VkRenderPass horizontalBlurRenderPass;
	std::vector<VkFramebuffer>horizontalBlurFrameBuffer;
	bool gameMode = 0;

	//Shadow Map
	struct RSM{
	
		std::vector<VkFramebuffer>frameBuffers;
		VkRenderPass renderpass;

	
		VkSampler sampler;

		void destroy(const VkDevice device) {
		
			vkDestroySampler(device, sampler, nullptr);
		}

	}shadowMapPass;


	struct SceneSettings {

		float saturation;
		float contrast;
		float brightness;
		float exposure;

		void readSettings() {};

		void startSceneSettings() {



			contrast = 1.885f;
			brightness = .318f;
			saturation = 1.94f;
			exposure = 3.1f;
		};

	}sceneSettings;

	struct ImGuIResources {

		std::vector<VkCommandPool> cmdPool;
		std::vector<VkCommandBuffer> cmdBuffers;
		std::vector<VkFramebuffer> framebuffer;
		VkRenderPass renderpass;
		RenderAttachment imguiAtt;
		bool change_sky_Event = false;

		void destroy(const VkDevice* device, const VkQueue& queue, VkInstance* instance, GLFWwindow* window) {

			vkDeviceWaitIdle(*device);

			ImGui::DestroyContext();
			ImGui_ImplGlfw_Shutdown();
			ImGui_ImplVulkan_Shutdown();

			for (int i = 0; i < framebuffer.size(); i++)
				vkDestroyFramebuffer(*device, framebuffer[i], nullptr);

			vkFreeMemory(*device, imguiAtt.memory, nullptr);
			vkDestroyImageView(*device, imguiAtt.view, nullptr);
			vkDestroyImage(*device, imguiAtt.image, nullptr);

			vkDestroyRenderPass(*device, renderpass, nullptr);
			vkDestroyCommandPool(*device, cmdPool[0], nullptr);

		}
	}guiResources;

	lightContainer sceneLights;


	struct deferredRenderingResources {
		std::vector< RenderAttachment>normal;
		std::vector< RenderAttachment>position;
		std::vector< RenderAttachment>material;
		std::vector< RenderAttachment>albedo;
		std::vector< RenderAttachment>ao;

		std::vector<VkFramebuffer> framebuffers;
		VkRenderPass renderpass;
		void destroy(const VkDevice* device) {
			for (int i = 0; i < position.size(); i++) {

				vkFreeMemory(*device, normal[i].memory, nullptr);
				vkFreeMemory(*device, position[i].memory, nullptr);
				vkFreeMemory(*device, material[i].memory, nullptr);
				vkFreeMemory(*device, albedo[i].memory, nullptr);

				vkDestroyImageView(*device, normal[i].view, nullptr);
				vkDestroyImageView(*device, position[i].view, nullptr);
				vkDestroyImageView(*device, material[i].view, nullptr);
				vkDestroyImageView(*device, albedo[i].view, nullptr);
				//
				vkDestroyImage(*device, normal[i].image, nullptr);
				vkDestroyImage(*device, position[i].image, nullptr);
				vkDestroyImage(*device, material[i].image, nullptr);
				vkDestroyImage(*device, albedo[i].image, nullptr);
				
			}
			for (auto& fb : framebuffers)vkDestroyFramebuffer(*device, fb, nullptr);

			vkDestroyRenderPass(*device,renderpass, nullptr);
			normal.clear();
			position.clear();
			material.clear();
			albedo.clear();
			ao.clear();
		}
	}deferedResources;;


	Device device;
	Debuger debuger;
	PresentationEngine presentationEngine;
	VkInstance instance;
	VkRenderPass mainRenderPass;
	std::vector<VkFramebuffer> mainFrameBuffers;
	ImageStr depthResource;
	struct { std::vector<VkSemaphore>renderFinished; std::vector<VkSemaphore>imageAvaible; std::vector<VkFence>fencesOn; std::vector<VkFence>imagesOn; }syncObjcs;
	VkCommandPool cmdPool;
	VkCommandPool transferPool;
	VkCommandPool transientCmdPool;
	std::vector<VkCommandBuffer>cmdBuffers;
	VkDescriptorPool descriptorPool;
	VkDescriptorPool dynamicDescriptorPool;
	std::vector<VkDescriptorSet>descriptorSets;
	std::vector<Buffer> lightUniforms;
	std::vector<Buffer> sceneSettingsUniform;
	std::vector<Buffer> shadowMapUniform;

	VkDescriptorSetLayout lightDescriptorSetLayout;
	std::vector<VkDescriptorSet> lightDescriptorSets;
	std::vector<VkDescriptorSet>toneMapDescriptorSets;
	std::vector<VkDescriptorSet>verticalBlurDescriptorSet;
	std::vector<VkDescriptorSet>horizontalBlurDescriptorSet;
	std::vector<VkDescriptorSet>shadowMapDescriptorSet;
	std::vector<VkDescriptorSet> depthNirradianceMapDescriptorSet;
	std::vector<VkDescriptorSet> irradianceDescriptorSet;


	GLFWwindow* window;
	VkSurfaceKHR surface;

	

	struct {
		Texture cubeMap;
		Texture irradianceMap;
		std::vector<Buffer> uniformBuffers;
		std::vector<Buffer> skyUniform;

		UniformSkybox uniformUbo;
		std::vector<VkDescriptorSet>descriptorsets;
		Buffer vertexBuffer;
		VkSampler sampler;

		void destroyBuffers(const VkDevice device) {

			for (auto& buffer : uniformBuffers)buffer.destroy(device);
			for (auto& buffer : skyUniform)buffer.destroy(device);
		}

		void destroytextures(const VkDevice device) {

			vkFreeMemory(device, cubeMap.imageMemory, nullptr);
			vkDestroyImageView(device, cubeMap.view, nullptr);
			vkDestroyImage(device, cubeMap.image, nullptr);

			vkFreeMemory(device, irradianceMap.imageMemory, nullptr);
			vkDestroyImageView(device, irradianceMap.view, nullptr);
			vkDestroyImage(device, irradianceMap.image, nullptr);

			vkDestroySampler(device, sampler, nullptr);

		}

		void destroy(const VkDevice device) {
			
			vkFreeMemory(device, cubeMap.imageMemory, nullptr);
			vkDestroyImageView(device, cubeMap.view, nullptr);
			vkDestroyImage(device, cubeMap.image, nullptr);

			vkFreeMemory(device, irradianceMap.imageMemory, nullptr);
			vkDestroyImageView(device, irradianceMap.view, nullptr);
			vkDestroyImage(device, irradianceMap.image, nullptr);

			vkDestroySampler(device, sampler, nullptr);

			for (auto& buffer : uniformBuffers)buffer.destroy(device);
			for (auto& buffer : skyUniform)buffer.destroy(device);

			vertexBuffer.destroy(device);


		}

	}skybox;

	struct SSAO {
		struct Kernel{
			std::array<glm::vec3,64> kernelSamples;
		}uniforms;
		
		struct Parameters {
			glm::mat4 projection;
			glm::mat4 view;
			float radius = .5f;
		}parameters;;

		VkRenderPass renderpass;
		VkRenderPass gausianSSAORenderPass;

		std::vector<VkFramebuffer> frameBuffers;
		std::vector<VkFramebuffer> gausianFbs;

		const int numSamples = 64;
		std::default_random_engine generator;
		Texture noiseTexture;
		std::array<glm::vec3,16> noise;
		
		std::vector<RenderAttachment> gausianAttachments;
		std::vector< RenderAttachment>ssao;

		std::vector<Buffer> buffers;
		std::vector<Buffer>parameterBuffer;

		std::vector<VkDescriptorSet> descriptorsets;
		std::vector<VkDescriptorSet> gausianDescriptorsets;
		
		

		void initializeSamples() {
			std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);

			for (int i = 0; i < numSamples; i++) {
				glm::vec3 sample(
					randomFloats(generator) * 2.0 - 1.0,
					randomFloats(generator) * 2.0 - 1.0,
					randomFloats(generator)
				);
				sample = glm::normalize(sample);
				sample *= randomFloats(generator);
				float scale = i / numSamples;
				scale = lerp(.1f, 1.0f, scale * scale);
				sample *= scale;

				uniforms.kernelSamples[i] = sample;
			}

			for (int i = 0; i < 16; i++) {
				glm::vec3 noisePoints(
					randomFloats(generator) * 2.0 - 1.0,
					randomFloats(generator) * 2.0 - 1.0,
					0.0f);
					noise[i] = noisePoints;

			}

		}

		float lerp(float a, float b, float f)
		{
			return a + f * (b - a);
		}

		void destroy(const VkDevice device) {


			vkDestroyRenderPass(device, renderpass, nullptr);

			vkDestroyRenderPass(device, gausianSSAORenderPass, nullptr);

			for (auto& p : frameBuffers)vkDestroyFramebuffer(device, p, nullptr);
			for (auto& p : buffers)p.destroy(device);
			for (auto& p : parameterBuffer)p.destroy(device);
			for (auto& p : ssao)p.destroy(device);
			for (auto& p : gausianAttachments)p.destroy(device);

			for (auto& p : gausianFbs)vkDestroyFramebuffer(device, p, nullptr);

			vkDestroyImage(device, noiseTexture.image, nullptr);
			vkDestroyImageView(device, noiseTexture.view, nullptr);
			vkFreeMemory(device, noiseTexture.imageMemory, nullptr);

			gausianAttachments.clear();
			ssao.clear();
			buffers.clear();
			gausianFbs.clear();
			frameBuffers.clear();
			descriptorsets.clear();
			gausianDescriptorsets.clear();

		}

		void partialyDestroy(const VkDevice device) {

			vkDestroyRenderPass(device, renderpass, nullptr);

			vkDestroyRenderPass(device, gausianSSAORenderPass, nullptr);

			for (auto& p : frameBuffers)vkDestroyFramebuffer(device, p, nullptr);
			for (auto& p : buffers)p.destroy(device);
			for (auto& p : parameterBuffer)p.destroy(device);
			for (auto& p : ssao)p.destroy(device);
			for (auto& p : gausianAttachments)p.destroy(device);

			for (auto& p : gausianFbs)vkDestroyFramebuffer(device, p, nullptr);

			descriptorsets.clear();
			gausianDescriptorsets.clear();
			gausianAttachments.clear();
			ssao.clear();
			buffers.clear();
			gausianFbs.clear();
			frameBuffers.clear();
			parameterBuffer.clear();

		}

	}aoPass;

	struct {
		VkDescriptorSetLayout layout;
		VkSampler sampler;
		VkSampler samplerLut;
		std::vector<VkDescriptorSet> descriptorset;
		Texture filteredEnvMap;
		Texture lutBRDF;

		void destroyFilteredEnv(const VkDevice device) {

			vkDestroyImageView(device, filteredEnvMap.view, nullptr);
			vkDestroyImage(device, filteredEnvMap.image, nullptr);
			vkFreeMemory(device, filteredEnvMap.imageMemory, nullptr);
			vkDestroySampler(device, sampler, nullptr);

		}

		void destroy(const VkDevice device) {

			vkDestroyImageView(device, filteredEnvMap.view, nullptr);
			vkDestroyImage(device, filteredEnvMap.image, nullptr);
			vkFreeMemory(device, filteredEnvMap.imageMemory, nullptr);

			vkDestroyImageView(device, lutBRDF.view, nullptr);
			vkDestroyImage(device, lutBRDF.image, nullptr);
			vkFreeMemory(device, lutBRDF.imageMemory, nullptr);

			vkDestroySampler(device, samplerLut, nullptr);
			vkDestroySampler(device, sampler, nullptr);

		}

	}specularBrdf;

	std::string hdrImage =  "Aplication\\assets\\skybox\\Scherbelin.hdr";


	struct Diagnosis {
		int outPutFBO = 0;

	}diagnosis;
	struct { 
		std::vector<Buffer> parametersBuffer;
		std::vector <VkDescriptorSet> descriptorSets;
		Texture normal;
		Texture dudv;
		Texture reflection;
		struct waterParameters {
			float strengh = .003;
			alignas(16) glm::vec3 texCoordsScale = glm::vec3(1);
	
			float time;
			float reflectivity = .8	 ;
		}parameters;

		void update(const VkDevice device,int currentImage){
			parameters.time = fmod((.03f * (float)glfwGetTime()), 1.0);
			void* data1;
			vkMapMemory(device, parametersBuffer[currentImage].bufferMemory, 0, sizeof(waterParameters), 0, &data1);
			memcpy(data1, &parameters, sizeof(waterParameters));
			vkUnmapMemory(device, parametersBuffer[currentImage].bufferMemory);
		
		}

		void destroyUniformBuffers(const VkDevice device) {
			for (auto& buffer : parametersBuffer)buffer.destroy(device);
			parametersBuffer.clear();
		
		}

		void destroy(const VkDevice device) {

			for (int i = 0; i < parametersBuffer.size(); i++) {
				vkDestroyBuffer(device, parametersBuffer[i].buffer, nullptr);
				vkFreeMemory(device, parametersBuffer[i].bufferMemory, nullptr);
			}
	
			normal.destroy(device);
			dudv.destroy(device);
		}

	}waterResources;

	Buffer seaVertexBuffer;
	VkPipelineCache pipelineCache;
	std::vector<Buffer> toneMappingBuffer;
	std::vector<Buffer> rndSamples;
	std::vector<Buffer>visualizationBuffer;
	bool VIEW_MODE = 1;
	bool cleaned = false;
	const char* appName;

	int measure = 0;

	void createAttachments(VkFormat format, VkImageUsageFlags usage, RenderAttachment* attachment, VkExtent2D extent);
	void allocatePostProcessingBuffers();
	void createSkybox();
	void createIrradianceMap();
	void createpreFilteredEnvMap();
	void createBrdfLut();
	void createSkyEnviroment();
	void createPostProcessingRenderPasses();
	void createShadowMapRenderPass();
	void createmGuiInterface();
	void prepareImGuiResources();
	void renderUI(uint32_t imageIndex);
	void updateSky();
	void updateLights(uint32_t imageIndex);
	void createGBufferRenderpass();
	void desingUi();
	void rebuild();



	void createWindow();
	void createSurface();
	void createInstance();
	void createDeferredShadingRenderpass();
	void creategraphicsPipelines();
	void createDepthAttachment();
	void createAttachments();
	void createAmbientOclusionResources();
	void createMainFrameBuffers();
	void createCommandPool();
	void allocateCommandBuffers();
	void createDrawCommand();
	void createSyncronizationObjects();
	void createLightUniformBuffers();
	void initiateLightContainer();
	void updateMaterialUniforms(int currentImage);
	void update(unsigned int CurrentImage,float frameTIme);
	void createTransientCommandPool();
	void createDescriptorPool();
	void loadSceneData();
	void renderCycle();
	void renderLoop();
	void renderCleanUp();

	std::unique_ptr<TUGEV::RenderManager> renderManager;
	Scenegraph sceneGraph;
};