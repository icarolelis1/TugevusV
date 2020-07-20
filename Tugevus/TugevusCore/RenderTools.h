#pragma once
#include "TugevusStructs.h"
#include <array>
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

//Sascha Willems Initializer 
static const std::vector<const char*> validationLayer = {
		"VK_LAYER_LUNARG_standard_validation"
};

static  int WIDTH = 1024;
static  int HEIGHT = 1024;

namespace initializers {

	inline VkMemoryAllocateInfo memoryAllocateInfo()
	{
		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		return memAllocInfo;
	}

	inline VkMappedMemoryRange mappedMemoryRange()
	{
		VkMappedMemoryRange mappedMemoryRange{};
		mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		return mappedMemoryRange;
	}

	inline VkCommandBufferAllocateInfo commandBufferAllocateInfo(
		VkCommandPool commandPool,
		VkCommandBufferLevel level,
		uint32_t bufferCount)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.level = level;
		commandBufferAllocateInfo.commandBufferCount = bufferCount;
		return commandBufferAllocateInfo;
	}

	inline VkCommandPoolCreateInfo commandPoolCreateInfo()
	{
		VkCommandPoolCreateInfo cmdPoolCreateInfo{};
		cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		return cmdPoolCreateInfo;
	}

	inline VkCommandBufferBeginInfo commandBufferBeginInfo()
	{
		VkCommandBufferBeginInfo cmdBufferBeginInfo{};
		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		return cmdBufferBeginInfo;
	}

	inline VkCommandBufferInheritanceInfo commandBufferInheritanceInfo()
	{
		VkCommandBufferInheritanceInfo cmdBufferInheritanceInfo{};
		cmdBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		return cmdBufferInheritanceInfo;
	}

	inline VkRenderPassBeginInfo renderPassBeginInfo()
	{
		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		return renderPassBeginInfo;
	}

	inline VkRenderPassCreateInfo renderPassCreateInfo()
	{
		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		return renderPassCreateInfo;
	}

	inline VkImageMemoryBarrier imageMemoryBarrier()
	{
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		return imageMemoryBarrier;
	}

	inline VkBufferMemoryBarrier bufferMemoryBarrier()
	{
		VkBufferMemoryBarrier bufferMemoryBarrier{};
		bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		return bufferMemoryBarrier;
	}

	inline VkMemoryBarrier memoryBarrier()
	{
		VkMemoryBarrier memoryBarrier{};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		return memoryBarrier;
	}

	inline VkImageCreateInfo imageCreateInfo()
	{
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		return imageCreateInfo;
	}

	inline VkSamplerCreateInfo samplerCreateInfo()
	{
		VkSamplerCreateInfo samplerCreateInfo{};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;
		return samplerCreateInfo;
	}

	inline VkImageViewCreateInfo imageViewCreateInfo()
	{
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		return imageViewCreateInfo;
	}

	inline VkFramebufferCreateInfo framebufferCreateInfo()
	{
		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		return framebufferCreateInfo;
	}

	inline VkSemaphoreCreateInfo semaphoreCreateInfo()
	{
		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		return semaphoreCreateInfo;
	}

	inline VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags = 0)
	{
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = flags;
		return fenceCreateInfo;
	}

	inline VkEventCreateInfo eventCreateInfo()
	{
		VkEventCreateInfo eventCreateInfo{};
		eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		return eventCreateInfo;
	}

	inline VkSubmitInfo submitInfo()
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		return submitInfo;
	}

	inline VkViewport viewport(
		float width,
		float height,
		float minDepth,
		float maxDepth)
	{
		VkViewport viewport{};
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = minDepth;
		viewport.maxDepth = maxDepth;
		return viewport;
	}

	inline VkRect2D rect2D(
		int32_t width,
		int32_t height,
		int32_t offsetX,
		int32_t offsetY)
	{
		VkRect2D rect2D{};
		rect2D.extent.width = width;
		rect2D.extent.height = height;
		rect2D.offset.x = offsetX;
		rect2D.offset.y = offsetY;
		return rect2D;
	}

	inline VkBufferCreateInfo bufferCreateInfo()
	{
		VkBufferCreateInfo bufCreateInfo{};
		bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		return bufCreateInfo;
	}

	inline VkBufferCreateInfo bufferCreateInfo(
		VkBufferUsageFlags usage,
		VkDeviceSize size)
	{
		VkBufferCreateInfo bufCreateInfo{};
		bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufCreateInfo.usage = usage;
		bufCreateInfo.size = size;
		return bufCreateInfo;
	}

	inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
		uint32_t poolSizeCount,
		VkDescriptorPoolSize* pPoolSizes,
		uint32_t maxSets)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = poolSizeCount;
		descriptorPoolInfo.pPoolSizes = pPoolSizes;
		descriptorPoolInfo.maxSets = maxSets;
		return descriptorPoolInfo;
	}

	inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
		const std::vector<VkDescriptorPoolSize>& poolSizes,
		uint32_t maxSets)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		descriptorPoolInfo.pPoolSizes = poolSizes.data();
		descriptorPoolInfo.maxSets = maxSets;
		return descriptorPoolInfo;
	}

	inline VkDescriptorPoolSize descriptorPoolSize(
		VkDescriptorType type,
		uint32_t descriptorCount)
	{
		VkDescriptorPoolSize descriptorPoolSize{};
		descriptorPoolSize.type = type;
		descriptorPoolSize.descriptorCount = descriptorCount;
		return descriptorPoolSize;
	}

	inline VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(
		VkDescriptorType type,
		VkShaderStageFlags stageFlags,
		uint32_t binding,
		uint32_t descriptorCount = 1)
	{
		VkDescriptorSetLayoutBinding setLayoutBinding{};
		setLayoutBinding.descriptorType = type;
		setLayoutBinding.stageFlags = stageFlags;
		setLayoutBinding.binding = binding;
		setLayoutBinding.descriptorCount = descriptorCount;
		return setLayoutBinding;
	}

	inline VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const VkDescriptorSetLayoutBinding* pBindings,
		uint32_t bindingCount)
	{
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.pBindings = pBindings;
		descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
		return descriptorSetLayoutCreateInfo;
	}

	inline VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
		descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutCreateInfo.pBindings = bindings.data();
		descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		return descriptorSetLayoutCreateInfo;
	}


	inline VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		const VkDescriptorSetLayout* pSetLayouts,
		uint32_t setLayoutCount = 1)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
		pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
		return pipelineLayoutCreateInfo;
	}

	inline VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(
		uint32_t setLayoutCount = 1)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
		return pipelineLayoutCreateInfo;
	}

	inline VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(
		VkDescriptorPool descriptorPool,
		const VkDescriptorSetLayout* pSetLayouts,
		uint32_t descriptorSetCount)
	{
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.descriptorPool = descriptorPool;
		descriptorSetAllocateInfo.pSetLayouts = pSetLayouts;
		descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
		return descriptorSetAllocateInfo;
	}

	inline VkDescriptorImageInfo descriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout)
	{
		VkDescriptorImageInfo descriptorImageInfo{};
		descriptorImageInfo.sampler = sampler;
		descriptorImageInfo.imageView = imageView;
		descriptorImageInfo.imageLayout = imageLayout;
		return descriptorImageInfo;
	}

	inline VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorBufferInfo* bufferInfo,
		uint32_t descriptorCount = 1)
	{
		VkWriteDescriptorSet writeDescriptorSet{};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = dstSet;
		writeDescriptorSet.descriptorType = type;
		writeDescriptorSet.dstBinding = binding;
		writeDescriptorSet.pBufferInfo = bufferInfo;
		writeDescriptorSet.descriptorCount = descriptorCount;
		return writeDescriptorSet;
	}

	inline VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorImageInfo* imageInfo,
		uint32_t descriptorCount = 1)
	{
		VkWriteDescriptorSet writeDescriptorSet{};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.dstSet = dstSet;
		writeDescriptorSet.descriptorType = type;
		writeDescriptorSet.dstBinding = binding;
		writeDescriptorSet.pImageInfo = imageInfo;
		writeDescriptorSet.descriptorCount = descriptorCount;
		return writeDescriptorSet;
	}

	inline VkVertexInputBindingDescription vertexInputBindingDescription(
		uint32_t binding,
		uint32_t stride,
		VkVertexInputRate inputRate)
	{
		VkVertexInputBindingDescription vInputBindDescription{};
		vInputBindDescription.binding = binding;
		vInputBindDescription.stride = stride;
		vInputBindDescription.inputRate = inputRate;
		return vInputBindDescription;
	}

	inline VkVertexInputAttributeDescription vertexInputAttributeDescription(
		uint32_t binding,
		uint32_t location,
		VkFormat format,
		uint32_t offset)
	{
		VkVertexInputAttributeDescription vInputAttribDescription{};
		vInputAttribDescription.location = location;
		vInputAttribDescription.binding = binding;
		vInputAttribDescription.format = format;
		vInputAttribDescription.offset = offset;
		return vInputAttribDescription;
	}

	inline VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo()
	{
		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo{};
		pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		return pipelineVertexInputStateCreateInfo;
	}

	inline VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
		VkPrimitiveTopology topology,
		VkPipelineInputAssemblyStateCreateFlags flags,
		VkBool32 primitiveRestartEnable)
	{
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
		pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineInputAssemblyStateCreateInfo.topology = topology;
		pipelineInputAssemblyStateCreateInfo.flags = flags;
		pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
		return pipelineInputAssemblyStateCreateInfo;
	}

	inline VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace,
		VkPipelineRasterizationStateCreateFlags flags = 0)
	{
		VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
		pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
		pipelineRasterizationStateCreateInfo.cullMode = cullMode;
		pipelineRasterizationStateCreateInfo.frontFace = frontFace;
		pipelineRasterizationStateCreateInfo.flags = flags;
		pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
		pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
		return pipelineRasterizationStateCreateInfo;
	}

	inline VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(
		VkColorComponentFlags colorWriteMask,
		VkBool32 blendEnable)
	{
		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState{};
		pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
		pipelineColorBlendAttachmentState.blendEnable = blendEnable;
		return pipelineColorBlendAttachmentState;
	}

	inline VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
		uint32_t attachmentCount, const VkPipelineColorBlendAttachmentState* pAttachments)
	{
		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
		pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
		pipelineColorBlendStateCreateInfo.pAttachments = pAttachments;
		return pipelineColorBlendStateCreateInfo;
	}

	inline VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
		VkBool32 depthTestEnable,
		VkBool32 depthWriteEnable,
		VkCompareOp depthCompareOp)
	{
		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo{};
		pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
		pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
		pipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
		pipelineDepthStencilStateCreateInfo.front = pipelineDepthStencilStateCreateInfo.back;
		pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
		return pipelineDepthStencilStateCreateInfo;
	}

	inline VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
		uint32_t viewportCount,
		uint32_t scissorCount,
		VkPipelineViewportStateCreateFlags flags = 0)
	{
		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
		pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipelineViewportStateCreateInfo.viewportCount = viewportCount;
		pipelineViewportStateCreateInfo.scissorCount = scissorCount;
		pipelineViewportStateCreateInfo.flags = flags;
		return pipelineViewportStateCreateInfo;
	}

	inline VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(
		VkSampleCountFlagBits rasterizationSamples,
		VkPipelineMultisampleStateCreateFlags flags = 0)
	{
		VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
		pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
		pipelineMultisampleStateCreateInfo.flags = flags;
		return pipelineMultisampleStateCreateInfo;
	}

	inline VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const VkDynamicState* pDynamicStates,
		uint32_t dynamicStateCount,
		VkPipelineDynamicStateCreateFlags flags = 0)
	{
		VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
		pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates;
		pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
		pipelineDynamicStateCreateInfo.flags = flags;
		return pipelineDynamicStateCreateInfo;
	}

	inline VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const std::vector<VkDynamicState>& pDynamicStates,
		VkPipelineDynamicStateCreateFlags flags = 0)
	{
		VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
		pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates.data();
		pipelineDynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(pDynamicStates.size());
		pipelineDynamicStateCreateInfo.flags = flags;
		return pipelineDynamicStateCreateInfo;
	}

	inline VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo(uint32_t patchControlPoints)
	{
		VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo{};
		pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
		return pipelineTessellationStateCreateInfo;
	}

	inline VkGraphicsPipelineCreateInfo pipelineCreateInfo(
		VkPipelineLayout layout,
		VkRenderPass renderPass,
		VkPipelineCreateFlags flags = 0)
	{
		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.layout = layout;
		pipelineCreateInfo.renderPass = renderPass;
		pipelineCreateInfo.flags = flags;
		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		return pipelineCreateInfo;
	}

	inline VkGraphicsPipelineCreateInfo pipelineCreateInfo()
	{
		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.basePipelineIndex = -1;
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		return pipelineCreateInfo;
	}

	inline VkComputePipelineCreateInfo computePipelineCreateInfo(
		VkPipelineLayout layout,
		VkPipelineCreateFlags flags = 0)
	{
		VkComputePipelineCreateInfo computePipelineCreateInfo{};
		computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computePipelineCreateInfo.layout = layout;
		computePipelineCreateInfo.flags = flags;
		return computePipelineCreateInfo;
	}

	inline VkPushConstantRange pushConstantRange(
		VkShaderStageFlags stageFlags,
		uint32_t size,
		uint32_t offset)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = stageFlags;
		pushConstantRange.offset = offset;
		pushConstantRange.size = size;
		return pushConstantRange;
	}

	inline VkBindSparseInfo bindSparseInfo()
	{
		VkBindSparseInfo bindSparseInfo{};
		bindSparseInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
		return bindSparseInfo;
	}

	inline VkSpecializationMapEntry specializationMapEntry(uint32_t constantID, uint32_t offset, size_t size)
	{
		VkSpecializationMapEntry specializationMapEntry{};
		specializationMapEntry.constantID = constantID;
		specializationMapEntry.offset = offset;
		specializationMapEntry.size = size;
		return specializationMapEntry;
	}

	inline VkSpecializationInfo specializationInfo(uint32_t mapEntryCount, const VkSpecializationMapEntry* mapEntries, size_t dataSize, const void* data)
	{
		VkSpecializationInfo specializationInfo{};
		specializationInfo.mapEntryCount = mapEntryCount;
		specializationInfo.pMapEntries = mapEntries;
		specializationInfo.dataSize = dataSize;
		specializationInfo.pData = data;
		return specializationInfo;
	}


}

