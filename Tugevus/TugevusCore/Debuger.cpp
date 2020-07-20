#include "Debuger.h"




Debuger::Debuger(VkInstance *inst):instance(inst)
{

}

bool Debuger::checkLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers;
	availableLayers.resize(layerCount);

	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayer) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;

				break;
			}
		}

		if (!layerFound) {

			return false;
		}
	}

	return true;
}

const std::vector<const char*> Debuger::getValidationLayers()
{
	return validationLayer;
}

const std::vector<const char*> Debuger::getExtensions(bool enableValidationLayers)
{
	uint32_t glfwExtensionCount =0 ;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}


Debuger::~Debuger()
{
}

void Debuger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT & createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


void Debuger::setupDebugMessenger(bool enableValidation)
{
	assert(instance != nullptr);
	if (!enableValidation)return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	populateDebugMessengerCreateInfo(createInfo);

	VkResult  result = vkTools::CreateDebugUtilsMessengerEXT(*instance, &createInfo, nullptr, &debugMessenger);
	
	vkTools::checkVkresult(result, "\t Failed to create DebugMesseger");
}

void Debuger::cleanDebugger()
{
	vkTools::DestroyDebugUtilsMessengerEXT(*instance, debugMessenger, nullptr);

}
	