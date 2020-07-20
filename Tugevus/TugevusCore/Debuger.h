#pragma once
#include <iostream>
#include <vector>
#include "RenderTools.h"
#include <memory.h>
#include <cstring>
#include <assert.h>
#ifndef  DEBUGER
#define DEBUGER





class Debuger
{
public:
	Debuger(VkInstance *inst);

	bool checkLayerSupport();
	const std::vector<const char*> getValidationLayers();
	const std::vector<const char*> getExtensions(bool enableValidationLayer);
	void setupDebugMessenger(bool enableValidation);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void cleanDebugger();
	~Debuger();

private:

	

	bool debugEnable = true;
	VkInstance* instance;
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	VkDebugUtilsMessengerEXT debugMessenger;

};

#endif