namespace vkTools {

	inline void checkVkresult(VkResult result, const char message[15]) {
		if (result != VK_SUCCESS)
			std::cout << message << std::endl;

	}
	inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
	inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
	inline void here(int i) {
		std::cout << "\n are we here? - " << i << "\n";
	}
};

namespace generalUtiliy {
	inline std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	inline void printVec3(glm::vec3 p) {
		std::cout << "X : " << p.x << std::endl;
		std::cout << "Y : " << p.y << std::endl;
		std::cout << "Z : " << p.z << std::endl;

	}
}

namespace graphicsToolKit {
	struct PipelineInfo {

		const char* vertexShaderPath;
		const char* fragmentShaderPath;
		VkRenderPass* rdpass;
		std::vector<Atributes> atributes;
		uint32_t subpass = 0;
		VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE;
		VkBool32 dephTest = VK_TRUE;
		VkFrontFace frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		uint32_t colorAttCount = 1;
		uint32_t pushConstanteCount = 0;
		bool depthBias = 0;
		VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		uint32_t vertexBindingCount = 1;
		std::vector<uint32_t> vertexOffsets;
		bool alphablending = false;

	};
	enum pipelineType {
		COOK_TORRANCE,
		PHONG_LIGHTING,
		CUSTOM_SHADER,
		NO_LIGHTING
	};

	

	inline VkResult  createImageView(VkDevice device, VkImageView& view, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType  viewType = VK_IMAGE_VIEW_TYPE_2D, uint32_t layerCount = 1,uint32_t numMips = 1) {
	
		
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = viewType;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = numMips;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = layerCount;
		VkResult result;
		result = vkCreateImageView(device, &viewInfo, nullptr, &view);

		return result;
	}

	inline VkShaderModule createShaderModule(const VkDevice& device, const std::vector<char>& code) {

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
		vkTools::checkVkresult(result, "FTC shaderModule\n");
		return shaderModule;

	}

	inline uint32_t findMemoryType(const VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		std::cout << "Failed to find a suitable Memory\n";


	}

