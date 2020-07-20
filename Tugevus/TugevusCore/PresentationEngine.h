#pragma once
#include <iostream>

#include <assert.h>
#include <algorithm>
#include "RenderTools.h"
#include "TugevusStructs.h"


class PresentationEngine
{
public:
	PresentationEngine();

	VkFormat getSwapChainFormat();
	const std::vector<VkImageView> &getImageViews();
	uint32_t getImageCount();
	VkExtent2D getExtent();
	VkSwapchainKHR &getSwapChain();
	void setWindowUserPointer(GLFWwindow *window);

	bool windowResize;

	void cleanUp();
	~PresentationEngine();


	void preparePresentationEngine(const  VkDevice *deveice, const VkPhysicalDevice *device, QueueSharingMode sharing, VkSurfaceKHR surf);

private:
	const VkDevice *device;
	const VkPhysicalDevice *physicalDevice;
	VkSurfaceKHR surface;
	SwapChainExt swapChainExt;
	ImagesExt swapChainImages;
	GLFWwindow *window;


	void getSwapChainProperties();
	VkSurfaceFormatKHR chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR>&formats);
	VkPresentModeKHR  choosePresentationMode(std::vector< VkPresentModeKHR> &presentModes);
	VkExtent2D chooseImageExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	void createSwapChain(QueueSharingMode queueSharing);
	void getSwapChainImages();
	void createSwapChainImageViews();
};

