#include "PresentationEngine.h"


PresentationEngine::PresentationEngine()
{
}

VkFormat PresentationEngine::getSwapChainFormat()
{
	return swapChainExt.format.format;
}

const std::vector<VkImageView>& PresentationEngine::getImageViews()
{
	return swapChainImages.views;
}

uint32_t PresentationEngine::getImageCount()
{
	return swapChainImages.views.size();
}

VkExtent2D PresentationEngine::getExtent()
{
	VkExtent2D extent = { WIDTH,HEIGHT };


	return extent;
}

VkSwapchainKHR & PresentationEngine::getSwapChain()
{
	return swapChainExt.spChain;
}

void PresentationEngine::setWindowUserPointer(GLFWwindow *window)
{

}

void PresentationEngine::cleanUp()
{

	for (auto view : swapChainImages.views) {
		vkDestroyImageView(*device, view, nullptr);
	}

	
	vkDestroySwapchainKHR(*device, swapChainExt.spChain, nullptr);
}

PresentationEngine::~PresentationEngine()
{
}

void PresentationEngine::preparePresentationEngine(const VkDevice *dev,const VkPhysicalDevice *physicalDevice,QueueSharingMode sharing, VkSurfaceKHR surf)
{
	this->surface = surf;
	this->physicalDevice = physicalDevice;
	this->device = dev;

	getSwapChainProperties();
	createSwapChain(sharing);
	getSwapChainImages();
	createSwapChainImageViews();
}

void PresentationEngine::getSwapChainProperties()
{
	//The suitability of the physical device to support a swapChain was already tested in DeviceCreation
	SwapChainQuery swapChainQuery;

	uint32_t formatCount;
	uint32_t presentModeCount;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*physicalDevice, surface, &swapChainExt.capabilities);

	vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &formatCount, nullptr);
	swapChainQuery.formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, surface, &formatCount, swapChainQuery.formats.data());

	vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, surface, &presentModeCount, nullptr);
	assert(presentModeCount > 0);
	swapChainQuery.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, surface, &presentModeCount, swapChainQuery.presentModes.data());


	swapChainExt.format = chooseSurfaceFormat(swapChainQuery.formats);
	swapChainExt.extent = chooseImageExtent(swapChainExt.capabilities);
	swapChainExt.presentMode = choosePresentationMode(swapChainQuery.presentModes);

	swapChainExt.minImageCount = swapChainExt.capabilities.minImageCount + 1;

	if (swapChainExt.capabilities.maxImageCount > 0 && swapChainExt.minImageCount > swapChainExt.capabilities.maxImageCount) {
		swapChainExt.minImageCount = swapChainExt.capabilities.maxImageCount;
	}

}

VkSurfaceFormatKHR PresentationEngine::chooseSurfaceFormat(std::vector<VkSurfaceFormatKHR>&formats)
{
	for (const auto &format : formats) {
		if (format.format == VK_FORMAT_R32G32B32A32_SFLOAT && format.colorSpace ==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return format;
	}
	return formats[0];
}

VkPresentModeKHR PresentationEngine::choosePresentationMode(std::vector< VkPresentModeKHR> &presentModes) {
	//for (const auto &presentMode : presentModes) {
	//	if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
	//		return presentMode;
	//}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D PresentationEngine::chooseImageExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		int w, h;
		VkExtent2D actualExtent;
		//	= { WIDTH, HEIGHT };
		glfwGetFramebufferSize(window, &w, &h);
		actualExtent.width = static_cast<uint32_t>(w);
		actualExtent.height = static_cast<uint32_t>(h);

		return actualExtent;
	}
}

void PresentationEngine::createSwapChain(QueueSharingMode queueSharing)
{
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.clipped = VK_TRUE;
	createInfo.imageFormat = swapChainExt.format.format;
	createInfo.minImageCount = swapChainExt.minImageCount;
	createInfo.imageArrayLayers = 1;
	createInfo.imageColorSpace = swapChainExt.format.colorSpace;
	createInfo.imageSharingMode = swapChainExt.sharingMode;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = swapChainExt.presentMode;
	createInfo.imageExtent = swapChainExt.extent;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = swapChainExt.capabilities.currentTransform;
	if (queueSharing.queueFamiliyIndexCount > 1) {
		createInfo.imageSharingMode = queueSharing.sharingMode;
		createInfo.queueFamilyIndexCount = queueSharing.queueFamiliyIndexCount;
		createInfo.pQueueFamilyIndices = queueSharing.queueFamilies;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	createInfo.flags = 0;

	VkResult result =  vkCreateSwapchainKHR(*device, &createInfo, nullptr, &swapChainExt.spChain);
	vkTools::checkVkresult(result, "\nFTC swapChain \n");
	std::cout << "Sucessfully created a SwapChain\n";

}

void PresentationEngine::getSwapChainImages()
{
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(*device, swapChainExt.spChain, &imageCount, nullptr);
	assert(imageCount >= 1);
	swapChainImages.images.resize(imageCount);
	vkGetSwapchainImagesKHR(*device, swapChainExt.spChain, &imageCount, swapChainImages.images.data());

}

void PresentationEngine::createSwapChainImageViews()
{
	swapChainImages.views.resize((swapChainImages.images.size()));

	for (size_t i = 0; i < swapChainImages.images.size(); i++) {

		 VkResult result = graphicsToolKit::createImageView(*device,swapChainImages.views[i], swapChainImages.images[i], swapChainExt.format.format,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, 1);
		 vkTools::checkVkresult(result, "FTC SwapChainViews\n");
	}

}