	inline void createBuffer(const VkPhysicalDevice& physicalDevice, const VkDevice* device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(*device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(*device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(*device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(*device, buffer, bufferMemory, 0);
	}

	inline VkCommandBuffer beginSingleTimeCmdBuffer(VkDevice device, VkCommandPool pool) {

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = pool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	inline void endSingleTimeCmdBuffer(VkCommandBuffer& cmd, VkDevice device, VkQueue queue, VkCommandPool& pool) {
		vkEndCommandBuffer(cmd);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmd;
		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(queue);

		vkFreeCommandBuffers(device, pool, 1, &cmd);
	}

	inline void setImageLayout(VkQueue queue, VkDevice device, VkCommandPool pool, VkImage& image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newLayout, uint32_t layerCount, int baseLayer,uint32_t mipLevels = 1) {

		VkCommandBuffer commandBuffer = graphicsToolKit::beginSingleTimeCmdBuffer(device, pool);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldImageLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = mipLevels;
		barrier.subresourceRange.baseArrayLayer = baseLayer;
		barrier.subresourceRange.layerCount = layerCount;

		VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

		switch (oldImageLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			barrier.srcAccessMask = 0;
			break;

		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			break;
		}

		switch (newLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:

			barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (barrier.srcAccessMask == 0)
			{
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			std::cout << "Layout transition not supported";
			break;
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage,
			destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		graphicsToolKit::endSingleTimeCmdBuffer(commandBuffer, device, queue, pool);
	}

	inline void setImageLayout(VkQueue queue, VkDevice device, VkCommandPool pool, VkImage& image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newLayout, uint32_t layerCount, int baseLayer, VkCommandBuffer commandBuffer, uint32_t levelCount = 1) {

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldImageLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = levelCount;
		barrier.subresourceRange.baseArrayLayer = baseLayer;
		barrier.subresourceRange.layerCount = layerCount;

		VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

		switch (oldImageLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			barrier.srcAccessMask = 0;
			break;

		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			break;
		}

		switch (newLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:

			barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (barrier.srcAccessMask == 0)
			{
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			std::cout << "Layout transition not supported";
			break;
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage,
			destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

	}

	inline void createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags, uint32_t graphicsFamilyIndex, const VkDevice& device) {
		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = graphicsFamilyIndex;
		commandPoolCreateInfo.flags = flags;

		if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, commandPool) != VK_SUCCESS) {
			throw std::runtime_error("Could not create graphics command pool");
		}
	}

	inline void createCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool& commandPool, const VkDevice device) {
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
		vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffer);
	}

	inline void copyBuffer(VkCommandPool pool, VkDevice device, VkBuffer src, VkBuffer dst, VkDeviceSize size, const VkQueue* queue)
	{
		VkCommandBuffer cmd = beginSingleTimeCmdBuffer(device, pool);
		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(cmd, src, dst, 1, &copyRegion);

		graphicsToolKit::endSingleTimeCmdBuffer(cmd, device, *queue, pool);
	}

	inline void copyBufferToImage(VkBuffer buffer, VkImage image, VkDevice device, VkCommandPool pool, uint32_t width, uint32_t height, uint32_t baseLayer, uint32_t layerCount, VkQueue queue) {

		VkCommandBuffer cmdBuffer = graphicsToolKit::beginSingleTimeCmdBuffer(device, pool);

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = baseLayer;
		region.imageSubresource.layerCount = layerCount;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};
		vkCmdCopyBufferToImage(cmdBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		graphicsToolKit::endSingleTimeCmdBuffer(cmdBuffer, device, queue, pool);
	}

	inline void createImage(const VkPhysicalDevice& physicalDevice, const VkDevice& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkImageCreateFlags flags, uint32_t arrayLayers,uint32_t numMips = 1) {
		

		
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = numMips;
		imageInfo.arrayLayers = arrayLayers;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = flags;
		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = graphicsToolKit::findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
	
		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device, image, imageMemory, 0);
	}

	inline VkResult createDescriptorSetLayout(const VkDevice* device, VkDescriptorSetLayoutCreateInfo* layoutInfo, VkDescriptorSetLayout* descriptorsetLayout) {
		return vkCreateDescriptorSetLayout(*device, layoutInfo, nullptr, descriptorsetLayout);
	}

	inline void createDescriptorPool(const VkDevice* device, std::vector<VkDescriptorPoolSize> poolSizes, VkDescriptorPool& descriptorPool) {
		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		createInfo.pPoolSizes = poolSizes.data();
		createInfo.maxSets = static_cast<uint32_t>(poolSizes.size() * 25);

		VkResult result = vkCreateDescriptorPool(*device, &createInfo, nullptr, &descriptorPool);
		vkTools::checkVkresult(result, "FTC descriptorPool\n");
	};

	inline void createRenderPass(const VkDevice* device, Pass& pass, std::vector<VkAttachmentDescription> attDescriptions, std::vector<VkAttachmentReference>references, std::vector<VkSubpassDescription>subpasses, std::vector<VkSubpassDependency>dependencies) {

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attDescriptions.size());
		renderPassInfo.pAttachments = attDescriptions.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
		renderPassInfo.pSubpasses = subpasses.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());;
		renderPassInfo.pDependencies = dependencies.data();

		if (vkCreateRenderPass(*device, &renderPassInfo, nullptr, &pass.renderpass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	inline VkResult createPipeline(const VkDevice* device, VkPipeline& pipeline, VkPipelineLayout& pipelineLayout, PipelineInfo& info,uint32_t bindingCount = 1) {

		VertexLayout vertexLayout(info.atributes,info.vertexOffsets,info.vertexBindingCount);

		// Pipeline
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = initializers::pipelineInputAssemblyStateCreateInfo(info.topology, 0, VK_FALSE);
		VkPipelineRasterizationStateCreateInfo rasterizationState = initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, info.cullMode, info.frontFaceClock);

		if (info.depthBias) { 
			rasterizationState.depthBiasEnable = 1;
			rasterizationState.depthBiasConstantFactor = 4.0f;
			rasterizationState.depthBiasSlopeFactor = 1.50f;
		}
		VkPipelineColorBlendAttachmentState* colorBlends = new VkPipelineColorBlendAttachmentState[info.colorAttCount];
		VkPipelineColorBlendAttachmentState blendAttachmentState;

		for (unsigned int i = 0; i < info.colorAttCount; i++) {
			if (!info.alphablending) {
				 blendAttachmentState = initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
			}
			else {
				blendAttachmentState.blendEnable = VK_TRUE;
				blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
				blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
				blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
				blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
				blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			}
				colorBlends[i] = blendAttachmentState;
		}

		VkPipelineColorBlendStateCreateInfo colorBlendState = initializers::pipelineColorBlendStateCreateInfo(info.colorAttCount, colorBlends);
		VkPipelineDepthStencilStateCreateInfo depthStencilState = initializers::pipelineDepthStencilStateCreateInfo(info.dephTest, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
		VkPipelineViewportStateCreateInfo viewportState = initializers::pipelineViewportStateCreateInfo(1, 1, VK_DYNAMIC_STATE_VIEWPORT);
		VkPipelineMultisampleStateCreateInfo multisampleState = initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
		std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState = initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);

		//Vertex input state

		VkPipelineVertexInputStateCreateInfo vertexInputState{};
		VkVertexInputBindingDescription bindingDescription = vertexLayout.getBinding(0);
		std::vector<VkVertexInputAttributeDescription> atribDescription;
		vertexLayout.getAttributeDescription(0, atribDescription);

		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(atribDescription.size());
		vertexInputState.pVertexAttributeDescriptions = atribDescription.data();

		if (bindingCount != 0) {
			vertexInputState.pVertexBindingDescriptions = &bindingDescription;
			vertexInputState.vertexBindingDescriptionCount = bindingCount;
		}
		else {
			vertexInputState.vertexBindingDescriptionCount = 0;


		}
		auto vertShaderCode = generalUtiliy::readFile(info.vertexShaderPath);
		auto fragShaderCode = generalUtiliy::readFile(info.fragmentShaderPath);

		auto vert = graphicsToolKit::createShaderModule(*device, vertShaderCode);
		auto frag = graphicsToolKit::createShaderModule(*device, fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShader = {};
		vertShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShader.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShader.module = vert;
		vertShader.pName = "main";

		VkPipelineShaderStageCreateInfo fragShader = {};
		fragShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShader.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShader.module = frag;
		fragShader.pName = "main";

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;


		shaderStages.resize(2);

		shaderStages = { vertShader,fragShader };

		VkGraphicsPipelineCreateInfo pipelineCI = initializers::pipelineCreateInfo(pipelineLayout, *info.rdpass);
		pipelineCI.pInputAssemblyState = &inputAssemblyState;
		pipelineCI.pRasterizationState = &rasterizationState;
		pipelineCI.pColorBlendState = &colorBlendState;
		pipelineCI.pMultisampleState = &multisampleState;
		pipelineCI.pViewportState = &viewportState;
		pipelineCI.pDepthStencilState = &depthStencilState;
		pipelineCI.pDynamicState = &dynamicState;
		pipelineCI.stageCount = 2;

		pipelineCI.pStages = shaderStages.data();


		pipelineCI.pVertexInputState = &vertexInputState;
		pipelineCI.renderPass = *info.rdpass;
		pipelineCI.subpass = info.subpass;

		VkResult r = vkCreateGraphicsPipelines(*device, nullptr, 1, &pipelineCI, nullptr, &pipeline);

		//delete (colorBlends);
		vkDestroyShaderModule(*device, vert, nullptr);
		vkDestroyShaderModule(*device, frag, nullptr);

		delete[] colorBlends;
		return r;
	}
	/**/
	/*
	inline void createPipeline(const VkDevice* device, const char* vertexShaderPath, const char* fragShaderPath, PipelineStc& pipeline, VkRenderPass* rdpass, std::vector<Atributes> atributes, uint32_t subpass = 0, VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE, VkBool32 dephTest = VK_TRUE, VkFrontFace frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE, uint32_t colorAttCount = 1, uint32_t pushConstanteCount = 0, bool depthOnly = 0, bool depthBias = 0) {

		VertexLayout vertexLayout(atributes);

		// Pipeline
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
		VkPipelineRasterizationStateCreateInfo rasterizationState = initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, cullMode, frontFaceClock);

		if (depthBias) {
			rasterizationState.depthBiasEnable = 1;
			rasterizationState.depthBiasConstantFactor = 4.0f;
			rasterizationState.depthBiasSlopeFactor = 1.50f;
		}
		VkPipelineColorBlendAttachmentState* colorBlends = new VkPipelineColorBlendAttachmentState[colorAttCount];

		for (unsigned int i = 0; i < colorAttCount; i++) {
			VkPipelineColorBlendAttachmentState blendAttachmentState = initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);

			colorBlends[i] = blendAttachmentState;

		}

		VkPipelineColorBlendStateCreateInfo colorBlendState = initializers::pipelineColorBlendStateCreateInfo(colorAttCount, colorBlends);
		VkPipelineDepthStencilStateCreateInfo depthStencilState = initializers::pipelineDepthStencilStateCreateInfo(dephTest, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
		VkPipelineViewportStateCreateInfo viewportState = initializers::pipelineViewportStateCreateInfo(1, 1, VK_DYNAMIC_STATE_VIEWPORT);
		VkPipelineMultisampleStateCreateInfo multisampleState = initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
		std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState = initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);

		//Vertex input state

		VkPipelineVertexInputStateCreateInfo vertexInputState{};
		VkVertexInputBindingDescription bindingDescription = vertexLayout.getBinding(0);
		std::vector<VkVertexInputAttributeDescription> atribDescription;
		vertexLayout.getAttributeDescription(0, atribDescription);

		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(atribDescription.size());
		vertexInputState.pVertexAttributeDescriptions = atribDescription.data();
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.vertexBindingDescriptionCount = 1;

		auto vertShaderCode = generalUtiliy::readFile(vertexShaderPath);
		auto fragShaderCode = generalUtiliy::readFile(fragShaderPath);

		auto vert = graphicsToolKit::createShaderModule(*device, vertShaderCode);
		auto frag = graphicsToolKit::createShaderModule(*device, fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShader = {};
		vertShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShader.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShader.module = vert;
		vertShader.pName = "main";

		VkPipelineShaderStageCreateInfo fragShader = {};
		fragShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShader.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShader.module = frag;
		fragShader.pName = "main";

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		if (depthOnly) {
			shaderStages.resize(1);
			shaderStages = { vertShader };
		}

		else {
			shaderStages.resize(2);

			shaderStages = { vertShader,fragShader };
		}
		VkGraphicsPipelineCreateInfo pipelineCI = initializers::pipelineCreateInfo(pipeline.pipelinelayout, *rdpass);
		pipelineCI.pInputAssemblyState = &inputAssemblyState;
		pipelineCI.pRasterizationState = &rasterizationState;
		pipelineCI.pColorBlendState = &colorBlendState;
		pipelineCI.pMultisampleState = &multisampleState;
		pipelineCI.pViewportState = &viewportState;
		pipelineCI.pDepthStencilState = &depthStencilState;
		pipelineCI.pDynamicState = &dynamicState;
		pipelineCI.stageCount = 2;

		if (depthOnly > 0)
			pipelineCI.pStages = &shaderStages[0];
		else {

			pipelineCI.pStages = shaderStages.data();

		}
		pipelineCI.pVertexInputState = &vertexInputState;
		pipelineCI.renderPass = *rdpass;
		pipelineCI.subpass = subpass;

		vkCreateGraphicsPipelines(*device, nullptr, 1, &pipelineCI, nullptr, &pipeline.pipeline);

		//delete (colorBlends);
		vkDestroyShaderModule(*device, vert, nullptr);
		if (!depthOnly)
			vkDestroyShaderModule(*device, frag, nullptr);


	}

	inline void createPipeline(const VkDevice* device, const char* vertexShaderPath, const char* fragShaderPath, const char* geometryShaderPath, PipelineStc& pipeline, VkRenderPass* rdpass, std::vector<Atributes> atributes, uint32_t subpass = 0, VkCullModeFlagBits cullMode = VK_CULL_MODE_NONE, VkBool32 dephTest = VK_TRUE, VkFrontFace frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE, uint32_t colorAttCount = 1, uint32_t pushConstanteCount = 0, bool depthOnly = 0, bool depthBias = 0) {

		VertexLayout vertexLayout(atributes);

		// Pipeline
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
		VkPipelineRasterizationStateCreateInfo rasterizationState = initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, cullMode, frontFaceClock);

		if (depthBias) {
			rasterizationState.depthBiasEnable = 1;
			rasterizationState.depthBiasConstantFactor = 4.0f;
			rasterizationState.depthBiasSlopeFactor = 1.50f;
		}
		VkPipelineColorBlendAttachmentState* colorBlends = new VkPipelineColorBlendAttachmentState[colorAttCount];

		for (unsigned int i = 0; i < colorAttCount; i++) {
			VkPipelineColorBlendAttachmentState blendAttachmentState = initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);

			colorBlends[i] = blendAttachmentState;

		}

		VkPipelineColorBlendStateCreateInfo colorBlendState = initializers::pipelineColorBlendStateCreateInfo(colorAttCount, colorBlends);
		VkPipelineDepthStencilStateCreateInfo depthStencilState = initializers::pipelineDepthStencilStateCreateInfo(dephTest, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
		VkPipelineViewportStateCreateInfo viewportState = initializers::pipelineViewportStateCreateInfo(1, 1, VK_DYNAMIC_STATE_VIEWPORT);
		VkPipelineMultisampleStateCreateInfo multisampleState = initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
		std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState = initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);

		//Vertex input state

		VkPipelineVertexInputStateCreateInfo vertexInputState{};
		VkVertexInputBindingDescription bindingDescription = vertexLayout.getBinding(0);
		std::vector<VkVertexInputAttributeDescription> atribDescription;
		vertexLayout.getAttributeDescription(0, atribDescription);

		vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(atribDescription.size());
		vertexInputState.pVertexAttributeDescriptions = atribDescription.data();
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.vertexBindingDescriptionCount = 1;

		auto vertShaderCode = generalUtiliy::readFile(vertexShaderPath);
		auto fragShaderCode = generalUtiliy::readFile(fragShaderPath);
		auto geomShaderCode = generalUtiliy::readFile(geometryShaderPath);

		auto vert = graphicsToolKit::createShaderModule(*device, vertShaderCode);
		auto frag = graphicsToolKit::createShaderModule(*device, fragShaderCode);
		auto geo = graphicsToolKit::createShaderModule(*device, geomShaderCode);

		VkPipelineShaderStageCreateInfo vertShader = {};
		vertShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShader.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShader.module = vert;
		vertShader.pName = "main";

		VkPipelineShaderStageCreateInfo fragShader = {};
		fragShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShader.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShader.module = frag;
		fragShader.pName = "main";

		VkPipelineShaderStageCreateInfo geomShader = {};
		geomShader.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		geomShader.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
		geomShader.module = geo;
		geomShader.pName = "main";

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		if (depthOnly) {
			shaderStages.resize(1);
			shaderStages = { vertShader };
		}

		else {
			shaderStages.resize(3);

			shaderStages = { vertShader,fragShader,geomShader };
		}
		VkGraphicsPipelineCreateInfo pipelineCI = initializers::pipelineCreateInfo(pipeline.pipelinelayout, *rdpass);
		pipelineCI.pInputAssemblyState = &inputAssemblyState;
		pipelineCI.pRasterizationState = &rasterizationState;
		pipelineCI.pColorBlendState = &colorBlendState;
		pipelineCI.pMultisampleState = &multisampleState;
		pipelineCI.pViewportState = &viewportState;
		pipelineCI.pDepthStencilState = &depthStencilState;
		pipelineCI.pDynamicState = &dynamicState;
		pipelineCI.stageCount = 3;

		if (depthOnly > 0)
			pipelineCI.pStages = &shaderStages[0];
		else {

			pipelineCI.pStages = shaderStages.data();

		}
		pipelineCI.pVertexInputState = &vertexInputState;
		pipelineCI.renderPass = *rdpass;
		pipelineCI.subpass = subpass;

		vkCreateGraphicsPipelines(*device, nullptr, 1, &pipelineCI, nullptr, &pipeline.pipeline);

		//delete (colorBlends);
		vkDestroyShaderModule(*device, vert, nullptr);
		if (!depthOnly)
			vkDestroyShaderModule(*device, frag, nullptr);
		vkDestroyShaderModule(*device, geo, nullptr);



	}
	*/
	inline void createSampler
	(const VkDevice* device, VkSampler& sampler) {

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


		if (vkCreateSampler(*device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	inline void createPlane(Buffer* buffer, const VkPhysicalDevice* physicalDevice, const VkDevice* device, const VkQueue* queue, VkCommandPool commandPool) {

		struct vertex {
			glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 uv;

		};

		std::vector<float> vertices = {

			// positions				//Normal				// texture coords
			 100.f, 0.0f,  100.0f,		0.0f,-1.0f,0.0f,		 1.0f, 1.0f, // top right
			 100.f, 0.0f, -100.0f,		0.0f,-1.0f,0.0f,		 1.0f, 0.0f, // bottom right
			-100.f, 0.0f, -100.0f,		0.0f,-1.0f,0.0f,		 0.0f, 0.0f, // bottom left
			-100.f, 0.0f,  100.0f,		0.0f,-1.0f,0.0f,		 0.0f, 1.0f  // top left 

		};

		VkDeviceSize size = sizeof(vertex) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*device, stagingBufferMemory, 0, size, 0, &data);
		memcpy(data, vertices.data(), size);
		vkUnmapMemory(*device, stagingBufferMemory);

		graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer->buffer, buffer->bufferMemory);

		graphicsToolKit::copyBuffer(commandPool, *device, stagingBuffer, buffer->buffer, size, queue);

		vkDestroyBuffer(*device, stagingBuffer, nullptr);
		vkFreeMemory(*device, stagingBufferMemory, nullptr);

	}

	inline void createCube(Buffer* buffer, const VkPhysicalDevice* physicalDevice, const VkDevice* device, const VkQueue* queue, VkCommandPool commandPool) {

		struct vertex {
			glm::vec3 pos;
		};

		std::vector<float> vertices = {

			-1.0f, -1.0f, -1.0f, // bottom-left
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f, // top-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f, -1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-right
			 1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f,  1.0f, // top-left
			-1.0f, -1.0f,  1.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right         
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f, // top-left
			 1.0f,  1.0f , 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right     
			 1.0f,  1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f,  1.0f,  1.0f // bottom-left  
		};

		VkDeviceSize size = sizeof(vertex) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*device, stagingBufferMemory, 0, size, 0, &data);
		memcpy(data, vertices.data(), size);
		vkUnmapMemory(*device, stagingBufferMemory);

		graphicsToolKit::createBuffer(*physicalDevice, device, size, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer->buffer, buffer->bufferMemory);

		graphicsToolKit::copyBuffer(commandPool, *device, stagingBuffer, buffer->buffer, size, queue);

		vkDestroyBuffer(*device, stagingBuffer, nullptr);
		vkFreeMemory(*device, stagingBufferMemory, nullptr);

	}

	inline void createCubeMap(const VkDevice* device, const VkPhysicalDevice* physicalDevice, VkFormat format, int dim, VkImage* image, VkImageView* imageView, VkDeviceMemory* imageMemory, uint32_t numMips = 1) {

		VkImageCreateInfo imageCreateInfo = initializers::imageCreateInfo();
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = format;
		imageCreateInfo.extent.width = dim;
		imageCreateInfo.extent.height = dim;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = numMips;
		imageCreateInfo.arrayLayers = 6;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		vkCreateImage(*device, &imageCreateInfo, nullptr, image);

		VkMemoryAllocateInfo memAlloc = initializers::memoryAllocateInfo();
		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(*device, *image, &memReqs);
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = findMemoryType(*physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vkAllocateMemory(*device, &memAlloc, nullptr, imageMemory);
		vkBindImageMemory(*device, *image, *imageMemory, 0);

		VkImageViewCreateInfo colorImageView = initializers::imageViewCreateInfo();
		colorImageView.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		colorImageView.format = format;
		colorImageView.subresourceRange = {};
		colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorImageView.subresourceRange.baseMipLevel = 0;
		colorImageView.subresourceRange.levelCount = numMips;
		colorImageView.subresourceRange.baseArrayLayer = 0;
		colorImageView.subresourceRange.layerCount = 6;
		colorImageView.image = *image;

		if (vkCreateImageView(*device, &colorImageView, nullptr, imageView) != VK_SUCCESS) {
			std::cout << "Failed to create cubeMapTexture\n";
		}

	}

	inline void allocateDescriptorSet(const VkDevice* device, VkDescriptorPool& pool, std::vector<VkDescriptorSet>& set, VkDescriptorSetLayout* setLayout) {
		VkDescriptorSetAllocateInfo alloc = initializers::descriptorSetAllocateInfo(pool, setLayout, static_cast<uint32_t>(set.size()));
		for (unsigned int i = 0; i < set.size(); i++) {
			if (vkAllocateDescriptorSets(*device, &alloc, set.data()) != VK_SUCCESS) {
				std::cout << "FTC Allocate DescriptorSet\n";
				break;
			}
		}
	}

	inline void getTextureImageExtent(int& width, int& height, std::string path) {
		int texChannels;
		stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);

		free(pixels);
	}

	inline void createHostVisbleTextureBuffer(const VkDevice* device, const VkPhysicalDevice* physicalDevice, Buffer& buffer, std::string path, int& width, int& height) {

		int  texChannels;

		stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize;
		imageSize = width * height * 4;

		if (!pixels) {
			throw std::runtime_error("failed to load texture image!   ~~");
		}

		graphicsToolKit::createBuffer(*physicalDevice, device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer.buffer, buffer.bufferMemory);
		void* data;
		vkMapMemory(*device, buffer.bufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(*device, buffer.bufferMemory);

		free(pixels);

	}

	inline void loadSkyboxTextures(const VkDevice* device, const VkPhysicalDevice* physicalDevice, std::array<std::string, 6> files, Texture& cubeMap, VkQueue queue, VkCommandPool* transientPool, VkFormat format = VK_FORMAT_R8G8B8A8_UNORM) {

		std::array<Buffer, 6> buffers;
		int w = 1024;
		graphicsToolKit::createCubeMap(device, physicalDevice, format, w, &cubeMap.image, &cubeMap.view, &cubeMap.imageMemory);

		int width, height;
		for (int i = 0; i < 6; i++) {
			graphicsToolKit::createHostVisbleTextureBuffer(device, physicalDevice, buffers[i], files[i], width, height);
			graphicsToolKit::setImageLayout(queue, *device, *transientPool, cubeMap.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, i);
			graphicsToolKit::copyBufferToImage(buffers[i].buffer, cubeMap.image, *device, *transientPool, width, height, i, 1, queue);
		}

		for (int i = 0; i < 6; i++) {
			graphicsToolKit::setImageLayout(queue, *device, *transientPool, cubeMap.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, i);

			vkFreeMemory(*device, buffers[i].bufferMemory, nullptr);
			vkDestroyBuffer(*device, buffers[i].buffer, nullptr);
		}
	}

	/*inline void createImage(const VkPhysicalDevice *physicalDevice, const VkDevice *device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage & image, VkDeviceMemory & imageMemory, VkImageCreateFlags flags, uint32_t arrayLayers) {

		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = arrayLayers;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = flags;
		if (vkCreateImage(*device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(*device, image, &memRequirements);
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex =graphicsToolKit::findMemoryType(*physicalDevice,memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(*device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}
		vkBindImageMemory(*device, image, imageMemory, 0);
	}
	*/
	inline void generateMips(VkImage image, int width, int height, uint32_t numMips, const VkDevice* device, VkCommandPool pool, VkQueue queue) {

		VkCommandBuffer cmd = beginSingleTimeCmdBuffer(*device, pool);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mipHeight = height;
		int32_t mipWidth = width;

		for (unsigned int i = 1; i < numMips; i++) {

			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(cmd,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0] = { 0, 0, 0 };
			blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;
			blit.dstOffsets[0] = { 0, 0, 0 };
			blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(cmd,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmd,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (mipWidth > 1) mipWidth /= 2;
			if (mipHeight > 1) mipHeight /= 2;

		}
		//Transition the last mip
		barrier.subresourceRange.baseMipLevel = numMips - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		endSingleTimeCmdBuffer(cmd, *device, queue, pool);

	}

	inline void createTextureImage(const char* path, const VkPhysicalDevice* physicalDevice, const VkDevice* device, VkFormat format, VkImageTiling tiling, VkImage& image, VkDeviceMemory& imageMemory, VkImageCreateFlags flags, uint32_t arrayLayers, VkQueue transferQueue, VkQueue graphicsQueue, VkCommandPool graphicsPool, VkCommandPool transferPool,uint32_t &mipLevels) {
		//std::cout<<"IN PATH : "<<path<<std::endl;
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4.0;
		if (!pixels) {
			std::cout << path<<std::endl;
			throw std::runtime_error("failed to load texture image for path : ~");
		}
		const uint32_t numMips = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
		mipLevels = numMips;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		graphicsToolKit::createBuffer(*physicalDevice, device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(*device, stagingBufferMemory);

		free(pixels);

		graphicsToolKit::createImage(*physicalDevice, *device, texWidth, texHeight, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT| VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory, flags, 1,numMips);
		graphicsToolKit::setImageLayout(transferQueue, *device, transferPool, image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 0,mipLevels);
		graphicsToolKit::copyBufferToImage(stagingBuffer, image, *device, transferPool, texWidth, texHeight, 0, 1, transferQueue);
		//graphicsToolKit::setImageLayout(queue, *device, pool, image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, 0);
		vkDestroyBuffer(*device, stagingBuffer, nullptr);
		vkFreeMemory(*device, stagingBufferMemory, nullptr);
		graphicsToolKit::generateMips(image, texWidth, texHeight, numMips, device,graphicsPool, graphicsQueue);

	}

	inline void convertEquirectImageToCubeMap(std::string& equirectImage, Texture& cubeMap, const VkDevice* device, const VkPhysicalDevice* physicalDevice, VkCommandPool commandPool, VkQueue transferQueue, uint32_t imageCount, int width, int height, VkDescriptorPool* pool) {

		Texture equirectImageCubeMap;
		int dim = WIDTH;

		//Equirect to CubeMap
		VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;

		{
			int texWidth, texHeight, texChannels;
			float * pixels = stbi_loadf(equirectImage.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

			VkDeviceSize imageSize;



			if (!pixels) {
				throw std::runtime_error("failed to load texture image for path HDri Image: ~" + equirectImage);
			}

		

			imageSize = texWidth * texHeight * 4 * sizeof(float);


			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(*physicalDevice, device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(*device, stagingBufferMemory, 0, imageSize, 0, &data);
			memcpy(data, pixels, static_cast<size_t>(imageSize));
			vkUnmapMemory(*device, stagingBufferMemory);

			free(pixels);

			createImage(*physicalDevice, *device, texWidth, texHeight, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, equirectImageCubeMap.image, equirectImageCubeMap.imageMemory, 0, 1);

			setImageLayout(transferQueue, *device, commandPool, equirectImageCubeMap.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 0);
			copyBufferToImage(stagingBuffer, equirectImageCubeMap.image, *device, commandPool, texWidth, texHeight, 0, 1, transferQueue);
			setImageLayout(transferQueue, *device, commandPool, equirectImageCubeMap.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, 0);
			vkDestroyBuffer(*device, stagingBuffer, nullptr);
			vkFreeMemory(*device, stagingBufferMemory, nullptr);

			createImageView(*device, equirectImageCubeMap.view, equirectImageCubeMap.image, format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, 1);

		}

		//getTextureImageExtent(dim, dim, equirectImage);
		graphicsToolKit::createCubeMap(device, physicalDevice, format, dim, &cubeMap.image, &cubeMap.view, &cubeMap.imageMemory);
		setImageLayout(transferQueue, *device, commandPool, cubeMap.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, 0);


		//RenderPass	
		VkRenderPass renderpass;

		VkAttachmentDescription cubeMapFaceAttachment = {};
		cubeMapFaceAttachment.format = format;
		cubeMapFaceAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		cubeMapFaceAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		cubeMapFaceAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		cubeMapFaceAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		cubeMapFaceAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		cubeMapFaceAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		cubeMapFaceAttachment.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;


		VkAttachmentReference cubeMapAttRef = {};
		cubeMapAttRef.attachment = 0;
		cubeMapAttRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorRefs = cubeMapAttRef;

		std::array<VkSubpassDescription, 1> subpass = {};
		subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass[0].colorAttachmentCount = 1;
		subpass[0].pColorAttachments = &colorRefs;

		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
		VkAttachmentDescription attachmentsRendePass = cubeMapFaceAttachment;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &attachmentsRendePass;
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpass.size());
		renderPassInfo.pSubpasses = subpass.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkResult result = vkCreateRenderPass(*device, &renderPassInfo, nullptr, &renderpass);

		vkTools::checkVkresult(result, "FTCSKY RenderPass");
		if (result == VK_SUCCESS) {
			std::cout << "successfully created Spherical Mapping RenderPass  \n";
		}

		Texture offscreen;
		VkFramebuffer frameBuffer;

		// Offfscreen framebuffer
		{
			VkImageCreateInfo imageCreateInfo = initializers::imageCreateInfo();
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = format;
			imageCreateInfo.extent.width = dim;
			imageCreateInfo.extent.height = dim;
			imageCreateInfo.extent.depth = 1;
			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vkCreateImage(*device, &imageCreateInfo, nullptr, &offscreen.image);

			VkMemoryAllocateInfo memAlloc = initializers::memoryAllocateInfo();
			VkMemoryRequirements memReqs;
			vkGetImageMemoryRequirements(*device, offscreen.image, &memReqs);
			memAlloc.allocationSize = memReqs.size;
			memAlloc.memoryTypeIndex = findMemoryType(*physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			vkAllocateMemory(*device, &memAlloc, nullptr, &offscreen.imageMemory);
			vkBindImageMemory(*device, offscreen.image, offscreen.imageMemory, 0);

			VkImageViewCreateInfo colorImageView = initializers::imageViewCreateInfo();
			colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorImageView.format = format;
			colorImageView.flags = 0;
			colorImageView.subresourceRange = {};
			colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorImageView.subresourceRange.baseMipLevel = 0;
			colorImageView.subresourceRange.levelCount = 1;
			colorImageView.subresourceRange.baseArrayLayer = 0;
			colorImageView.subresourceRange.layerCount = 1;
			colorImageView.image = offscreen.image;

			vkCreateImageView(*device, &colorImageView, nullptr, &offscreen.view);

			VkFramebufferCreateInfo fbufCreateInfo = initializers::framebufferCreateInfo();
			fbufCreateInfo.renderPass = renderpass;
			fbufCreateInfo.attachmentCount = 1;
			fbufCreateInfo.pAttachments = &offscreen.view;
			fbufCreateInfo.width = dim;
			fbufCreateInfo.height = dim;
			fbufCreateInfo.layers = 1;
			vkCreateFramebuffer(*device, &fbufCreateInfo, nullptr, &frameBuffer);

			VkCommandBuffer cmd = beginSingleTimeCmdBuffer(*device, commandPool);
			setImageLayout(transferQueue, *device, commandPool, offscreen.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, 0, cmd);
			endSingleTimeCmdBuffer(cmd, *device, transferQueue, commandPool);
		}


		//Pipeline

		VkDescriptorSet descriptorSets;

		PipelineStc pipelineExt{};
		pipelineExt.descriptorSetLayout.resize(1);

		std::vector<VkDescriptorSetLayoutBinding> bindings{
			initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,VK_SHADER_STAGE_FRAGMENT_BIT,0),

		};

		VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = initializers::descriptorSetLayoutCreateInfo(bindings);
		graphicsToolKit::createDescriptorSetLayout(device, &descriptorsetlayoutCI, &pipelineExt.descriptorSetLayout[0]);

		const char* vertexPath = "Aplication\\Shaders\\cubeMap\\equirectangular\\vert.spv";
		const char* fragmentPath = "Aplication\\Shaders\\cubeMap\\equirectangular\\frag.spv";

		struct PushConstant {
			glm::mat4 view;
			glm::mat4 projection;
		}push;

		VkPushConstantRange	pushConstantRange = initializers::pushConstantRange(
			VK_SHADER_STAGE_VERTEX_BIT,
			sizeof(PushConstant),
			0);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = initializers::pipelineLayoutCreateInfo(&pipelineExt.descriptorSetLayout[0], 1);
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		result = vkCreatePipelineLayout(*device, &pipelineLayoutInfo, nullptr, &pipelineExt.pipelinelayout);
		vkTools::checkVkresult(result, "FTC PipelineLayout");


		std::vector<Atributes> atributes = { VEC3 };
		PipelineInfo pipelineInfo{};
		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 1 };


		createPipeline(device, pipelineExt.pipeline, pipelineExt.pipelinelayout, pipelineInfo);


		//createPipeline(device, vertexPath, fragmentPath, pipelineExt, &renderpass, atributes, 0, VK_CULL_MODE_BACK_BIT, 1, VK_FRONT_FACE_COUNTER_CLOCKWISE, 1);


		//DescriptorSet

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(*pool, &pipelineExt.descriptorSetLayout[0], 1);
		result = vkAllocateDescriptorSets(*device, &allocInfo, &descriptorSets);
		vkTools::checkVkresult(result, "FTC Allocate(CUBEMAPS)");

		VkSampler sampler;
		createSampler(device, sampler);

		//Update DescriptorSet

		std::vector<VkDescriptorImageInfo> descriptors = {
		initializers::descriptorImageInfo(sampler,equirectImageCubeMap.view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		};

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

			initializers::writeDescriptorSet(descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &descriptors[0]),


		};
		vkUpdateDescriptorSets(*device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

		//VertexBuffer
		Buffer vertexBuffer;
		graphicsToolKit::createCube(&vertexBuffer, physicalDevice, device, &transferQueue, commandPool);

		//Draw
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[6] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};


		for (int i =0; i < 6 ; i++) {



			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderpass;
			renderPassInfo.framebuffer = frameBuffer;
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent.width = width;
			renderPassInfo.renderArea.extent.height = height;

			VkClearValue clearValues = {};
			clearValues.color = { 0.0f, .0f, 1.0f, 0.f };

			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearValues;

			VkCommandBuffer commandBuffer = beginSingleTimeCmdBuffer(*device, commandPool);

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = width;
			viewport.width = height;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = width;
			rect.extent.height = height;
			rect.offset = { 0,0 };
			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
			vkCmdSetScissor(commandBuffer, 0, 1, &rect);

			VkDeviceSize offsets[1] = { 0 };

			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.buffer, offsets);

			std::array<VkDescriptorSet, 1>sets = { descriptorSets };
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineExt.pipeline);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineExt.pipelinelayout, 0, 1, sets.data(), 0, nullptr);

			push.view = captureViews[i];
			push.projection = captureProjection;

			vkCmdPushConstants(
				commandBuffer,
				pipelineExt.pipelinelayout,
				VK_SHADER_STAGE_VERTEX_BIT,
				0,
				sizeof(PushConstant),
				&push);

			vkCmdDraw(commandBuffer, 36, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffer);

		
			VkImageCopy copyRegion = {};

			copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.srcSubresource.baseArrayLayer = 0;
			copyRegion.srcSubresource.mipLevel = 0;
			copyRegion.srcSubresource.layerCount = 1;
			copyRegion.srcOffset = { 0, 0, 0 };

			copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.dstSubresource.baseArrayLayer = i;
			copyRegion.dstSubresource.mipLevel = 0;
			copyRegion.dstSubresource.layerCount = 1;
			copyRegion.dstOffset = { 0, 0, 0 };

			copyRegion.extent.width = static_cast<uint32_t>(viewport.width);
			copyRegion.extent.height = static_cast<uint32_t>(viewport.height);
			copyRegion.extent.depth = 1;

			vkCmdCopyImage(
				commandBuffer,
				offscreen.image,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				cubeMap.image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&copyRegion);

			setImageLayout(transferQueue, *device, commandPool, offscreen.image, format, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, 0, commandBuffer, 1);

			endSingleTimeCmdBuffer(commandBuffer, *device, transferQueue, commandPool);
		}

		setImageLayout(transferQueue, *device, commandPool, cubeMap.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6, 0);

		//Destroy
		vkDestroySampler(*device, sampler, nullptr);

		vkFreeMemory(*device, vertexBuffer.bufferMemory, nullptr);
		vkDestroyBuffer(*device, vertexBuffer.buffer, nullptr);

		vkFreeMemory(*device, equirectImageCubeMap.imageMemory, nullptr);
		vkDestroyImageView(*device, equirectImageCubeMap.view, nullptr);
		vkDestroyImage(*device, equirectImageCubeMap.image, nullptr);

		vkFreeMemory(*device, offscreen.imageMemory, nullptr);
		vkDestroyImageView(*device, offscreen.view, nullptr);
		vkDestroyImage(*device, offscreen.image, nullptr);

		vkDestroyDescriptorSetLayout(*device, pipelineExt.descriptorSetLayout[0], nullptr);
		vkDestroyPipelineLayout(*device, pipelineExt.pipelinelayout, nullptr);
		vkDestroyPipeline(*device, pipelineExt.pipeline, nullptr);

		vkDestroyFramebuffer(*device, frameBuffer, nullptr);
		vkDestroyRenderPass(*device, renderpass, nullptr);

	}

	inline void convoluteIrradiance(Texture& convolutionMap, Texture& skybox, const VkDevice* device, const VkPhysicalDevice* physicalDevice, VkCommandPool commandPool, VkQueue transferQueue, uint32_t imageCount, int fdsf, int fssd, VkDescriptorPool* pool, VkSampler sampler) {

		auto tStart = std::chrono::high_resolution_clock::now();

		VkFormat format = VK_FORMAT_R16G16B16A16_SFLOAT;

		//RenderPass	
		VkRenderPass renderpass;
		uint32_t dim = 64;

		Texture image;
		graphicsToolKit::createImage(*physicalDevice, *device, dim, dim, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image.image, image.imageMemory, 0, 1);
		graphicsToolKit::createImageView(*device, image.view, image.image, format, VK_IMAGE_ASPECT_COLOR_BIT);

		VkAttachmentDescription irradianceAttachment = {};
		irradianceAttachment.format = format;
		irradianceAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		irradianceAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		irradianceAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		irradianceAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		irradianceAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		irradianceAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		irradianceAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference cubeMapAttRef = {};
		cubeMapAttRef.attachment = 0;
		cubeMapAttRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


		VkAttachmentReference colorRef = cubeMapAttRef;

		std::array<VkSubpassDescription, 1> subpass = {};
		subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass[0].colorAttachmentCount = 1;
		subpass[0].pColorAttachments = &colorRef;

		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
		std::array<VkAttachmentDescription, 1> attachmentsRendePass = { irradianceAttachment };

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentsRendePass.size());
		renderPassInfo.pAttachments = attachmentsRendePass.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpass.size());
		renderPassInfo.pSubpasses = subpass.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkResult result = vkCreateRenderPass(*device, &renderPassInfo, nullptr, &renderpass);

		vkTools::checkVkresult(result, "FTCSKY RenderPass");
		if (result == VK_SUCCESS) {
			std::cout << "successfully created irradianceRenderPass  \n";
		}

		VkBool32 frameBufferCreation = VK_FALSE;

		std::vector<VkFramebuffer> frameBuffers;
		frameBuffers.resize(1);

		for (unsigned int i = 0; i < 1; i++) {
			std::array<VkImageView, 1> attachments = { image.view };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = renderpass;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.layers = 1;
			createInfo.width = static_cast<uint32_t>(dim);
			createInfo.height = static_cast<uint32_t>(dim);

			VkResult result = vkCreateFramebuffer(*device, &createInfo, nullptr, &frameBuffers[i]);

			vkTools::checkVkresult(result, "FTC Main FrameBuffers\n");
			frameBufferCreation += result;

		}
		if (frameBufferCreation == 0)std::cout << "Successfully created skyboxFB " << "\n";

		//Pipeline

		VkDescriptorSet descriptorSets;

		PipelineStc pipelineExt{};
		pipelineExt.descriptorSetLayout.resize(1);

		std::vector<VkDescriptorSetLayoutBinding> bindings{
			initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,VK_SHADER_STAGE_FRAGMENT_BIT,0)

		};

		VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = initializers::descriptorSetLayoutCreateInfo(bindings);
		graphicsToolKit::createDescriptorSetLayout(device, &descriptorsetlayoutCI, &pipelineExt.descriptorSetLayout[0]);

		const char* vertexPath = "Aplication\\Shaders\\cubemap\\convolution\\vert.spv";
		const char* fragmentPath = "Aplication\\Shaders\\cubemap\\convolution\\frag.spv";

		struct PushConstant {
			glm::mat4 view;
			glm::mat4 projection;
		}push;

		VkPushConstantRange	pushConstantRange = initializers::pushConstantRange(
			VK_SHADER_STAGE_VERTEX_BIT,
			sizeof(PushConstant),
			0);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = initializers::pipelineLayoutCreateInfo(&pipelineExt.descriptorSetLayout[0], 1);
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		result = vkCreatePipelineLayout(*device, &pipelineLayoutInfo, nullptr, &pipelineExt.pipelinelayout);
		vkTools::checkVkresult(result, "FTC PipelineLayout");


		std::vector<Atributes> atributes = { VEC3 };
		PipelineInfo pipelineInfo{};
		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 1 };


