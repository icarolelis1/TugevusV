#include "Device.h"




Device::Device(VkInstance * inst,VkSurfaceKHR *surf, GLFWwindow *w):instance(inst),surface(surf),window(w)
{
	assert(instance != nullptr);
	
}

void Device::prepareDevice()
{
	choosePhysicalDevice();
	createLogicalDevice();
}

const VkPhysicalDevice * Device::getPhysicalDevice()
{
	return &gpu.physicalDevice;
}

const VkDevice * Device::getVulkanDevice()
{
	return &gpu.device;
}
const VkDevice* Device::getDevice()
{
	return &gpu.device;
}

uint32_t Device::getGraphicsQueueIndex()
{
	return gpu.queueFamilies.graphics.value();
}

uint32_t Device::getTransferQueueIndex()
{
	return gpu.queueFamilies.transfer.value();
}

const VkQueue & Device::getGraphicsQueueHandle()
{
	return graphicsQueue;
}

const VkQueue & Device::getPresentationQueueHandle()
{
	return presentQueue;
}

const VkQueue  *Device::getTransferQueueHandle()
{
	return &transferQueue;
}

Device::~Device()
{
}

VkFormat Device::getSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{

	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(gpu.physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
	
}

bool Device::checkDeviceSuitability(VkPhysicalDevice device)
{
	auto properties = VkPhysicalDeviceProperties{};
	//auto memProperties = VkPhysicalDeviceMemoryProperties{};
	//auto features = VkPhysicalDeviceFeatures{};
	vkGetPhysicalDeviceProperties(device, &properties);
	//vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
	//vkGetPhysicalDeviceFeatures(device, &features);

	uint32_t queueFamilityCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilityCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilityCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilityCount, queueFamilyProperties.data());

	VkBool32 foundGraphics = false;
	VkBool32 foundCompute  = false;
	VkBool32 foundTransfer = false;
	VkBool32 foundPresent = false;

	if (properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		std::cout << "\nEvaluating Device : " << properties.deviceName << std::endl;
		std::cout << "\tNumber of QueueFamily Properties : " << queueFamilyProperties.size() << std::endl;

		for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {

			if (!foundGraphics && getQueuFamilityIndex(queueFamilyProperties[i], VK_QUEUE_GRAPHICS_BIT)) {
				foundGraphics = true;
				gpu.queueFamilies.graphics = i; 
			}
			if (!foundCompute && getQueuFamilityIndex(queueFamilyProperties[i], VK_QUEUE_COMPUTE_BIT)) {
				foundCompute = true;
				gpu.queueFamilies.compute = i;
			}
			if (!foundTransfer && getQueuFamilityIndex(queueFamilyProperties[i], VK_QUEUE_TRANSFER_BIT)) {
				foundTransfer = true;
				gpu.queueFamilies.transfer = i;

			}
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &foundPresent);

			if (foundPresent && !gpu.queueFamilies.present.has_value()) {
				gpu.queueFamilies.present = i;
			}

		}

	}
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	bool features = supportedFeatures.samplerAnisotropy && supportedFeatures.shaderClipDistance;

	return (foundGraphics && foundCompute &&foundTransfer && checkDeviceExtensions(device) && querySwapChainProperties(device) && features);


}

void Device::choosePhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		std::cout << "\nFailed to find a proper Graphics Card\n";
	}

	std::vector<VkPhysicalDevice>devices(deviceCount);
	vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

	VkBool32 foundSuitableDevice = false;

	for (const auto& device : devices) {
		if (checkDeviceSuitability(device)) {
			gpu.physicalDevice = device;
			foundSuitableDevice = true;
			break;
		}
	}

	if (!foundSuitableDevice)std::cout << "\n\tFailed to find a suitableDevice\n";

	vkGetPhysicalDeviceProperties(gpu.physicalDevice, &gpu.properties);
	vkGetPhysicalDeviceMemoryProperties(gpu.physicalDevice, &gpu.memoryProperties);
	vkGetPhysicalDeviceFeatures(gpu.physicalDevice, &gpu.features);

	std::cout << "\nThe device picked is : " << std::endl;
	std::cout << "\t" << gpu.properties.deviceName << std::endl;

	gpu.queueFamilies.isComplete();
}

