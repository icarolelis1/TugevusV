#pragma once
#include <iostream>
#include "RenderTools.h"
#include <vector>
#include <assert.h>
#include <optional>
#include "RenderTools.h"
#include <set>
#include "TugevusStructs.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> transfer;
	std::optional<uint32_t> graphics;
	std::optional<uint32_t> compute;
	std::optional<uint32_t> present;
	bool isComplete() {
		checkGraphicsQueue();
		checkTransferQueue();
		checkComputeQueue();

		return graphics.has_value();
	}
	bool checkGraphicsQueue() {
		if (!graphics.has_value()) {
			std::cout << "\nThere is no GraphicsQueue Suitable\n";
			return false;
		}
		std::cout << "\tThe Graphics Queue is at Index : " << graphics.value() << std::endl;
		return true;
	}
	bool checkTransferQueue() {
		if (!transfer.has_value()) {
			std::cout << "\nThere is no TransferQueue Suitable\n";
			return false;
		}
		std::cout << "\tThe Transfer Queue is at Index : " << transfer.value() << std::endl;

		return true;
	}
	 bool checkComputeQueue() {
		if (!compute.has_value()) {
			std::cout << "\nThere is no Compute Suitable\n";
			return false;
		}
		std::cout << "\tThe Compute Compute is at Index : " << compute.value() << std::endl;

		return true;
	}
	 bool checkPresentQueue() {
		 if (!transfer.has_value()) {
			 std::cout << "\nThere is no Presentation Suitable\n";
			 return false;
		 }
		 std::cout << "\tThe Presentation Queue is at Index : " << present.value() << std::endl;

		 return true;
	 }
};

struct VulkanGpu {
	VkPhysicalDevice physicalDevice;
	VkDevice		device;
	QueueFamilyIndices queueFamilies;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceMemoryProperties memoryProperties;
	VkPhysicalDeviceFeatures features;
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class Device
{
public:
	Device(VkInstance *instance,VkSurfaceKHR *surface,GLFWwindow *window);
	void prepareDevice();

	const VkPhysicalDevice *getPhysicalDevice();
	const VkDevice *getVulkanDevice();
	uint32_t getGraphicsQueueIndex();
	uint32_t getTransferQueueIndex();
	const VkQueue &getGraphicsQueueHandle();
	const VkQueue &getPresentationQueueHandle();
	const VkDevice *getDevice();
	const VkQueue *getTransferQueueHandle();

	QueueSharingMode getSharingMode();
	VkFormat getSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	void cleanUp();

	~Device();

private:
	VkPhysicalDeviceFeatures supportedFeatures;

	VkInstance *instance;
	VulkanGpu gpu;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkQueue transferQueue;
	VkSurfaceKHR *surface;
	GLFWwindow *window;


	bool checkDeviceSuitability(VkPhysicalDevice device);
	void choosePhysicalDevice();
	bool checkDeviceExtensions(VkPhysicalDevice device);
	bool querySwapChainProperties(VkPhysicalDevice device);
	bool getQueuFamilityIndex(VkQueueFamilyProperties props,VkQueueFlagBits queueFlags);
	void createLogicalDevice();

};