		createPipeline(device, pipelineExt.pipeline, pipelineExt.pipelinelayout, pipelineInfo);


		//graphicsToolKit::createPipeline(device, vertexPath, fragmentPath, pipelineExt, &renderpass, atributes, 0, VK_CULL_MODE_BACK_BIT, 1, VK_FRONT_FACE_COUNTER_CLOCKWISE, 1, 1);

		//DescriptorSet

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(*pool, &pipelineExt.descriptorSetLayout[0], 1);
		result = vkAllocateDescriptorSets(*device, &allocInfo, &descriptorSets);
		vkTools::checkVkresult(result, "FTC Allocate(CONVOLUTION)");

		//Update DescriptorSet

		std::vector<VkDescriptorImageInfo> descriptors = {
		initializers::descriptorImageInfo(sampler, skybox.view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		};

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

			initializers::writeDescriptorSet(descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,0, &descriptors[0]),

		};
		vkUpdateDescriptorSets(*device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

		//VertexBuffer
		Buffer vertexBuffer;


		graphicsToolKit::createCube(&vertexBuffer, physicalDevice, device, &transferQueue, commandPool);

		//Draw
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(-1.0f,  0.0f,  0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};


		VkRenderPassBeginInfo renderPassInfoB{};
		renderPassInfoB.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfoB.renderPass = renderpass;
		renderPassInfoB.framebuffer = frameBuffers[0];
		renderPassInfoB.renderArea.offset = { 0,0 };
		renderPassInfoB.renderArea.extent.width = static_cast<uint32_t>(dim);
		renderPassInfoB.renderArea.extent.height = static_cast<uint32_t>(dim);
		VkClearValue  clearValues = {};

		clearValues.color = { 1.0f, 1.f, .0f, 1.f };

		VkViewport viewport = {};
		viewport.height = dim;
		viewport.width = dim;

		viewport.maxDepth = 1.0f;

		VkRect2D rect = {};
		rect.extent.width = static_cast<uint32_t>(dim);
		rect.extent.height = static_cast<uint32_t>(dim);
		rect.offset = { 0,0 };

		renderPassInfoB.clearValueCount = 1;
		renderPassInfoB.pClearValues = &clearValues;

		//graphicsToolKit::setImageLayout(transferQueue, *device, commandPool, image.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, 0);
		graphicsToolKit::setImageLayout(transferQueue, *device, commandPool, convolutionMap.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, 0);

		for (int i = 0; i < 6; i++) {

			VkCommandBuffer commandBuffer = beginSingleTimeCmdBuffer(*device, commandPool);

			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			vkCmdSetScissor(commandBuffer, 0, 1, &rect);

			vkCmdBeginRenderPass(commandBuffer, &renderPassInfoB, VK_SUBPASS_CONTENTS_INLINE);

			VkDeviceSize offsets[1] = { 0 };

			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.buffer, offsets);

			std::array<VkDescriptorSet, 1>sets = { descriptorSets };
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineExt.pipeline);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineExt.pipelinelayout, 0, 1, sets.data(), 0, nullptr);

			push.view = captureViews[i];
			push.projection = captureProjection;

			vkCmdPushConstants(
				commandBuffer,
				pipelineExt.pipelinelayout,
				VK_SHADER_STAGE_VERTEX_BIT,
				0,
				sizeof(push),
				&push);

			vkCmdDraw(commandBuffer, 36, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffer);

			graphicsToolKit::setImageLayout(transferQueue, *device, commandPool, image.image, format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 1, 0, commandBuffer);

			VkImageCopy copyRegion = {};

			copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.srcSubresource.baseArrayLayer = 0;
			copyRegion.srcSubresource.mipLevel = 0;
			copyRegion.srcSubresource.layerCount = 1;
			copyRegion.srcOffset = { 0, 0, 0 };

			copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.dstSubresource.baseArrayLayer = i;
			copyRegion.dstSubresource.mipLevel = 0;
			copyRegion.dstSubresource.layerCount = 1;
			copyRegion.dstOffset = { 0, 0, 0 };

			copyRegion.extent.width = static_cast<uint32_t>(dim);
			copyRegion.extent.height = static_cast<uint32_t>(dim);
			copyRegion.extent.depth = 1;

			vkCmdCopyImage(
				commandBuffer,
				image.image,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				convolutionMap.image,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&copyRegion);


			endSingleTimeCmdBuffer(commandBuffer, *device, transferQueue, commandPool);

		}

		vkQueueWaitIdle(transferQueue);

		graphicsToolKit::setImageLayout(transferQueue, *device, commandPool, convolutionMap.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6, 0);

		vkFreeMemory(*device, vertexBuffer.bufferMemory, nullptr);
		vkDestroyBuffer(*device, vertexBuffer.buffer, nullptr);


		vkDestroyDescriptorSetLayout(*device, pipelineExt.descriptorSetLayout[0], nullptr);
		vkDestroyPipelineLayout(*device, pipelineExt.pipelinelayout, nullptr);
		vkDestroyPipeline(*device, pipelineExt.pipeline, nullptr);

		vkFreeMemory(*device, image.imageMemory, nullptr);
		vkDestroyImage(*device, image.image, nullptr);
		vkDestroyImageView(*device, image.view, nullptr);

		for (auto frameBuffer : frameBuffers)
			vkDestroyFramebuffer(*device, frameBuffer, nullptr);
		vkDestroyRenderPass(*device, renderpass, nullptr);

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		std::cout << "Generating Irradiance LUT took " << tDiff << " ms" << std::endl;
	}



	inline void preFilterEnviromentMap(Texture& enviromentMap, Texture& filteredMap, const VkDevice* device, const VkPhysicalDevice* physicalDevice, VkCommandPool commandPool, VkQueue transferQueue, uint32_t imageCount, VkDescriptorPool* pool, VkSampler sampler, int32_t dim) {

		auto tStart = std::chrono::high_resolution_clock::now();
		const VkFormat format = VK_FORMAT_R16G16B16A16_SFLOAT;
		const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;

		// FB, Att, RP, Pipe, etc.
		VkAttachmentDescription attDesc = {};
		// Color attachment
		attDesc.format = format;
		attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;

		// Use subpass dependencies for layout transitions
		std::array<VkSubpassDependency, 2> dependencies;
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		// Renderpass
		VkRenderPassCreateInfo renderPassCI = initializers::renderPassCreateInfo();
		renderPassCI.attachmentCount = 1;
		renderPassCI.pAttachments = &attDesc;
		renderPassCI.subpassCount = 1;
		renderPassCI.pSubpasses = &subpassDescription;
		renderPassCI.dependencyCount = 2;
		renderPassCI.pDependencies = dependencies.data();
		VkRenderPass renderpass;
		vkCreateRenderPass(*device, &renderPassCI, nullptr, &renderpass);


		struct {
			VkImage image;
			VkImageView view;
			VkDeviceMemory memory;
			VkFramebuffer framebuffer;
		} offscreen;

		// Offfscreen framebuffer
		{
			VkImageCreateInfo imageCreateInfo = initializers::imageCreateInfo();
			imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			imageCreateInfo.format = format;
			imageCreateInfo.extent.width = dim;
			imageCreateInfo.extent.height = dim;
			imageCreateInfo.extent.depth = 1;
			imageCreateInfo.mipLevels = 1;
			imageCreateInfo.arrayLayers = 1;
			imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vkCreateImage(*device, &imageCreateInfo, nullptr, &offscreen.image);

			VkMemoryAllocateInfo memAlloc = initializers::memoryAllocateInfo();
			VkMemoryRequirements memReqs;
			vkGetImageMemoryRequirements(*device, offscreen.image, &memReqs);
			memAlloc.allocationSize = memReqs.size;
			memAlloc.memoryTypeIndex = findMemoryType(*physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			vkAllocateMemory(*device, &memAlloc, nullptr, &offscreen.memory);
			vkBindImageMemory(*device, offscreen.image, offscreen.memory, 0);

			VkImageViewCreateInfo colorImageView = initializers::imageViewCreateInfo();
			colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorImageView.format = format;
			colorImageView.flags = 0;
			colorImageView.subresourceRange = {};
			colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorImageView.subresourceRange.baseMipLevel = 0;
			colorImageView.subresourceRange.levelCount = 1;
			colorImageView.subresourceRange.baseArrayLayer = 0;
			colorImageView.subresourceRange.layerCount = 1;
			colorImageView.image = offscreen.image;
			vkCreateImageView(*device, &colorImageView, nullptr, &offscreen.view);

			VkFramebufferCreateInfo fbufCreateInfo = initializers::framebufferCreateInfo();
			fbufCreateInfo.renderPass = renderpass;
			fbufCreateInfo.attachmentCount = 1;
			fbufCreateInfo.pAttachments = &offscreen.view;
			fbufCreateInfo.width = dim;
			fbufCreateInfo.height = dim;
			fbufCreateInfo.layers = 1;
			vkCreateFramebuffer(*device, &fbufCreateInfo, nullptr, &offscreen.framebuffer);

			VkCommandBuffer cmd = beginSingleTimeCmdBuffer(*device, commandPool);
			setImageLayout(transferQueue, *device, commandPool, offscreen.image, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, 0, cmd);
			endSingleTimeCmdBuffer(cmd, *device, transferQueue, commandPool);
		}


		// Descriptors
		VkDescriptorSetLayout descriptorsetlayout;
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {
			initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0),
		};
		PipelineStc pipeline;
		pipeline.descriptorSetLayout.resize(1);

		VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
		vkCreateDescriptorSetLayout(*device, &descriptorsetlayoutCI, nullptr, &pipeline.descriptorSetLayout[0]);

		// Descriptor Pool
		std::vector<VkDescriptorPoolSize> poolSizes = { initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1) };
		VkDescriptorPoolCreateInfo descriptorPoolCI = initializers::descriptorPoolCreateInfo(poolSizes, 2);
		VkDescriptorPool descriptorpool;
		vkCreateDescriptorPool(*device, &descriptorPoolCI, nullptr, &descriptorpool);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = enviromentMap.view;
		imageInfo.sampler = sampler;

		// Descriptor sets
		VkDescriptorSet descriptorset;
		VkDescriptorSetAllocateInfo allocInfo = initializers::descriptorSetAllocateInfo(descriptorpool, &pipeline.descriptorSetLayout[0], 1);
		vkAllocateDescriptorSets(*device, &allocInfo, &descriptorset);
		VkWriteDescriptorSet writeDescriptorSet = initializers::writeDescriptorSet(descriptorset, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &imageInfo);
		vkUpdateDescriptorSets(*device, 1, &writeDescriptorSet, 0, nullptr);


		struct PushConstant {
			glm::mat4 view;
			glm::mat4 projection;
			float roughness;
		}pushBlock;

		VkPipelineLayout pipelinelayout;
		std::vector<VkPushConstantRange> pushConstantRanges = {
			initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(pushBlock), 0),
		};
		VkPipelineLayoutCreateInfo pipelineLayoutCI = initializers::pipelineLayoutCreateInfo(&pipeline.descriptorSetLayout[0], 1);
		pipelineLayoutCI.pushConstantRangeCount = 1;
		pipelineLayoutCI.pPushConstantRanges = pushConstantRanges.data();
		vkCreatePipelineLayout(*device, &pipelineLayoutCI, nullptr, &pipeline.pipelinelayout);


		const char* vertexPath = "Aplication\\Shaders\\cubemap\\prefiltered\\vert.spv";
		const char* fragmentPath = "Aplication\\Shaders\\cubemap\\prefiltered\\frag.spv";



		std::vector<Atributes> atributes = { VEC3 };
		PipelineInfo pipelineInfo{};
		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 1 };


		createPipeline(device, pipeline.pipeline, pipeline.pipelinelayout, pipelineInfo);


		//graphicsToolKit::createPipeline(device, vertexPath, fragmentPath, pipeline, &renderpass, atributes, 0, VK_CULL_MODE_BACK_BIT, 1, VK_FRONT_FACE_COUNTER_CLOCKWISE);

		VkClearValue clearValues[1];
		clearValues[0].color = { { 0.0f, 1.0f, .2f, 0.0f } };

		VkRenderPassBeginInfo renderPassBeginInfo = initializers::renderPassBeginInfo();
		// Reuse render pass from example pass
		renderPassBeginInfo.renderPass = renderpass;
		renderPassBeginInfo.framebuffer = offscreen.framebuffer;
		renderPassBeginInfo.renderArea.extent.width = dim;
		renderPassBeginInfo.renderArea.extent.height = dim;
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = clearValues;

		Buffer vertexBuffer;

		createCube(&vertexBuffer, physicalDevice, device, &transferQueue, commandPool);


		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(-1.0f,  0.0f,  0.0f),glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, -.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};


		VkCommandBuffer cmdBuffer = beginSingleTimeCmdBuffer(*device, commandPool);

		VkViewport viewport = initializers::viewport((float)dim, (float)dim, 0.0f, 1.0f);
		VkRect2D scissor = initializers::rect2D(dim, dim, 0, 0);

		vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);
		vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = numMips;
		subresourceRange.layerCount = 6;

		setImageLayout(transferQueue, *device, commandPool, filteredMap.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 6, 0, cmdBuffer, numMips);
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

		pushBlock.projection = captureProjection;

		for (uint32_t m = 0; m < numMips; m++) {
			pushBlock.roughness = (float)m / (float)(numMips - 1);
			for (int f = 0; f < 6; f++) {
				viewport.width = static_cast<float>(dim * std::pow(0.5f, m));
				viewport.height = static_cast<float>(dim * std::pow(0.5f, m));
				vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

				vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
				pushBlock.view = captureViews[f];

				vkCmdPushConstants(cmdBuffer, pipeline.pipelinelayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstant), &pushBlock);

				vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
				vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipelinelayout, 0, 1, &descriptorset, 0, NULL);

				VkDeviceSize offsets[1] = { 0 };
				vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer.buffer, offsets);
				vkCmdDraw(cmdBuffer, 36, 1, 0, 0);

				vkCmdEndRenderPass(cmdBuffer);

				setImageLayout(transferQueue, *device, commandPool, offscreen.image, format, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 1, 0, cmdBuffer);

				VkImageCopy copyRegion = {};

				copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				copyRegion.srcSubresource.baseArrayLayer = 0;
				copyRegion.srcSubresource.mipLevel = 0;
				copyRegion.srcSubresource.layerCount = 1;
				copyRegion.srcOffset = { 0, 0, 0 };

				copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				copyRegion.dstSubresource.baseArrayLayer = f;
				copyRegion.dstSubresource.mipLevel = m;
				copyRegion.dstSubresource.layerCount = 1;
				copyRegion.dstOffset = { 0, 0, 0 };

				copyRegion.extent.width = static_cast<uint32_t>(viewport.width);
				copyRegion.extent.height = static_cast<uint32_t>(viewport.height);
				copyRegion.extent.depth = 1;

				vkCmdCopyImage(
					cmdBuffer,
					offscreen.image,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					filteredMap.image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&copyRegion);

				setImageLayout(transferQueue, *device, commandPool, offscreen.image, format, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, 0, cmdBuffer);
			}
		}

		setImageLayout(transferQueue, *device, commandPool, filteredMap.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 6, 0, cmdBuffer, numMips);

		endSingleTimeCmdBuffer(cmdBuffer, *device, transferQueue, commandPool);

		vkDestroyDescriptorPool(*device, descriptorpool, nullptr);

		vkDestroyBuffer(*device, vertexBuffer.buffer, nullptr);
		vkFreeMemory(*device, vertexBuffer.bufferMemory, nullptr);

		vkDestroyDescriptorSetLayout(*device, pipeline.descriptorSetLayout[0], nullptr);
		vkDestroyPipelineLayout(*device, pipeline.pipelinelayout, nullptr);
		vkDestroyPipeline(*device, pipeline.pipeline, nullptr);

		vkFreeMemory(*device, offscreen.memory, nullptr);
		vkDestroyImage(*device, offscreen.image, nullptr);
		vkDestroyImageView(*device, offscreen.view, nullptr);

		vkDestroyFramebuffer(*device, offscreen.framebuffer, nullptr);
		vkDestroyRenderPass(*device, renderpass, nullptr);

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		std::cout << "Generating pre-filtered enivornment cube with " << numMips << " mip levels took " << tDiff << " ms" << std::endl;

	}

	inline void generateBrdfLUT(Texture& brdfLut, const VkDevice* device, const VkPhysicalDevice* physicalDevice, VkCommandPool commandPool, VkQueue transferQueue, VkDescriptorPool* pool, int32_t dim) {

		auto tStart = std::chrono::high_resolution_clock::now();

		const VkFormat format = VK_FORMAT_R16G16_SFLOAT;	// R16G16 is supported pretty much everywhere

		VkAttachmentDescription attDesc = {};

		// Color attachment
		attDesc.format = format;
		attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;

		// Use subpass dependencies for layout transitions
		std::array<VkSubpassDependency, 2> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		// Create the actual renderpass
		VkRenderPassCreateInfo renderPassCI = initializers::renderPassCreateInfo();
		renderPassCI.attachmentCount = 1;
		renderPassCI.pAttachments = &attDesc;
		renderPassCI.subpassCount = 1;
		renderPassCI.pSubpasses = &subpassDescription;
		renderPassCI.dependencyCount = 2;
		renderPassCI.pDependencies = dependencies.data();

		VkRenderPass renderpass;
		vkCreateRenderPass(*device, &renderPassCI, nullptr, &renderpass);

		VkFramebufferCreateInfo framebufferCI = initializers::framebufferCreateInfo();
		framebufferCI.renderPass = renderpass;
		framebufferCI.attachmentCount = 1;
		framebufferCI.pAttachments = &brdfLut.view;
		framebufferCI.width = dim;
		framebufferCI.height = dim;
		framebufferCI.layers = 1;

		VkFramebuffer framebuffer;
		vkCreateFramebuffer(*device, &framebufferCI, nullptr, &framebuffer);


		PipelineStc pipeline;
		pipeline.descriptorSetLayout.resize(1);

		// Desriptors
		VkDescriptorSetLayout descriptorsetlayout;
		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {};
		VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
		vkCreateDescriptorSetLayout(*device, &descriptorsetlayoutCI, nullptr, &pipeline.descriptorSetLayout[0]);

		VkPipelineLayoutCreateInfo pipelineLayoutCI = initializers::pipelineLayoutCreateInfo(&pipeline.descriptorSetLayout[0], 1);
		vkCreatePipelineLayout(*device, &pipelineLayoutCI, nullptr, &pipeline.pipelinelayout);

		// Descriptor Pool
		std::vector<VkDescriptorPoolSize> poolSizes = { initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1) };
		VkDescriptorPoolCreateInfo descriptorPoolCI = initializers::descriptorPoolCreateInfo(poolSizes, 2);
		VkDescriptorPool descriptorpool;
		vkCreateDescriptorPool(*device, &descriptorPoolCI, nullptr, &descriptorpool);

		// Descriptor sets
		VkDescriptorSet descriptorset;
		VkDescriptorSetAllocateInfo allocInfo = initializers::descriptorSetAllocateInfo(descriptorpool, &pipeline.descriptorSetLayout[0], 1);
		vkAllocateDescriptorSets(*device, &allocInfo, &descriptorset);

		std::vector<Atributes> atributes{NONE};

		const char* vertexPath = "Aplication\\Shaders\\cubemap\\brdfLut\\vert.spv";
		const char* fragmentPath = "Aplication\\Shaders\\cubemap\\brdfLut\\frag.spv";

		PipelineInfo pipelineInfo;
		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_NONE;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.vertexOffsets = { 0 };

	//	graphicsToolKit::createPipeline(device, vertexPath, fragmentPath, pipeline, &renderpass, atributes, 0, VK_CULL_MODE_FRONT_BIT,1, VK_FRONT_FACE_COUNTER_CLOCKWISE, 1);
		if ((graphicsToolKit::createPipeline(device,pipeline.pipeline, pipeline.pipelinelayout, pipelineInfo,0) != VK_SUCCESS)) {
			std::cout << "Failed to create BRDF Pipeline : " <<std::endl;
		};

		// Render
		VkClearValue clearValues[1];
		clearValues[0].color = { { 0.0f, 1.0f} };

		VkRenderPassBeginInfo renderPassBeginInfo = initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = renderpass;
		renderPassBeginInfo.renderArea.extent.width = dim;
		renderPassBeginInfo.renderArea.extent.height = dim;
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = framebuffer;

		VkCommandBuffer cmd = beginSingleTimeCmdBuffer(*device, commandPool);

		vkCmdBeginRenderPass(cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		VkViewport viewport = initializers::viewport((float)dim, (float)dim, 0.0f, 1.0f);
		VkRect2D scissor = initializers::rect2D(dim, dim, 0, 0);
		vkCmdSetViewport(cmd, 0, 1, &viewport);
		vkCmdSetScissor(cmd, 0, 1, &scissor);
		vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
		vkCmdDraw(cmd, 3, 1, 0, 0);
		vkCmdEndRenderPass(cmd);

		endSingleTimeCmdBuffer(cmd, *device, transferQueue, commandPool);

		vkQueueWaitIdle(transferQueue);

		vkDestroyRenderPass(*device, renderpass, nullptr);
		vkDestroyDescriptorSetLayout(*device, pipeline.descriptorSetLayout[0], nullptr);
		vkDestroyPipelineLayout(*device, pipeline.pipelinelayout, nullptr);
		vkDestroyPipeline(*device, pipeline.pipeline, nullptr);
		vkDestroyFramebuffer(*device, framebuffer, nullptr);
		vkDestroyDescriptorPool(*device, descriptorpool, nullptr);

		auto tEnd = std::chrono::high_resolution_clock::now();
		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		std::cout << "Generating BRDF LUT took " << tDiff << " ms" << std::endl;
	}
	
	
}