bool Device::checkDeviceExtensions(VkPhysicalDevice device)
{
	uint32_t extensionsCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);
	
	std::vector<VkExtensionProperties> extensions(extensionsCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, extensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());


	for (const auto& extension : extensions) {
		requiredExtensions.erase(extension.extensionName);
	}
	
	return requiredExtensions.empty();
}

bool Device::querySwapChainProperties(VkPhysicalDevice device)
{
	SwapChainQuery swapChainExt;
	uint32_t formatCount;
	uint32_t presentModeCount;

	//vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *surface, &swapChainExt.capabilities);

	vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surface,  &formatCount, nullptr);
	assert(formatCount > 0);
	swapChainExt.formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surface, &formatCount, swapChainExt.formats.data());

	vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surface , &presentModeCount, nullptr);
	assert(presentModeCount > 0);
	swapChainExt.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surface, &presentModeCount, swapChainExt.presentModes.data());

	bool isSuitableSwapChain = false;
	isSuitableSwapChain = !swapChainExt.formats.empty() && !swapChainExt.presentModes.empty();

	return isSuitableSwapChain;
}

bool Device::getQueuFamilityIndex(VkQueueFamilyProperties props, VkQueueFlagBits queueFlags)
{

	if (queueFlags & VK_QUEUE_GRAPHICS_BIT) {
		if (props.queueFlags & queueFlags) { 
			return true; 
		
		}
	}

	else if (queueFlags & VK_QUEUE_COMPUTE_BIT) {

		if (props.queueFlags & queueFlags)	return true;

	}

	else if (queueFlags & VK_QUEUE_TRANSFER_BIT) {

		if (props.queueFlags & VK_QUEUE_TRANSFER_BIT && !(props.queueFlags & VK_QUEUE_GRAPHICS_BIT))	return true;
		
	}

	return false;
}

void Device::createLogicalDevice()
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t>uniqueFamilies = { gpu.queueFamilies.graphics.value(),gpu.queueFamilies.present.value() ,gpu.queueFamilies.transfer.value()};


	for (uint32_t queueFamily : uniqueFamilies) {
		VkDeviceQueueCreateInfo QcreateInfo{};
		QcreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QcreateInfo.queueCount = 1;
		QcreateInfo.queueFamilyIndex = queueFamily;
		float priority = 1.0f;
		QcreateInfo.pQueuePriorities = &priority;
		queueCreateInfos.push_back(QcreateInfo);
	}



	VkPhysicalDeviceFeatures features{};
	features.samplerAnisotropy = VK_TRUE;
	features.shaderClipDistance = VK_TRUE;
	features.geometryShader = VK_TRUE;

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceInfo.pEnabledFeatures = &features;
	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer.size());
	deviceInfo.ppEnabledLayerNames = validationLayer.data();
	VkResult result= vkCreateDevice(gpu.physicalDevice, &deviceInfo, nullptr, &gpu.device);

	vkTools::checkVkresult(result, "\nFTC device!\n");

	if (result == VK_SUCCESS) {
		std::cout << "\nSucessfully created a logical device \n";
		gpu.queueFamilies.isComplete();
	}
	vkGetDeviceQueue(gpu.device, gpu.queueFamilies.graphics.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(gpu.device, gpu.queueFamilies.present.value (), 0, &presentQueue);
	vkGetDeviceQueue(gpu.device, gpu.queueFamilies.transfer.value(), 0, &transferQueue);

}

QueueSharingMode Device::getSharingMode()
{
	QueueSharingMode queueSharing;
	if (gpu.queueFamilies.graphics.value() != gpu.queueFamilies.present.value()) {
		queueSharing.sharingMode = VK_SHARING_MODE_CONCURRENT;
		queueSharing.queueFamiliyIndexCount = 2;
		std::vector<uint32_t> queueF(2);
		queueF = { gpu.queueFamilies.graphics.value(),gpu.queueFamilies.present.value() };
		queueSharing.queueFamilies = queueF.data();
		return queueSharing;
	}

	queueSharing.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	queueSharing.queueFamiliyIndexCount = 1;
	return queueSharing;
	


}

void Device::cleanUp()
{
	assert(gpu.device != NULL);
	vkDestroyDevice(gpu.device, nullptr);
}