namespace TUGEV {

	using namespace vkTools;

	struct TimeStep {
		TimeStep(float t) :time(t) {};
		float time;
		float getTimeInSec() { return time; };
		float getTimeinMilli() { return (time * 1000.); };
	};


	struct DescriptorResourceQuantities {
		DescriptorResourceQuantities() { numImageSampler = 0; numBuffer = 0; numInputAttachments = 0; };

		unsigned int numImageSampler;

		unsigned int numBuffer;

		unsigned int numInputAttachments;

	};

	struct DescriptorSetLayout {
		std::string definition;
		VkDescriptorSetLayout vulkanDescriptorSetLayout;
		DescriptorResourceQuantities resourceQuantities;

		DescriptorSetLayout(const char* def) :definition(def) {}  ;
		~DescriptorSetLayout() {};

		void buildDescriptorSetLayout(const VkDevice* device, std::vector<VkDescriptorSetLayoutBinding>& bindings) {
			assert(bindings.size() > 0);

			VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = initializers::descriptorSetLayoutCreateInfo(bindings);
			if (graphicsToolKit::createDescriptorSetLayout(device, &descriptorsetlayoutCI, &vulkanDescriptorSetLayout) != VK_SUCCESS) {
				std::cout << "Failed to create descriptorSetLayout\n";
			};
		}

	};

	struct Descriptors {
		Descriptors() { descriptorSetLayouts.reserve(4); }

		std::vector<std::unique_ptr<DescriptorSetLayout> >descriptorSetLayouts;

		~Descriptors() {};

		void destroyDescriptors(const VkDevice device) {

			for (auto const& d : descriptorSetLayouts) {

				vkDestroyDescriptorSetLayout(device, d->vulkanDescriptorSetLayout, nullptr);

			}
		}
	};


	struct ChildPipeline {

		ChildPipeline() {};
		const char* definition;
		VkPipeline pipeline;
	};

	struct RenderPipeline {

		RenderPipeline(const char* def, unsigned int i) :definition(def), id(i) {};

		//This is used to hold pipelines that are almost identical do this one;

		unsigned int id;
		std::string definition;

		std::unique_ptr<Descriptors> pipelineDescriptorsLayouts;
		VkPipeline pipeline;
		VkPipelineLayout pipelinelayout;


		void buildPipelineLayout(const VkDevice* device) {

			std::vector<VkDescriptorSetLayout> pSetLayouts;

			const unsigned int numDescriptors = pipelineDescriptorsLayouts->descriptorSetLayouts.size();

			for (unsigned int i = 0; i < numDescriptors; ++i) {
				VkDescriptorSetLayout layout = pipelineDescriptorsLayouts.get()->descriptorSetLayouts[i]->vulkanDescriptorSetLayout;
				pSetLayouts.push_back(layout);

			}

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = initializers::pipelineLayoutCreateInfo(&pSetLayouts[0], static_cast<uint32_t>(pSetLayouts.size()));
			VkResult result = vkCreatePipelineLayout(*device, &pipelineLayoutInfo, nullptr, &pipelinelayout);

		}

		void buildPipeline(const VkDevice* device, graphicsToolKit::PipelineInfo& data) {


			if ((graphicsToolKit::createPipeline(device, pipeline, pipelinelayout, data) != VK_SUCCESS)) {
				std::cout << "Failed to create Pipeline : " << definition << std::endl;
			};


		}
			
	
		void destroyPipeline(const VkDevice device) {

			vkDestroyPipelineLayout(device, pipelinelayout, nullptr);
			vkDestroyPipeline(device, pipeline, nullptr);
			pipelineDescriptorsLayouts->destroyDescriptors(device);
			std::cout << "Pipeline : " << definition << " destroyed" << std::endl;
		
		}

	};

	//Not in Use YEt
	struct AttachmentsBlock {

		AttachmentsBlock() {};
		std::vector<std::vector<RenderAttachment> > attachments;
		~AttachmentsBlock() {};

	};

	//Not in Use YEt
	struct FrameBuffer {

		FrameBuffer(const char *def,unsigned int i,unsigned int count,unsigned int id) : definition(def),pipelineID(id) {};
		unsigned int pipelineID; // index of the pipeline used by this frameBuffer
		const char* definition;

		std::unique_ptr<AttachmentsBlock> frameBufferTargets;
		std::vector<VkFramebuffer>vulkanFrameBuffers;


	};


	struct RenderPass {
		RenderPass(const char *def,const VkDevice *d):definition(def),device(d) {};
		const char* definition;
		const VkDevice* device;
		//FrameBuffer holds a bunch of frameBuffers
		std::unique_ptr<FrameBuffer> frameBuffers;
		VkRenderPass renderpass;
		int width;
		int height;
	};




	struct RenderManager {
		RenderManager() {};
		std::vector<std::unique_ptr<RenderPipeline> >renderPipelines;

		void destroyRenderManager(const VkDevice device) {
			for (int i = 0; i < renderPipelines.size(); i++) {
				renderPipelines[i].get()->destroyPipeline(device);
			}
		}
	};



};