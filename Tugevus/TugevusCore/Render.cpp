#include "Render.h"

Render::Render(const char* name,GLFWwindow *_window) :appName(name), debuger(&instance), device(&instance, &surface, window),window(_window)
{
	std::cout << "Render Created  for aplication : " << appName << std::endl;
	//initiateResources();
}

Render::~Render()
{
	std::cout << "\nRender Destroyer---------------------------------------------------------------------------------------------\n";
	renderCleanUp();

}

void Render::initiateResources()
{
	createInstance();
	createSurface();
	sceneSettings.startSceneSettings();
	debuger.setupDebugMessenger(enableValidationlayer);
	device.prepareDevice();
	presentationEngine.preparePresentationEngine(device.getVulkanDevice(), device.getPhysicalDevice(), device.getSharingMode(), surface);
	createDeferredShadingRenderpass();
	createDescriptorPool();
	createDepthAttachment();
	createShadowMapRenderPass();
	createGBufferRenderpass();
	createCommandPool();
	createAmbientOclusionResources();
	createAttachments();
	createPostProcessingRenderPasses();
	creategraphicsPipelines();
	createTransientCommandPool();
	createLightUniformBuffers();
	initiateLightContainer();
	createMainFrameBuffers();
	createSkyEnviroment();
	createmGuiInterface();
	loadSceneData();
	allocatePostProcessingBuffers(); 
	allocateCommandBuffers();
	createDrawCommand();
	createSyncronizationObjects();

}

void Render::waitIdle()
{
	vkDeviceWaitIdle(*device.getDevice());
}

void Render::setSceneGraph(Scenegraph &_sceneGraph)
{
	this->sceneGraph = std::move(_sceneGraph);

}

void Render::setSceneMaterials(Materials& _materials)
{
	this->materials = std::move(_materials);
}

void Render::createAttachments(VkFormat format, VkImageUsageFlags usage, RenderAttachment* attachment, VkExtent2D extent)
{

	VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
	VkImageLayout layout;
	attachment->format = format;

	uint32_t w = extent.width;
	uint32_t h = extent.height;

	if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
		aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}
	if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
		aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
		layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	if (usage & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
		properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
	}

	//const uint32_t numMips = static_cast<uint32_t>(floor(log2(std::max(extent.width, extent.height)))) +1;

	graphicsToolKit::createImage(*device.getPhysicalDevice(), *device.getVulkanDevice(), w, h, format, VK_IMAGE_TILING_OPTIMAL, usage, properties, attachment->image, attachment->memory,0,1);
	graphicsToolKit::createImageView(*device.getVulkanDevice(), attachment->view, attachment->image, format, aspect,VK_IMAGE_VIEW_TYPE_2D,1);
	//graphicsToolKit::generateMips(attachment->image, extent.width, extent.height, numMips,device.getVulkanDevice(), cmdPool,device.getGraphicsQueueHandle());
}

void Render::createIrradianceMap() {

	//	graphicsToolKit::loadSkyboxTextures(device.getDevice(), device.getPhysicalDevice(), files, skybox.cubeMap, *device.getTransferQueueHandle(), &quicklyCmdPool);
	graphicsToolKit::createCubeMap(device.getVulkanDevice(), device.getPhysicalDevice(), VK_FORMAT_R16G16B16A16_SFLOAT, 64, &skybox.irradianceMap.image, &skybox.irradianceMap.view, &skybox.irradianceMap.imageMemory);
	graphicsToolKit::convoluteIrradiance(skybox.irradianceMap, skybox.cubeMap, device.getVulkanDevice(), device.getPhysicalDevice(),transientCmdPool, device.getGraphicsQueueHandle(), presentationEngine.getImageCount(), WIDTH, HEIGHT, &descriptorPool, skybox.sampler);


}

void Render::createpreFilteredEnvMap()
{
	const VkFormat format = VK_FORMAT_R16G16B16A16_SFLOAT;
	const int32_t dim = 512;
	const uint32_t numMips = static_cast<uint32_t>(floor(log2(dim))) + 1;

	VkSamplerCreateInfo samplerCI = initializers::samplerCreateInfo();
	samplerCI.magFilter = VK_FILTER_LINEAR;
	samplerCI.minFilter = VK_FILTER_LINEAR;
	samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.minLod = 0.0f;
	samplerCI.maxLod = static_cast<float>(numMips);
	samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	(vkCreateSampler(*device.getVulkanDevice(), &samplerCI, nullptr, &specularBrdf.sampler));

	graphicsToolKit::createCubeMap(device.getVulkanDevice(), device.getPhysicalDevice(), VK_FORMAT_R16G16B16A16_SFLOAT, dim, &specularBrdf.filteredEnvMap.image, &specularBrdf.filteredEnvMap.view, &specularBrdf.filteredEnvMap.imageMemory, numMips);
	graphicsToolKit::preFilterEnviromentMap(skybox.cubeMap, specularBrdf.filteredEnvMap, device.getVulkanDevice(), device.getPhysicalDevice(), transientCmdPool, device.getGraphicsQueueHandle(), presentationEngine.getImageCount(), &dynamicDescriptorPool, skybox.sampler, dim);

}

void Render::createBrdfLut()
{
	const VkFormat format = VK_FORMAT_R16G16_SFLOAT;	// R16G16 is supported pretty much everywhere
	const int32_t dim = 512;

	// Image
	VkImageCreateInfo imageCI = initializers::imageCreateInfo();
	imageCI.imageType = VK_IMAGE_TYPE_2D;
	imageCI.format = format;
	imageCI.extent.width = dim;
	imageCI.extent.height = dim;
	imageCI.extent.depth = 1;
	imageCI.mipLevels = 1;
	imageCI.arrayLayers = 1;
	imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	vkCreateImage(*device.getVulkanDevice(), &imageCI, nullptr, &specularBrdf.lutBRDF.image);
	VkMemoryAllocateInfo memAlloc = initializers::memoryAllocateInfo();
	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements(*device.getVulkanDevice(), specularBrdf.lutBRDF.image, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex = graphicsToolKit::findMemoryType(*device.getPhysicalDevice(), memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(*device.getVulkanDevice(), &memAlloc, nullptr, &specularBrdf.lutBRDF.imageMemory);
	vkBindImageMemory(*device.getVulkanDevice(), specularBrdf.lutBRDF.image, specularBrdf.lutBRDF.imageMemory, 0);
	// Image view
	VkImageViewCreateInfo viewCI = initializers::imageViewCreateInfo();
	viewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCI.format = format;
	viewCI.subresourceRange = {};
	viewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewCI.subresourceRange.levelCount = 1;
	viewCI.subresourceRange.layerCount = 1;
	viewCI.image = specularBrdf.lutBRDF.image;
	vkCreateImageView(*device.getVulkanDevice(), &viewCI, nullptr, &specularBrdf.lutBRDF.view);
	// Sampler
	VkSamplerCreateInfo samplerCI = initializers::samplerCreateInfo();
	samplerCI.magFilter = VK_FILTER_LINEAR;
	samplerCI.minFilter = VK_FILTER_LINEAR;
	samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.minLod = 0.0f;
	samplerCI.maxLod = 1.0f;
	samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	vkCreateSampler(*device.getVulkanDevice(), &samplerCI, nullptr, &specularBrdf.samplerLut);

	graphicsToolKit::generateBrdfLUT(specularBrdf.lutBRDF, device.getVulkanDevice(), device.getPhysicalDevice(), cmdPool, device.getGraphicsQueueHandle(), &dynamicDescriptorPool, dim);

}

void Render::createSkyEnviroment()
{
	createSkybox();
	createIrradianceMap();
	createpreFilteredEnvMap();
	createBrdfLut();
}

void Render::createSkybox() {

	
	VkSamplerCreateInfo samplerCI = initializers::samplerCreateInfo();
	samplerCI.magFilter = VK_FILTER_LINEAR;
	samplerCI.minFilter = VK_FILTER_LINEAR;
	samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCI.minLod = 0.0f;
	samplerCI.maxLod = 1;
	samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

	if (vkCreateSampler(*device.getVulkanDevice(), &samplerCI, nullptr, &skybox.sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}


   //(std::vector<std::string> equirectImages, Texture &cubeMap, Texture & irradianceMap, const VkDevice* device, const VkPhysicalDevice* physicalDevice, VkCommandPool commandPool, VkQueue transferQueue, uint32_t imageCount, int width, int height, VkDescriptorPool *pool) {
	graphicsToolKit::convertEquirectImageToCubeMap(hdrImage,skybox.cubeMap, device.getDevice(), device.getPhysicalDevice(), cmdPool, device.getGraphicsQueueHandle(), presentationEngine.getImageCount(), WIDTH, HEIGHT, &dynamicDescriptorPool);
	//graphicsToolKit::loadSkyboxTextures(device.getVulkanDevice(), device.getPhysicalDevice(), files, skybox.cubeMap, device.getGraphicsQueueHandle(), &cmdPool);
		
}

void Render::createPostProcessingRenderPasses() {

	VkExtent2D extent = presentationEngine.getExtent();
	VkExtent2D extent1 = presentationEngine.getExtent();
	extent1.width /= 4;
	extent1.height /= 4;

	VkFormat format = presentationEngine.getSwapChainFormat();
	uint32_t imageCount = presentationEngine.getImageCount();

	//Bloom pass
	VkAttachmentDescription verticalBlurColorAttachment = {};
	verticalBlurColorAttachment.format = presentationEngine.getSwapChainFormat();
	verticalBlurColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	verticalBlurColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	verticalBlurColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	verticalBlurColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	verticalBlurColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	verticalBlurColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	verticalBlurColorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = device.getSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference verticalBlurColor = {};
	verticalBlurColor.attachment = 0;
	verticalBlurColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	std::array<VkSubpassDescription, 1> subpass = {};
	subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass[0].colorAttachmentCount = 1;
	subpass[0].pColorAttachments = &verticalBlurColor;

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

	std::array<VkAttachmentDescription, 1> attachments = { verticalBlurColorAttachment };

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = static_cast<uint32_t>(subpass.size());
	renderPassInfo.pSubpasses = subpass.data();
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &verticalBlurRenderPass);
	vkTools::checkVkresult(result, "FTCVB RenderPass");
	if (result == VK_SUCCESS) {
		std::cout << "successfully created VERTICAL_BLUR_RENDERPASS  \n";
	}

	verticalBlurFrameBuffer.resize(imageCount);
	VkBool32 frameBufferCreation = VK_FALSE;

	for (size_t i = 0; i < imageCount; i++) {

		std::array<VkImageView, 1> attachments = { verticalBlurImage[i].view };

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = verticalBlurRenderPass;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		createInfo.width = extent1.width;
		createInfo.height = extent1.height;

		VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &verticalBlurFrameBuffer[i]);

		vkTools::checkVkresult(result, "FTC Main FrameBuffers\n");
		frameBufferCreation += result;
	}
	if (frameBufferCreation == 0)std::cout << "Successfully created VERTICA_BLUR_FBO " << "\n";

	//HorizontalBlur

	{

		VkAttachmentDescription finalImageAttachment = {};
		finalImageAttachment.format = presentationEngine.getSwapChainFormat();
		finalImageAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		finalImageAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		finalImageAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		finalImageAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		finalImageAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		finalImageAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		finalImageAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;



		VkAttachmentReference horizontalBlurColor = {};
		horizontalBlurColor.attachment = 0;
		horizontalBlurColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		std::array<VkSubpassDescription, 1> subpass = {};
		subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass[0].colorAttachmentCount = 1;
		subpass[0].pColorAttachments = &horizontalBlurColor;

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
		std::array<VkAttachmentDescription, 1> attachments = { finalImageAttachment };

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpass.size());
		renderPassInfo.pSubpasses = subpass.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &horizontalBlurRenderPass);
		vkTools::checkVkresult(result, "FTCVB RenderPass");
		if (result == VK_SUCCESS) {
			std::cout << "successfully created HORIZONTAL_BLUR_RENDERPASS  \n";
		}

		horizontalBlurFrameBuffer.resize(imageCount);
		VkBool32 frameBufferCreation = VK_FALSE;

		for (size_t i = 0; i < imageCount; i++) {

			std::array<VkImageView, 1> attachments = { presentationEngine.getImageViews()[i] };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = verticalBlurRenderPass;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.layers = 1;
			createInfo.width = WIDTH;
			createInfo.height = HEIGHT;

			VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &horizontalBlurFrameBuffer[i]);

			vkTools::checkVkresult(result, "FTC Main FrameBuffers\n");
			frameBufferCreation += result;
		}
		if (frameBufferCreation == 0)std::cout << "Successfully created HORIZONTAL_BLUR_FBO " << "\n";

	}

	//Ambient Oclusion Pass

	{
	VkFormat format = VK_FORMAT_R8_UNORM;


	aoPass.ssao.resize(presentationEngine.getImageCount());
	aoPass.gausianAttachments.resize(presentationEngine.getImageCount());

	for (unsigned int i = 0; i < presentationEngine.getImageCount(); ++i) {
		createAttachments(VK_FORMAT_R8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &aoPass.ssao[i], extent);
		createAttachments(VK_FORMAT_R8_UNORM, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &aoPass.gausianAttachments[i], extent);

	}


		VkAttachmentDescription ssaoAttachment = {};
		ssaoAttachment.format = format;
		ssaoAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		ssaoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		ssaoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		ssaoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		ssaoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		ssaoAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		ssaoAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


		VkAttachmentReference ssaoRef;
		ssaoRef.attachment = 0;
		ssaoRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		std::array<VkSubpassDescription, 1> subpass = {};
		subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass[0].colorAttachmentCount = 1;
		subpass[0].pColorAttachments = &ssaoRef;

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

		std::array<VkAttachmentDescription, 1> attachments = { ssaoAttachment };

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpass.size());
		renderPassInfo.pSubpasses = subpass.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &aoPass.renderpass);
		vkTools::checkVkresult(result, "FTCVB RenderPass");
		if (result == VK_SUCCESS) {
			std::cout << "successfully created SSAO_RENDERPASS  \n";
		}

		aoPass.frameBuffers.resize(imageCount);
		VkBool32 frameBufferCreation = VK_FALSE;

		for (size_t i = 0; i < imageCount; i++) {

			std::array<VkImageView, 1> attachments = { aoPass.ssao[i].view };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = aoPass.renderpass;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.layers = 1;
			createInfo.width = WIDTH;
			createInfo.height = HEIGHT;

			VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &aoPass.frameBuffers[i]);

			vkTools::checkVkresult(result, "FTC \n");
			frameBufferCreation += result;
		}
		if (frameBufferCreation == 0)std::cout << "Successfully created SSAO_FBO " << "\n";

	}

	//Gaussian SSAO
	{
		VkFormat format = VK_FORMAT_R8_UNORM;

		VkAttachmentDescription gausianAttachment = {};
		gausianAttachment.format = format;
		gausianAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		gausianAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		gausianAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		gausianAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		gausianAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		gausianAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		gausianAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


		VkAttachmentReference gausianRef;
		gausianRef.attachment = 0;
		gausianRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		std::array<VkSubpassDescription, 1> subpass = {};
		subpass[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass[0].colorAttachmentCount = 1;
		subpass[0].pColorAttachments = &gausianRef;

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

		std::array<VkAttachmentDescription, 1> attachments = { gausianAttachment };

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpass.size());
		renderPassInfo.pSubpasses = subpass.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &aoPass.gausianSSAORenderPass);
		vkTools::checkVkresult(result, "FTCVB RenderPass");
		if (result == VK_SUCCESS) {
			std::cout << "successfully created GAUSIAN_SSAO_RENDERPASS  \n";
		}

		aoPass.gausianFbs.resize(imageCount);
		aoPass.gausianAttachments.resize(imageCount);

		VkBool32 frameBufferCreation = VK_FALSE;

		for (size_t i = 0; i < imageCount; i++) {

			std::array<VkImageView, 1> attachments = { aoPass.gausianAttachments[i].view };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = aoPass.renderpass;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.layers = 1;
			createInfo.width = WIDTH;
			createInfo.height = HEIGHT;

			VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &aoPass.gausianFbs[i]);

			vkTools::checkVkresult(result, "FTC ao gaus FrameBuffers\n");
			frameBufferCreation += result;
		}
		if (frameBufferCreation == 0)std::cout << "Successfully created GAUSIAN_SSAO_FBO " << "\n";

	}

}

void Render::createShadowMapRenderPass()
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;


	if (vkCreateSampler(*device.getDevice(), &samplerInfo, nullptr, &shadowMapPass.sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}


	uint32_t numImages = presentationEngine.getImageCount();
	VkExtent2D extent{ 1024,1024 };



	VkFormat format = VK_FORMAT_R8G8B8A8_SNORM;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = depthMap.format;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;


	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 0;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 0;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	std::array<VkSubpassDependency, 2> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = 0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	std::array<VkAttachmentDescription, 1> attachments = { depthAttachment };

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &shadowMapPass.renderpass);
	vkTools::checkVkresult(result, "FTCVB SHADOW_RENDERPASS");
	if (result == VK_SUCCESS) {
		std::cout << "successfully created SHADOWMAP_RENDERPASS  \n";
	}

	uint32_t imageCount = presentationEngine.getImageCount();
	shadowMapPass.frameBuffers.resize(imageCount);
	VkBool32 frameBufferCreation = VK_FALSE;

	for (size_t i = 0; i < imageCount; i++) {

		std::array<VkImageView, 1> attachments = { depthMap.view };

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = shadowMapPass.renderpass;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		createInfo.width = 1024;
		createInfo.height = 1024;

		VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &shadowMapPass.frameBuffers[i]);

		vkTools::checkVkresult(result, "FTC shadowmap FrameBuffers\n");
		frameBufferCreation += result;
	}
	if (frameBufferCreation == 0)std::cout << "Successfully created SHADOW FBO " << "\n";

}

void Render::prepareImGuiResources()
{
	//Create RenderPass


	uint32_t imageCount = presentationEngine.getImageCount();
	VkExtent2D extent = presentationEngine.getExtent();

	VkAttachmentDescription attachment = {};
	attachment.format = presentationEngine.getSwapChainFormat();
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment = {};
	color_attachment.attachment = 0;
	color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &attachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;

	if (vkCreateRenderPass(*device.getVulkanDevice(), &info, nullptr, &guiResources.renderpass) != VK_SUCCESS) {
		throw std::runtime_error("Could not create Dear ImGui's render pass");
	}

	createAttachments(presentationEngine.getSwapChainFormat(), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &guiResources.imguiAtt, extent);
	guiResources.framebuffer.resize(imageCount);

	for (unsigned int i = 0; i < imageCount; i++) {
		VkImageView  fbAtt = { presentationEngine.getImageViews()[i] };

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = guiResources.renderpass;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &fbAtt;
		createInfo.layers = 1;
		createInfo.width = extent.width;
		createInfo.height = extent.height;

		VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &guiResources.framebuffer[i]);

		vkTools::checkVkresult(result, "FTC Main FrameBuffers\n");
	}
	guiResources.cmdPool.resize(1);
	guiResources.cmdBuffers.resize(imageCount);

	graphicsToolKit::createCommandPool(&guiResources.cmdPool[0], VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, device.getGraphicsQueueIndex(), *device.getVulkanDevice());

	for (unsigned int i = 0; i < imageCount; i++)
		graphicsToolKit::createCommandBuffers(&guiResources.cmdBuffers[i], imageCount, guiResources.cmdPool[0], *device.getVulkanDevice());

}

void  Render::renderUI(uint32_t imageIndex)
{

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		gameMode = 0;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //input mode
		glfwWaitEventsTimeout(1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		gameMode = 1;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwWaitEventsTimeout(1.0f);
	}
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VkResult result = vkBeginCommandBuffer(guiResources.cmdBuffers[imageIndex], &beginInfo);

	VkClearValue clearValues;

	clearValues.color = { .0f,1.0f, .0f, 1.f };

	VkViewport viewport = {};
	viewport.height = (float)HEIGHT;
	viewport.width = (float)WIDTH;

	viewport.maxDepth = 1.0f;

	VkRect2D rect = {};
	rect.extent.width = WIDTH;
	rect.extent.height = HEIGHT;
	rect.offset = { 0,0 };

	vkCmdSetViewport(guiResources.cmdBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(guiResources.cmdBuffers[imageIndex], 0, 1, &rect);

	VkRenderPassBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = guiResources.renderpass;
	info.framebuffer = guiResources.framebuffer[imageIndex];
	info.renderArea.extent.width = WIDTH;
	info.renderArea.extent.height = HEIGHT;
	info.clearValueCount = 0;

	vkCmdBeginRenderPass(guiResources.cmdBuffers[imageIndex], &info, VK_SUBPASS_CONTENTS_INLINE);

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	desingUi();

	//ImGui::ShowDemoWindow();

	ImGui::Render();

	

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), guiResources.cmdBuffers[imageIndex]);

	vkCmdEndRenderPass(guiResources.cmdBuffers[imageIndex]);
	if (vkEndCommandBuffer(guiResources.cmdBuffers[imageIndex]) != VK_SUCCESS) {
		std::cout << "Failed to record ImGui CmdBuffer for Index : " << imageIndex << std::endl;
	}


}

void Render::updateSky()
{
	//skybox.destroytextures(*device.getVulkanDevice());
	//specularBrdf.destroyFilteredEnv(*device.getVulkanDevice());
	createSkybox();
	createIrradianceMap();
	createpreFilteredEnvMap();
	rebuild();
	

}

void Render::updateLights(uint32_t currentImage)
{

	Entity *e = sceneGraph->root->childs[22]->entity.get();

	sceneLights.lights[0].lightPos = sceneGraph->root->childs[20]->entity->transform.getPosition();
	sceneLights.lights[1].lightPos = sceneGraph->root->childs[21]->entity->transform.getPosition();
	sceneLights.lights[2].lightPos = e->transform.getPosition();
	sceneLights.lights[3].lightPos = sceneGraph->root->childs[23]->entity->transform.getPosition();

	sceneLights.lights[0].lightColor = (materials[3]->pbrParameters.albedo) /10.f;
	sceneLights.lights[1].lightColor = (materials[4]->pbrParameters.albedo) /10.f;
	sceneLights.lights[2].lightColor = (materials[5]->pbrParameters.albedo) /10.f;
	sceneLights.lights[3].lightColor = (materials[6]->pbrParameters.albedo) /10.f;

	void* p;

	vkMapMemory(*device.getVulkanDevice(), lightUniforms[currentImage].bufferMemory, 0, sizeof(lightContainer), 0, &p);
	memcpy(p, &sceneLights, sizeof(lightContainer));
	vkUnmapMemory(*device.getVulkanDevice(), lightUniforms[currentImage].bufferMemory);


}

void Render::createGBufferRenderpass()
{

	
	uint32_t numImages = presentationEngine.getImageCount();
	VkExtent2D extent{ WIDTH,HEIGHT };


	deferedResources.albedo.resize(numImages);
	deferedResources.normal.resize(numImages);
	deferedResources.position.resize(numImages);
	deferedResources.material.resize(numImages);
	VkFormat formatP = VK_FORMAT_R32G32B32A32_SFLOAT;;

	VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
	VkFormat depthFormat =  device.getSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL, 
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);

	for (int i = 0; i < numImages; i++) {
		createAttachments(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &deferedResources.albedo[i], extent);
		createAttachments(format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &deferedResources.normal[i], extent);
		createAttachments(formatP, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &deferedResources.position[i], extent);
		createAttachments(format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &deferedResources.material[i], extent);

	}


	VkAttachmentDescription positionAttachment = {};
	positionAttachment.format = formatP;
	positionAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	positionAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	positionAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	positionAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	positionAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	positionAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	positionAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


	VkAttachmentDescription normalAttachment = {};
	normalAttachment.format = format;
	normalAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	normalAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	normalAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentDescription materialAttachment = {};
	materialAttachment.format = format;
	materialAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	materialAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	materialAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	materialAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	materialAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	materialAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	materialAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;


	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = depthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;


	VkAttachmentDescription diffuseAttachment = {};
	diffuseAttachment.format = VK_FORMAT_R16G16B16A16_SFLOAT;
	diffuseAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	diffuseAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	diffuseAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	diffuseAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	diffuseAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	diffuseAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	diffuseAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 0;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference positionRef = {};
	positionRef.attachment = 1;
	positionRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference normalRef = {};
	normalRef.attachment = 2;
	normalRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference diffuseRef = {};
	diffuseRef.attachment = 3;
	diffuseRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference materialRef = {};
	materialRef.attachment = 4;
	materialRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorRef[4] = { positionRef,normalRef,diffuseRef,materialRef};

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 4;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pColorAttachments = colorRef;

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

	std::array<VkAttachmentDescription, 5> attachments = { depthAttachment, positionAttachment,normalAttachment,diffuseAttachment,materialAttachment };

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &deferedResources.renderpass);
	vkTools::checkVkresult(result, "FTCVB aoGPass");
	if (result == VK_SUCCESS) {
		std::cout << "successfully created SSAO_RENDERPASS  \n";
	}

	uint32_t imageCount = presentationEngine.getImageCount();
	deferedResources.framebuffers.resize(imageCount);
	VkBool32 frameBufferCreation = VK_FALSE;

	for (size_t i = 0; i < imageCount; i++) {

		std::array<VkImageView, 5> attachments = { depthResource.view,deferedResources.position[i].view,deferedResources.normal[i].view,deferedResources.albedo[i].view,deferedResources.material[i].view };

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = deferedResources.renderpass;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		createInfo.width = WIDTH;
		createInfo.height = HEIGHT;
		
		VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &deferedResources.framebuffers[i]);

		vkTools::checkVkresult(result, "FTC aoGB FrameBuffers\n");
		frameBufferCreation += result;
	}
	if (frameBufferCreation == 0)std::cout << "Successfully created SSAO_FBO " << "\n";
}
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
void Render::desingUi()
{


	ImGui::Begin("Tugevus", NULL, 0);
	static float click1R = true;
	static int id = -1;
	ImGui::Spacing();
	static bool selectingObject = false;


	if (ImGui::CollapsingHeader("Screen Settings")) {
		float time = (float)glfwGetTime();

	
		static float i1 = sceneSettings.brightness;
		ImGui::SliderFloat("Brightness", &i1, .001f, 1.5f);
		sceneSettings.brightness = i1;

		static float i2 = sceneSettings.saturation;
		ImGui::SliderFloat("Saturation", &i2, .1f, 3.f);
		sceneSettings.saturation = i2;

		static float i3 = sceneSettings.contrast;
		ImGui::SliderFloat("Constrat", &i3, .01f, 5.f);
		sceneSettings.contrast = i3;

		static float i4 = sceneSettings.exposure;
		ImGui::SliderFloat("Exposure", &i4, .01f, 18.f);
		sceneSettings.exposure = i4;

		static float r = aoPass.parameters.radius;
		ImGui::SliderFloat("SSAO Radius", &r, .0f, 5.f);
		ImGui::SameLine();
		HelpMarker("Radius = 0 disable SSAO");
		aoPass.parameters.radius = r;

	}


	if (ImGui::CollapsingHeader("Materials")) {
		for (int i = 0; i < materials.size(); i++) {
			if (materials[i]->materialType != TUGEV::MaterialType::PBR)continue;
			if (ImGui::TreeNode(materials[i]->name)) {
				TUGEV::Material* mat = materials[i].get();
				ImGui::PushID(i);
				ImGui::Spacing();

				

				ImGui::InputFloat3("Albedo", glm::value_ptr(mat->pbrParameters.albedo));
				ImGui::SameLine();
				HelpMarker("This Albedo value will be added to the texture value");
				ImGui::InputFloat2("Texture Scale", glm::value_ptr(mat->pbrParameters.textureScaleFactor));
				ImGui::SameLine();
				HelpMarker("Scales Texture Coordinates");
				ImGui::SliderFloat("Metallicness", &mat->pbrParameters.metallicRoughness.x, .0f, 2.f);
				ImGui::SameLine();
				HelpMarker("This values is a multiplier of the original value in the texture");
				ImGui::SliderFloat("Roughness", &mat->pbrParameters.metallicRoughness.y, .0f, 2.f);
				ImGui::SameLine();
				HelpMarker("This values is a multiplier of the original value in the texture");
				ImGui::PopID();
				ImGui::TreePop();

			}
		}
	}

	if (ImGui::CollapsingHeader("Scene Objects")) {

		for (int i = 0; i < sceneGraph->root->childs.size(); i++) {



			Entity* item = sceneGraph->root->childs[i].get()->entity.get();
			if (item->getPipelineType() != DUMMY)
			if (ImGui::TreeNode(item->name)) {

				ImGui::PushID(i);

				glm::vec3 position = item->transform.getPosition();
				glm::vec3 scale = item->transform.getScale();
				glm::vec3 rotation = item->transform.getRotation();

				ImGui::Spacing();
				ImGui::InputFloat3("Position", (float*)glm::value_ptr(position));
				ImGui::InputFloat3("Scale", (float*)glm::value_ptr(scale));
				ImGui::InputFloat3("Rotation", (float*)glm::value_ptr(rotation));
			
				ImGui::Spacing();

				item->transform.setPosition(position);
				item->transform.setScale(scale);
				item->transform.setRotation(rotation);

		
				ImGui::PopID();
				ImGui::TreePop();

			}


		}

	}

	/*if (ImGui::CollapsingHeader("Lights")) {

		for (int i = 0; i < sceneLights.numberOfLights; i++) {
			ImGui::PushID(i);
			ImGui::Text("Light %d", i);
			glm::vec3 position = sceneLights.lights[i].lightPos;
			glm::vec3 color = sceneLights.lights[i].lightColor;


			ImGui::InputFloat3("Position", (float*)glm::value_ptr(position));
			ImGui::InputFloat3("Color", (float*)glm::value_ptr(color));
			ImGui::InputFloat("Type", &sceneLights.lights[i].type,1,.2);
			ImGui::InputFloat("Factor", &sceneLights.lights[i].factor, .1, .4);

			sceneLights.lights[i].lightPos = position;
			sceneLights.lights[i].lightColor = color;
				


			ImGui::PopID();

		}
	}*/

	if (ImGui::CollapsingHeader("Water")) {

		//ImGui::BulletText("Text baseline:");
		//ImGui::SameLine();
		//HelpMarker("This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. Lines only composed of text or \"small\" widgets fit in less vertical spaces than lines with normal widgets.");
		//ImGui::Indent();


		ImGui::InputFloat("Strengh", &waterResources.parameters.strengh);
		ImGui::SameLine();
		HelpMarker("This is how strong should be the perturbations  on the water");
		ImGui::InputFloat("Reflectiviy ", &waterResources.parameters.reflectivity);
		ImGui::SameLine();
		HelpMarker("This is how big and bright should the water highlights be");
		ImGui::InputFloat2("TextureCoordsScale", glm::value_ptr(waterResources.parameters.texCoordsScale));
		ImGui::SameLine();
		HelpMarker("Scales texture coordinates");


	}
	

	if (ImGui::CollapsingHeader(" Emissive Materials")) {
		for (int i = 0; i < materials.size(); i++) {

			if (materials[i]->materialType != TUGEV::MaterialType::COLOR || (materials[i]->name == "BOARD_MATERIAL" ) )continue;
			if (ImGui::TreeNode(materials[i]->name)) {
				TUGEV::Material* mat = materials[i].get();
				ImGui::PushID(i);
				ImGui::Spacing();
				ImGui::InputFloat3("Albedo", glm::value_ptr(mat->pbrParameters.albedo));

				ImGui::PopID();
				ImGui::TreePop();

			}
		}
	}

	if (ImGui::CollapsingHeader("Scene Lights")) {

		ImGui::BulletText("These orbs are point lights");
		ImGui::BulletText("You can change it's colors in Emissive Materials tab");

		for (int i = 0; i < sceneGraph->root->childs.size(); i++) {


			Entity* item = sceneGraph->root->childs[i].get()->entity.get();
			if (item->getPipelineType() != DUMMY || item->name == "board")continue;
			if (ImGui::TreeNode(item->name)) {
				
				ImGui::PushID(i);

				glm::vec3 position = item->transform.getPosition();
	
				ImGui::Spacing();
				ImGui::InputFloat3("Position", (float*)glm::value_ptr(position));
				ImGui::Spacing();
				
				item->transform.setPosition(position);
				//item->transform.setScale(scale);
				//item->transform.setRotation(rotation);


				ImGui::PopID();
				ImGui::TreePop();

			}


		}
		if(ImGui::TreeNode("Directional Light")){
			ImGui::BulletText("The only light that cast shadows");
			ImGui::BulletText("Type 0 = Directional Light");

			glm::vec3 position = sceneLights.lights[4].lightPos;
			glm::vec3 color = sceneLights.lights[4].lightColor;
			ImGui::Spacing();
	


			ImGui::InputFloat3("Position", (float*)glm::value_ptr(position));
			ImGui::InputFloat3("Color", (float*)glm::value_ptr(color));
			ImGui::InputFloat("Type", &sceneLights.lights[4].type, 1, .2);

			sceneLights.lights[4].lightPos = position;
			sceneLights.lights[4].lightColor = color;

			ImGui::TreePop();
		}


	}

	if (ImGui::CollapsingHeader("Diagnosis")) {

		ImGui::Text("The frame is rendered in 7 different renderpass as follows");
		ImGui::BulletText("Renderpass 1  - Creates the directional shadow Map (1024x1024)");
		ImGui::BulletText("Renderpass 2  - Fill a GBUFFER with ( WorldSpace Normals,Material Properties, Albedo)  (1024x1024)");
		ImGui::BulletText("Renderpass 3  - Creates the ScreenSpace Ambient Occlusion with 56 kernels  (1024x1024)");
		ImGui::BulletText("Renderpass 4  - Performs a gausian Blur on the previus on SSAO image with a 4x4 kernel (1024x1024)");
		ImGui::BulletText("Renderpass 5  - Performs light calculations , Tone Mapping and fill a lumen attachment which  will be used to show Bloom Effect (1024x1024)");
		ImGui::BulletText("Renderpass 6  - Performs  Vertical Gausian Blur on the lumen attachment form the previus pass (1024x1024)");
		ImGui::BulletText("Renderpass 7  - Performs  Horizontal Gausian Blur on the lumen attachment and add it's result to final image outputed at lighting stage (1024x1024)");

		static float bright = sceneSettings.brightness;

		ImGui::Text("Attachments Visualization");

		const char* items[] = { "FINAL_IMAGE", "BLURRED_SSAO", "NORMALS", "ALBEDO","POSITION","LUMEN" };
		static const char* item_current = items[0];            // Here our selection is a single pointer stored outside the object.
		if (ImGui::BeginCombo("Color Output", item_current, 0)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (item_current == items[n]);
				if (ImGui::Selectable(items[n], is_selected)) {
					item_current = items[n];
					if (item_current != items[0])sceneSettings.brightness = 0.5;
					else { sceneSettings.brightness = bright; };

					if (item_current == items[0])diagnosis.outPutFBO = 0;
					else if (item_current == items[1]) { diagnosis.outPutFBO = 1; sceneSettings.brightness = 0;}
					else if (item_current == items[2])diagnosis.outPutFBO = 2;
					else if (item_current == items[3])diagnosis.outPutFBO = 3;
					else if (item_current == items[4])diagnosis.outPutFBO = 4;
					else if (item_current == items[5])diagnosis.outPutFBO = 5;

				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
			}
			ImGui::EndCombo();
		}

	}



	//Figuring how to change resources in runTime time
	/*  
	if (ImGui::CollapsingHeader("Hdr Sky")) {

		static int selected = 0;
		std::array<std::string,2> skyboxes = { "Moonless.hdr", "Canyon.hdr" };
		for (int n = 0; n < skyboxes.size(); n++)
		{
			char buf[32];
			sprintf_s(buf, skyboxes[n].c_str(), n);
			if (ImGui::Selectable(buf,  selected == n))
				selected = n;
			//ImGui::InputText();
		}

		if (ImGui::Button("Update")) {
			hdrImage = "Aplication\\assets\\skybox\\" + skyboxes[selected];
			guiResources.change_sky_Event = 1;
		}
	}
	*/
	ImGui::End();



}

void Render::rebuild()
{
	const VkDevice deviceW = *device.getVulkanDevice();

	vkDeviceWaitIdle(deviceW);

	vkDestroySampler(deviceW, samplerBloom, nullptr);
	vkDestroySampler(deviceW, shadowMapSampler, nullptr);
	vkDestroySampler(deviceW, shadowMapPass.sampler, nullptr);

	for (size_t i = 0; i < MAX_FRAMES; i++) {
		vkDestroySemaphore(deviceW, syncObjcs.imageAvaible[i], nullptr);
		vkDestroySemaphore(deviceW, syncObjcs.renderFinished[i], nullptr);
		vkDestroyFence(deviceW, syncObjcs.fencesOn[i], nullptr);

	}

	
	vkDestroyRenderPass(deviceW, mainRenderPass, nullptr);
	vkDestroyRenderPass(deviceW, verticalBlurRenderPass, nullptr);
	vkDestroyRenderPass(deviceW, horizontalBlurRenderPass, nullptr);
	vkDestroyRenderPass(deviceW, shadowMapPass.renderpass, nullptr);


	deferedResources.destroy(&deviceW);
	waterResources.destroyUniformBuffers(deviceW);
	skybox.destroyBuffers(deviceW);
	aoPass.partialyDestroy(deviceW);

	for (auto& buffer : lightUniforms)buffer.destroy(deviceW);
	for (auto& buffer : toneMappingBuffer)buffer.destroy(deviceW);
	for (auto& buffer : sceneSettingsUniform)buffer.destroy(deviceW);
	for (auto& buffer : visualizationBuffer)buffer.destroy(deviceW);



	for (size_t i = 0; i < mainFrameBuffers.size(); i++) {
		vkDestroyFramebuffer(deviceW, mainFrameBuffers[i], nullptr);
		vkDestroyFramebuffer(deviceW, verticalBlurFrameBuffer[i], nullptr);
		vkDestroyFramebuffer(deviceW, horizontalBlurFrameBuffer[i], nullptr);
		vkDestroyFramebuffer(deviceW, shadowMapPass.frameBuffers[i], nullptr);

	}
	renderManager->destroyRenderManager(*device.getVulkanDevice());
		vkDestroyDescriptorSetLayout(*device.getVulkanDevice(), lightDescriptorSetLayout, nullptr);


	vkResetDescriptorPool(deviceW, dynamicDescriptorPool, 0);
	presentationEngine.cleanUp();

	vkFreeCommandBuffers(deviceW, cmdPool, static_cast<uint32_t>(cmdBuffers.size()), cmdBuffers.data());
	presentationEngine.preparePresentationEngine(device.getVulkanDevice(), device.getPhysicalDevice(), device.getSharingMode(), surface);
	createGBufferRenderpass();
	createDeferredShadingRenderpass();
	createPostProcessingRenderPasses();
	createShadowMapRenderPass();
	creategraphicsPipelines();
	createLightUniformBuffers();
	allocatePostProcessingBuffers();
	createMainFrameBuffers();
	allocateCommandBuffers();
	createDrawCommand();
	createSyncronizationObjects();
}


void Render::createmGuiInterface()
{
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);

	prepareImGuiResources();
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = *device.getPhysicalDevice();
	init_info.Device = *device.getVulkanDevice();
	init_info.QueueFamily = device.getGraphicsQueueIndex();
	init_info.Queue = device.getGraphicsQueueHandle();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = dynamicDescriptorPool;
	init_info.Allocator = VK_NULL_HANDLE;
	init_info.MinImageCount = presentationEngine.getImageCount();
	init_info.ImageCount = presentationEngine.getImageCount();
	init_info.CheckVkResultFn = nullptr;
	ImGui_ImplVulkan_Init(&init_info, guiResources.renderpass);

	VkCommandBuffer cmd = graphicsToolKit::beginSingleTimeCmdBuffer(*device.getVulkanDevice(), guiResources.cmdPool[0]);
	ImGui_ImplVulkan_CreateFontsTexture(cmd);
	graphicsToolKit::endSingleTimeCmdBuffer(cmd, *device.getVulkanDevice(), device.getGraphicsQueueHandle(), guiResources.cmdPool[0]);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = guiResources.cmdPool[0];
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(guiResources.cmdBuffers.size());

	VkResult result = vkAllocateCommandBuffers(*device.getVulkanDevice(), &allocInfo, guiResources.cmdBuffers.data());
	assert(_CrtCheckMemory());
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);

}

void Render::allocatePostProcessingBuffers() {

	graphicsToolKit::createSampler(device.getVulkanDevice(), samplerBloom);



	std::array<VkDescriptorSetLayout, 5> defferredLayouts;
	defferredLayouts[0] = renderManager->renderPipelines[2]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;
	defferredLayouts[1] = renderManager->renderPipelines[2]->pipelineDescriptorsLayouts->descriptorSetLayouts[1]->vulkanDescriptorSetLayout;
	defferredLayouts[2] = renderManager->renderPipelines[2]->pipelineDescriptorsLayouts->descriptorSetLayouts[2]->vulkanDescriptorSetLayout;
	defferredLayouts[3] = renderManager->renderPipelines[2]->pipelineDescriptorsLayouts->descriptorSetLayouts[3]->vulkanDescriptorSetLayout;


	VkDescriptorSetAllocateInfo allocInfo{};



	//depthNirradianceMapDescriptorSet
	{

		VkSamplerCreateInfo sampler = initializers::samplerCreateInfo();
		sampler.magFilter = VK_FILTER_LINEAR;
		sampler.minFilter = VK_FILTER_LINEAR;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		sampler.addressModeV = sampler.addressModeU;
		sampler.addressModeW = sampler.addressModeU;
		sampler.mipLodBias = 0.0f;
		sampler.maxAnisotropy = 1.0f;
		sampler.minLod = 0.0f;
		sampler.maxLod = 1.0f;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		vkCreateSampler(*device.getVulkanDevice(), &sampler, nullptr, &shadowMapSampler);

		VkDescriptorSetAllocateInfo allocInfo{};


		std::vector<VkDescriptorSetLayout> layouts(presentationEngine.getImageCount(), defferredLayouts[2]);
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		depthNirradianceMapDescriptorSet.resize(presentationEngine.getImageCount());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, depthNirradianceMapDescriptorSet.data());
		vkTools::checkVkresult(result, "FTC Allocate(DEPTHMAP)");

		for (int i = 0; i < depthNirradianceMapDescriptorSet.size(); i++) {
			std::vector<VkDescriptorImageInfo> descriptors = {
			initializers::descriptorImageInfo(shadowMapSampler,depthMap.view,VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL),
			initializers::descriptorImageInfo(skybox.sampler,skybox.irradianceMap.view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
		

			};

			std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
				initializers::writeDescriptorSet(depthNirradianceMapDescriptorSet[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &descriptors[0]),
				initializers::writeDescriptorSet(depthNirradianceMapDescriptorSet[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &descriptors[1]),
				
			};

			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	}

	//FilteredEnv and BrdfLut
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		std::vector<VkDescriptorSetLayout> layouts(presentationEngine.getImageCount(), defferredLayouts[3]);
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		specularBrdf.descriptorset.resize(presentationEngine.getImageCount());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, specularBrdf.descriptorset.data());
		vkTools::checkVkresult(result, "FTC Allocate(SpecularBRDF)");

		for (int i = 0; i < specularBrdf.descriptorset.size(); i++) {

			std::vector<VkDescriptorImageInfo> descriptors = {

			initializers::descriptorImageInfo(specularBrdf.sampler,specularBrdf.filteredEnvMap.view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
			initializers::descriptorImageInfo(specularBrdf.samplerLut,specularBrdf.lutBRDF.view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)


			};

			std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

				initializers::writeDescriptorSet(specularBrdf.descriptorset[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &descriptors[0]),
				initializers::writeDescriptorSet(specularBrdf.descriptorset[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &descriptors[1]),

			};

			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	} 

	//Ambient Oclusion
	{

		uint32_t imageCount = presentationEngine.getImageCount();


		//create aoUniformuffers

		aoPass.buffers.resize(presentationEngine.getImageCount());
		aoPass.parameterBuffer.resize(presentationEngine.getImageCount());


		VkDeviceSize size = sizeof(glm::vec3) * aoPass.uniforms.kernelSamples.size();
		VkDeviceSize s = sizeof(SSAO::Parameters);
		for (unsigned int i = 0; i < aoPass.buffers.size(); i++) {

			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, aoPass.buffers[i].buffer, aoPass.buffers[i].bufferMemory);
			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), s, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, aoPass.parameterBuffer[i].buffer, aoPass.parameterBuffer[i].bufferMemory);

		}


		//create the attachments for ssao
		VkExtent2D extent = presentationEngine.getExtent();

		aoPass.descriptorsets.resize(imageCount);

		VkDescriptorSetLayout& aoLayout = renderManager->renderPipelines[10]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;
	
		std::vector<VkDescriptorSetLayout> layouts(imageCount, aoLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, aoPass.descriptorsets.data());
		vkTools::checkVkresult(result, "FTC Allocate(AODEscriptorSEts)");


		for (unsigned int i = 0; i < aoPass.descriptorsets.size(); i++) {

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = aoPass.buffers[i].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(glm::vec3) * aoPass.uniforms.kernelSamples.size();

			VkDescriptorBufferInfo bufferInfo2{};
			bufferInfo2.buffer = aoPass.parameterBuffer[i].buffer;
			bufferInfo2.offset = 0;
			bufferInfo2.range = sizeof(SSAO::Parameters);



			VkDescriptorImageInfo normal = {};
			normal.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			normal.imageView = deferedResources.normal[i].view;
			normal.sampler = textSampler;


			VkDescriptorImageInfo position = {};
			position.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			position.imageView = deferedResources.position[i].view;
			position.sampler = textSampler;

			VkDescriptorImageInfo noiseTexture = {};
			noiseTexture.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			noiseTexture.imageView = aoPass.noiseTexture.view;
			noiseTexture.sampler = textSampler;



			std::vector<VkWriteDescriptorSet> writes;
			writes.push_back(initializers::writeDescriptorSet(aoPass.descriptorsets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &position));
			writes.push_back(initializers::writeDescriptorSet(aoPass.descriptorsets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &normal));
			writes.push_back(initializers::writeDescriptorSet(aoPass.descriptorsets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, &noiseTexture));
			writes.push_back(initializers::writeDescriptorSet(aoPass.descriptorsets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3, &bufferInfo));
			writes.push_back(initializers::writeDescriptorSet(aoPass.descriptorsets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4, &bufferInfo2));


			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

		}

	}

	//Gausian Blur on SSAO

	{
		uint32_t imageCount = presentationEngine.getImageCount();

		aoPass.gausianDescriptorsets.resize(imageCount);

		VkDescriptorSetLayout& gausianLayout = renderManager->renderPipelines[11]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;

		std::vector<VkDescriptorSetLayout> layouts(imageCount, gausianLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, aoPass.gausianDescriptorsets.data());
		vkTools::checkVkresult(result, "FTC Allocate(Gausian)");


		for (unsigned int i = 0; i < aoPass.gausianDescriptorsets.size(); i++) {

			VkDescriptorImageInfo ssao = {};
			ssao.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			ssao.imageView = aoPass.ssao[i].view;
			ssao.sampler = textSampler;


			std::vector<VkWriteDescriptorSet> writes;
			writes.push_back(initializers::writeDescriptorSet(aoPass.gausianDescriptorsets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &ssao));


			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

		}

	}

	//Tone-Mapping and Deferred Shading
	{

		//AKI 1
		toneMappingBuffer.resize(presentationEngine.getImageCount());
		visualizationBuffer.resize(presentationEngine.getImageCount());
		VkDeviceSize size = sizeof(toneMappingUbo);


		for (unsigned int i = 0; i < toneMappingBuffer.size(); i++) {
			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, toneMappingBuffer[i].buffer, toneMappingBuffer[i].bufferMemory);
			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), sizeof(diagnosis), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, visualizationBuffer[i].buffer, visualizationBuffer[i].bufferMemory);

		}

		std::vector<VkDescriptorSetLayout> layouts(presentationEngine.getImageCount(), defferredLayouts[0]);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		toneMapDescriptorSets.resize(presentationEngine.getImageCount());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, toneMapDescriptorSets.data());
		vkTools::checkVkresult(result, "FTC Allocate(LIGHT)");

		for (unsigned int i = 0; i < toneMapDescriptorSets.size(); i++) {

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = toneMappingBuffer[i].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(toneMappingUbo);

			VkDescriptorBufferInfo diagnosiBuffer{};
			diagnosiBuffer.buffer = visualizationBuffer[i].buffer;
			diagnosiBuffer.offset = 0;
			diagnosiBuffer.range = sizeof(Diagnosis);

			std::vector<VkDescriptorImageInfo> descriptors = {
				initializers::descriptorImageInfo(samplerBloom,deferedResources.normal[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
				initializers::descriptorImageInfo(samplerBloom,depthResource.view,VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL),
				initializers::descriptorImageInfo(samplerBloom,deferedResources.position[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
				initializers::descriptorImageInfo(samplerBloom,deferedResources.material[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
				initializers::descriptorImageInfo(samplerBloom,deferedResources.albedo[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
				initializers::descriptorImageInfo(samplerBloom,aoPass.gausianAttachments[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),

			};

			std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
				// Binding 0: Position input attachment
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &descriptors[0]),
				// Binding 1: Depth input attachment
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &descriptors[1]),
				// Binding 2: Display parameters uniform buffer
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4, &bufferInfo),
				//Normal
				//Specula
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, &descriptors[3]),
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3, &descriptors[4]),
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 5, &descriptors[5]),
				initializers::writeDescriptorSet(toneMapDescriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 6, &diagnosiBuffer),

			};
			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	}


	//bloom
	{

		//Vertical Blur
		VkDescriptorSetLayout &verticalBlurSetLayout = renderManager->renderPipelines[3]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;
		std::vector<VkDescriptorSetLayout> layouts(presentationEngine.getImageCount(), verticalBlurSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};

		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		verticalBlurDescriptorSet.resize(presentationEngine.getImageCount());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, verticalBlurDescriptorSet.data());
		vkTools::checkVkresult(result, "FTC Allocate(vertical)");

		for (unsigned int i = 0; i < verticalBlurDescriptorSet.size(); i++) {

			std::vector<VkDescriptorImageInfo> descriptors = {
				initializers::descriptorImageInfo(samplerBloom,lumenColorImage[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)

			};

			std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

				initializers::writeDescriptorSet(verticalBlurDescriptorSet[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &descriptors[0]),

			};
			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}

		//Horizontal 
		VkDescriptorSetLayout &horizontalBlurLayout = renderManager->renderPipelines[4]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;

		std::vector<VkDescriptorSetLayout> horizontalLayout(presentationEngine.getImageCount(), horizontalBlurLayout);
		VkDescriptorSetAllocateInfo allocInfo2{};

		allocInfo2 = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, horizontalLayout.data(), horizontalLayout.size());
		horizontalBlurDescriptorSet.resize(presentationEngine.getImageCount());

		result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo2, horizontalBlurDescriptorSet.data());

		vkTools::checkVkresult(result, "FTC Allocate(horizontal)");

		sceneSettingsUniform.resize(horizontalBlurDescriptorSet.size());

		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

		//AKI 2
		for (unsigned int i = 0; i < horizontalBlurDescriptorSet.size(); i++) {
			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), sizeof(SceneSettings), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, sceneSettingsUniform[i].buffer, sceneSettingsUniform[i].bufferMemory);
			
		}
		for (unsigned int i = 0; i < horizontalBlurDescriptorSet.size(); i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = sceneSettingsUniform[i].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(SceneSettings);

			std::vector<VkDescriptorImageInfo> descriptors = {

				initializers::descriptorImageInfo(samplerBloom,verticalBlurImage[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
				initializers::descriptorImageInfo(samplerBloom,colorImageAttachment[i].view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),

			};

			std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

				initializers::writeDescriptorSet(horizontalBlurDescriptorSet[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &descriptors[0]),
				initializers::writeDescriptorSet(horizontalBlurDescriptorSet[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,1, &descriptors[1]),
				initializers::writeDescriptorSet(horizontalBlurDescriptorSet[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,2, &bufferInfo),

			};
			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	}

	//Skybox
	{
		VkDescriptorSetLayout &skyboxLayout = renderManager->renderPipelines[5]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;
		std::vector<VkDescriptorSetLayout> layouts(presentationEngine.getImageCount(), skyboxLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		skybox.descriptorsets.resize(layouts.size());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, skybox.descriptorsets.data());
		vkTools::checkVkresult(result, "FTC Allocate(skybox)");

		skybox.uniformBuffers.resize(presentationEngine.getImageCount());
		skybox.skyUniform.resize(presentationEngine.getImageCount());

		//AKI 3 
		VkDeviceSize size = sizeof(UniformSkybox);
		for (unsigned int i = 0; i < skybox.uniformBuffers.size(); i++) {

			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, skybox.uniformBuffers[i].buffer, skybox.uniformBuffers[i].bufferMemory);
			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, skybox.skyUniform[i].buffer, skybox.skyUniform[i].bufferMemory);

		}

		for (unsigned int i = 0; i < skybox.descriptorsets.size(); i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = skybox.uniformBuffers[i].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformSkybox);


			std::vector<VkDescriptorImageInfo> descriptors = {

				initializers::descriptorImageInfo(skybox.sampler,skybox.irradianceMap.view,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)

			};

			std::vector<VkWriteDescriptorSet> writeDescriptorSets = {

				initializers::writeDescriptorSet(skybox.descriptorsets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,0, &bufferInfo),
				initializers::writeDescriptorSet(skybox.descriptorsets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &descriptors[0])

			};
			vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
		}
	}


	//Water
	{
	
		VkDeviceSize size = sizeof(waterResources.parameters);
		int imageCount = presentationEngine.getImageCount();
		waterResources.parametersBuffer.resize(imageCount);

		//AKI4 
		for (int i = 0; i < imageCount; i++) {
			graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, waterResources.parametersBuffer[i].buffer,waterResources.parametersBuffer[i].bufferMemory);
		}

		waterResources.descriptorSets.resize(imageCount);
		VkDescriptorSetLayout& waterLAyoutMVP = renderManager->renderPipelines[6]->pipelineDescriptorsLayouts->descriptorSetLayouts[1]->vulkanDescriptorSetLayout;
		std::vector<VkDescriptorSetLayout> layouts(imageCount, waterLAyoutMVP);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());
		VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, waterResources.descriptorSets.data());
		vkTools::checkVkresult(result, "FTC Allocate(WATER)");


		for (unsigned int i = 0; i < waterResources.descriptorSets.size(); i++) {

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = waterResources.parametersBuffer[i].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(waterResources.parameters);

			VkDescriptorImageInfo normal = {};
			normal.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			normal.imageView =waterResources.normal.view;
			normal.sampler =waterResources.normal.sampler;

			VkDescriptorImageInfo dudv = {};
			dudv.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			dudv.imageView = waterResources.dudv.view;
			dudv.sampler = waterResources.dudv.sampler;

			VkDescriptorImageInfo skyboxD = {};
			skyboxD.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			skyboxD.imageView = skybox.cubeMap.view;
			skyboxD.sampler = skybox.sampler;

			VkDescriptorImageInfo irradianceDesc = {};
			irradianceDesc.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			irradianceDesc.imageView = skybox.irradianceMap.view;
			irradianceDesc.sampler = skybox.sampler;

			VkDescriptorImageInfo refraction = {};
			refraction.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			refraction.imageView = materials[1]->textures[0].view;
			refraction.sampler = textSampler;
			
			VkDescriptorImageInfo depthMapD = {};
			depthMapD.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			depthMapD.imageView = depthMap.view;
			depthMapD.sampler = shadowMapSampler;


			std::vector<VkWriteDescriptorSet> writes;
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &normal));
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &dudv));
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, &skyboxD));
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3, &irradianceDesc));
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 5, &refraction));
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 4, &bufferInfo));	
				writes.push_back(initializers::writeDescriptorSet(waterResources.descriptorSets[i], VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 6, &depthMapD));

			
				vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
		
		}

	} 


	std::cout << "Allocated DescriptorSets successfully\n";
}

void Render::createWindow()
{
	//glfwInit();
	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//window = glfwCreateWindow(WIDTH, HEIGHT, appName, nullptr, nullptr);
	//
	//glfwMakeContextCurrent(window);
	//if (gameMode == 0)
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //input mode
	//else {
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //input mode
	//
	//}
	//glfwSetKeyCallback(window, keyCallBack);
	//glfwSetScrollCallback(window, scroll_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	//
	//
	//presentationEngine.setWindowUserPointer(window);
	//

}

void Render::createSurface()
{
	glfwCreateWindowSurface(instance, window, nullptr, &surface);
}

void Render::createInstance()
{
		
	if (enableValidationlayer && !debuger.checkLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}


	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Tugevus";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = debuger.getExtensions(enableValidationlayer);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationlayer) {
		//const std::vector<const char*>  layers = debuger.getValidationLayers();
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer.size());
		createInfo.ppEnabledLayerNames = validationLayer.data();
		debuger.populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);


	vkTools::checkVkresult(result, "Failed to create Instance");
}

void Render::createDeferredShadingRenderpass()
{

	uint32_t numImages = presentationEngine.getImageCount();
	VkExtent2D extent = presentationEngine.getExtent();

	//reflectionResources.reflection.resize(numImages);


	VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;;


	VkAttachmentDescription swapchainColorAttachment = {};
	swapchainColorAttachment.format = format;
	swapchainColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	swapchainColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	swapchainColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	swapchainColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	swapchainColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	swapchainColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	swapchainColorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	VkAttachmentDescription lumenAttachment = {};
	lumenAttachment.format = format;
	lumenAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	lumenAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	lumenAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	lumenAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	lumenAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	lumenAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	lumenAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;



	VkAttachmentReference swapChainColor = {};
	swapChainColor.attachment = 0;
	swapChainColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference lumenColor = {};
	lumenColor.attachment = 1;
	lumenColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


	VkAttachmentReference colors[2] = { swapChainColor,lumenColor };
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 2;
	subpass.pColorAttachments = colors;

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

	std::array<VkAttachmentDescription, 2> attachments = { swapchainColorAttachment,lumenAttachment };

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	VkResult result = vkCreateRenderPass(*device.getVulkanDevice(), &renderPassInfo, nullptr, &mainRenderPass);
	vkTools::checkVkresult(result, "FTCVB maiNRenderpass");
	if (result == VK_SUCCESS) {
		std::cout << "successfully created DEFERRED_RENDERPASS  \n";
	}


	//Reflection RenderPass same as regular renederpass
	//result = vkCreateRenderPass(*device.getDevice(), &renderPassInfo, nullptr, &reflectionResources.renderpass);
	//vkTools::checkVkresult(result, "FTC RenderPass");
	//if (result == VK_SUCCESS) {
	//	std::cout << "successfully created MainRenderPass  \n";
	//}

}

void Render::creategraphicsPipelines()
{
	renderManager = std::make_unique<TUGEV::RenderManager>();
	renderManager->renderPipelines.reserve(9);

	{
		std::vector<VkDescriptorSetLayoutBinding> bindingsForLight{
	initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_FRAGMENT_BIT,0)
		};

		lightDescriptorSets.resize(presentationEngine.getImageCount());

		VkDescriptorSetLayoutCreateInfo descriptorsetlayoutCI = initializers::descriptorSetLayoutCreateInfo(bindingsForLight);
		graphicsToolKit::createDescriptorSetLayout(device.getVulkanDevice(), &descriptorsetlayoutCI, &lightDescriptorSetLayout);
	}


	// GBUFFER   Pipeline Creation 
	{

		std::unique_ptr<TUGEV::RenderPipeline> gBufferPipeline(new TUGEV::RenderPipeline("GBUFFER", 0));


		// Index 0 at renderPipelines Vector
		//TUGEV::RenderPipeline pbrPipeline("PBR_PIPELINE", 0);
		std::unique_ptr<TUGEV::Descriptors> pbrPipelineDescriptorsLayouts(new TUGEV::Descriptors());

		//DescriptorSetLayou used for MPV uniform
		std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("MVP Uniform"));

		//Descriptor uses binding 0 for Model View projection;
		std::vector<VkDescriptorSetLayoutBinding> bindings{
				initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT,0)
		};
		//This descriptor uses 1 UNIFORM BUFFER;
		descriptorSetLayout->resourceQuantities.numBuffer++;

		//Build first descriptorSetLayout of pbrPipeline;
		descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
		//Layout added to pbrPipeline Descriptors
		pbrPipelineDescriptorsLayouts->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));


		//DescriptorSetLayout for Material data
		std::unique_ptr<TUGEV::DescriptorSetLayout  > materialDescriptorSetLayout(new TUGEV::DescriptorSetLayout("Material Data"));

		bindings.clear();
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 3));;	
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 4));;	
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 6));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 5));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 7));


		materialDescriptorSetLayout->resourceQuantities.numBuffer = 1;
		materialDescriptorSetLayout->resourceQuantities.numImageSampler =7;
		materialDescriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		pbrPipelineDescriptorsLayouts->descriptorSetLayouts.push_back(std::move(materialDescriptorSetLayout));

		//Vertex Atributes
		std::vector<Atributes> atributes = { VEC3,VEC3,VEC2 };


		const char* vertexPath = "Aplication\\Shaders\\gBuffer\\vert.spv";
		const char* fragmentPath = "Aplication\\Shaders\\gBuffer\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};

		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 4;
		pipelineInfo.cullMode = VK_CULL_MODE_NONE;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &deferedResources.renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 3 };
		pipelineInfo.alphablending = false;
		gBufferPipeline->pipelineDescriptorsLayouts = std::move(pbrPipelineDescriptorsLayouts);

		gBufferPipeline->buildPipelineLayout(device.getVulkanDevice());
		gBufferPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);


	//	//GBUFFER composition pipeline added to renderManager;
	//	gBufferPipeline->createChildPipeline(device.getDevice(),"instancing pipeline" ,pipelineInfo);
		renderManager->renderPipelines.push_back(std::move(gBufferPipeline));

	


	}

	//Shadow Map generation Pipeline
	{

		//Shadow Map pipeline at Index 1 of renderPipelines vector 
		std::unique_ptr<TUGEV::RenderPipeline> shadowMapPipeline(new TUGEV::RenderPipeline("ShadowMap Pipeline", 1));

		//Shadow Map Pipeline Descriptors
		std::unique_ptr<TUGEV::Descriptors> shadowmapDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayou used for MPV uniform
		std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("MVP Uniform ShadowMap"));

		//Descriptor uses binding 0 for Model View projection;
		std::vector<VkDescriptorSetLayoutBinding> bindings{
				initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT,0)
		};
		//This descriptor uses 1 UNIFORM BUFFER;
		descriptorSetLayout->resourceQuantities.numBuffer++;

		//Build first descriptorSetLayout of shadowMapPipeline;
		descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		//Layout added to pbrPipeline Descriptors
		shadowmapDescriptors->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));

		//DescriptorSetLayout for Material data




		//Vertex Atributes
		std::vector<Atributes> atributes = { VEC3,VEC3,VEC2 };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\shadow\\directional\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\shadow\\directional\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};

		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 0;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT	;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 1;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &shadowMapPass.renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 3 };

		shadowMapPipeline->pipelineDescriptorsLayouts = std::move(shadowmapDescriptors);

		shadowMapPipeline->buildPipelineLayout(device.getVulkanDevice());
		shadowMapPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		//PBR pipeline added to renderManager;
		renderManager->renderPipelines.push_back(std::move(shadowMapPipeline));

	}

	//Defered Shading Pass
	{
		//Deferred Shading pipeline at Index 2 of renderPipelines vector
		std::unique_ptr<TUGEV::RenderPipeline> deferredShadingPipeline(new TUGEV::RenderPipeline("Deferred Shading Pipeline", 2));

		//Shadow Map Pipeline Descriptors
		std::unique_ptr<TUGEV::Descriptors> deferredDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayou used for GBUFFER
		std::unique_ptr<TUGEV::DescriptorSetLayout > inputAttachmentLayout(new TUGEV::DescriptorSetLayout("Input GBuffer "));

		//Descriptor used for Input Attachment in subpass;
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 3));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 4));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 5));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 6));

		//This descriptor uses 1 UNIFORM BUFFER and 6 ;
		inputAttachmentLayout->resourceQuantities.numImageSampler = 5;
		inputAttachmentLayout->resourceQuantities.numBuffer = 1;

		//Build first descriptorSetLayout of pbrPipeline;
		inputAttachmentLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		//Layout added to pbrPipeline Descriptors
		deferredDescriptors->descriptorSetLayouts.push_back(std::move(inputAttachmentLayout));




		//Descriptor Used for light Buffer
		std::unique_ptr<TUGEV::DescriptorSetLayout > lightLayout(new TUGEV::DescriptorSetLayout(" Scene Lights "));

		std::vector<VkDescriptorSetLayoutBinding> bindingsForLight{
			initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_FRAGMENT_BIT  ,0)
		};


		lightLayout->resourceQuantities.numBuffer = 1;

		lightLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindingsForLight);


		//Descriptor Layout added
		deferredDescriptors->descriptorSetLayouts.push_back(std::move(lightLayout));



		//Descriptor Used for Irradiance Map, Reflective Shadow Map(To do) and Reflection(To do)
		std::unique_ptr<TUGEV::DescriptorSetLayout > irradianceLayout(new TUGEV::DescriptorSetLayout("Irradiance/RSM/Reflection La"));

		bindings.clear();
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));


		irradianceLayout->resourceQuantities.numImageSampler = 2;

		irradianceLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		deferredDescriptors->descriptorSetLayouts.push_back(std::move(irradianceLayout));



		//Descriptor used for Pre Computed Enviroment Specular Reflection 
		std::unique_ptr<TUGEV::DescriptorSetLayout > enviromentBRDF(new TUGEV::DescriptorSetLayout("Enviroment Specular Reflection"));

		bindings.clear();
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));

		enviromentBRDF->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		//Descriptor Layout added
		deferredDescriptors->descriptorSetLayouts.push_back(std::move(enviromentBRDF));



		//Vertex Atributes
		std::vector<Atributes> atributes{ NONE };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\toneMapping\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\toneMapping\\frag.spv";
		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};

		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 2;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &mainRenderPass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
		pipelineInfo.subpass = 0;
		pipelineInfo.vertexOffsets = { 0 };

		deferredShadingPipeline->pipelineDescriptorsLayouts = std::move(deferredDescriptors);




		//Build Pipeline Objects
		deferredShadingPipeline->buildPipelineLayout(device.getVulkanDevice());
		deferredShadingPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		//PBR pipeline added to renderManager;
		renderManager->renderPipelines.push_back(std::move(deferredShadingPipeline));

	}

	//Bloom Pipeline uses 2 passes for gaussian blur

	//Vertical Blur Pass

	{
		{
			// Index 3  at renderPipelines Vector
			std::unique_ptr<TUGEV::RenderPipeline> verticalBlur(new TUGEV::RenderPipeline("VerticalBlurfor bloom", 3));

			std::unique_ptr<TUGEV::Descriptors> verticalBlurDescriptors(new TUGEV::Descriptors());

			//DescriptorSetLayou used for MPV uniform
			std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("Lumen Image VB"));

			//Descriptor uses binding 0 for Lumen Image where it will perform Blurring
			std::vector<VkDescriptorSetLayoutBinding> bindings{
				initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0)
			};
			//This descriptor uses 1 COMBINED IMAGE SAMPLER;
			descriptorSetLayout->resourceQuantities.numImageSampler = 1;

//Build DescritporSetLayout
descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
//Layout added to VerticalBlur Descriptors
verticalBlurDescriptors->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));


//Vertex Atributes
std::vector<Atributes> atributes{ NONE };

const char* vertexPath;
const char* fragmentPath;

vertexPath = "Aplication\\Shaders\\bloom\\vertical\\vert.spv";
fragmentPath = "Aplication\\Shaders\\bloom\\vertical\\frag.spv";

//Pipeline States Data
graphicsToolKit::PipelineInfo pipelineInfo{};

pipelineInfo.atributes = atributes;
pipelineInfo.colorAttCount = 1;
pipelineInfo.cullMode = VK_CULL_MODE_NONE;
pipelineInfo.dephTest = 0;
pipelineInfo.depthBias = 0;
pipelineInfo.fragmentShaderPath = fragmentPath;
pipelineInfo.vertexShaderPath = vertexPath;
pipelineInfo.rdpass = &verticalBlurRenderPass;
pipelineInfo.subpass = 0;
pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
pipelineInfo.vertexOffsets = { 0 };

verticalBlur->pipelineDescriptorsLayouts = std::move(verticalBlurDescriptors);

verticalBlur->buildPipelineLayout(device.getVulkanDevice());
verticalBlur->buildPipeline(device.getVulkanDevice(), pipelineInfo);

//Vertical Blur pipeline added to renderManager;
renderManager->renderPipelines.push_back(std::move(verticalBlur));
		}
		{
		// Index 4  at renderPipelines Vector
		std::unique_ptr<TUGEV::RenderPipeline> horizontalBlur(new TUGEV::RenderPipeline("HorizontalBlur bloom", 4));

		std::unique_ptr<TUGEV::Descriptors> horizontalBlurDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayou used for MPV uniform
		std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("Lumen Image HorizontalBlur"));

		//Descriptor uses binding 0 for Lumen Image where it will perform Blurring, 1 for Scene Rendered 2 for Constrast/Saturation/Brightness Buffer

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 2));

		//This descriptor uses 1 COMBINED IMAGE SAMPLER;
		descriptorSetLayout->resourceQuantities.numImageSampler = 2;
		descriptorSetLayout->resourceQuantities.numBuffer = 1;

		//Build DescritporSetLayout
		descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
		//Layout added to VerticalBlur Descriptors
		horizontalBlurDescriptors->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));


		//Vertex Atributes
		std::vector<Atributes> atributes{ NONE };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\bloom\\horizontal\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\bloom\\horizontal\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};

		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &horizontalBlurRenderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 0 };

		horizontalBlur->pipelineDescriptorsLayouts = std::move(horizontalBlurDescriptors);

		horizontalBlur->buildPipelineLayout(device.getVulkanDevice());
		horizontalBlur->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		//Horizontal Blur pipeline added to renderManager;
		renderManager->renderPipelines.push_back(std::move(horizontalBlur));
		}
	}


	{
		std::unique_ptr<TUGEV::RenderPipeline> skyboxPipeline(new TUGEV::RenderPipeline("SkyBox Pipeline", 5));

		std::unique_ptr<TUGEV::Descriptors> skyboxDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayou used for MPV uniform
		std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("Lumen Image VB"));

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.reserve(2);
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));

		descriptorSetLayout->resourceQuantities.numImageSampler = 1;
		descriptorSetLayout->resourceQuantities.numBuffer = 1;

		//Build DescritporSetLayout
		descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
		skyboxDescriptors->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));


		//Vertex Atributes
		std::vector<Atributes> atributes{ VEC3 };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\cubemap\\skybox\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\cubemap\\skybox\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};

		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 4;
		pipelineInfo.cullMode = VK_CULL_MODE_NONE;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &deferedResources.renderpass;
		pipelineInfo.subpass = 0;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 1 };

		skyboxPipeline->pipelineDescriptorsLayouts = std::move(skyboxDescriptors);

		skyboxPipeline->buildPipelineLayout(device.getVulkanDevice());
		skyboxPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		renderManager->renderPipelines.push_back(std::move(skyboxPipeline));

	}
		
	

	//Water Displacement  Pipeline
	std::unique_ptr<TUGEV::RenderPipeline> waterDisplacementPipelien(new TUGEV::RenderPipeline("Water Pipeline", 6));

	std::unique_ptr<TUGEV::Descriptors> waterDescriptors(new TUGEV::Descriptors());
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	//DescriptorSetLayou used for MPV uniform
	std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("MPV"));

	//Descriptor uses binding 0 for MVP Matrices
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0));

	//This descriptor uses 1 COMBINED IMAGE SAMPLER;
	descriptorSetLayout->resourceQuantities.numBuffer = 1;

	//Build DescritporSetLayout
	descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
	//Layout added to VerticalBlur Descriptors
	waterDescriptors->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));





	//Layout for water textures
	std::unique_ptr<TUGEV::DescriptorSetLayout > waterDescriptorSetLayout(new TUGEV::DescriptorSetLayout(" Water Fragment "));

	bindings.clear();
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 3));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 5));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 4));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 6));

	//This descriptor uses 1 COMBINED IMAGE SAMPLER;
	waterDescriptorSetLayout->resourceQuantities.numImageSampler = 4;

	//Build DescritporSetLayout
	waterDescriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
	//Layout added to VerticalBlur Descriptors
	waterDescriptors->descriptorSetLayouts.push_back(std::move(waterDescriptorSetLayout));



	//Layout for Light Buffer

	//DescriptorSetLayou used for MPV uniform
	std::unique_ptr<TUGEV::DescriptorSetLayout > lightLayout(new TUGEV::DescriptorSetLayout("Light Uniform"));

	//Descriptor uses binding 0 for MVP Matrices
	bindings.clear();
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT , 0));

	//This descriptor uses 1 COMBINED IMAGE SAMPLER;
	lightLayout->resourceQuantities.numBuffer = 1;

	//Build DescritporSetLayout
	lightLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
	//Layout added to VerticalBlur Descriptors
	waterDescriptors->descriptorSetLayouts.push_back(std::move(lightLayout));


	//Vertex Atributes
	std::vector<Atributes> atributes{ VEC3,VEC3 ,VEC2 };

	const char* vertexPath;
	const char* fragmentPath;

	vertexPath = "Aplication\\Shaders\\displacementWater\\vert.spv";
	fragmentPath = "Aplication\\Shaders\\displacementWater\\frag.spv";

	//Pipeline States Data
	graphicsToolKit::PipelineInfo pipelineInfo{};

	pipelineInfo.atributes = atributes;
	pipelineInfo.colorAttCount = 4;
	pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	pipelineInfo.dephTest = 1;
	pipelineInfo.depthBias = 0;
	pipelineInfo.fragmentShaderPath = fragmentPath;
	pipelineInfo.vertexShaderPath = vertexPath;
	pipelineInfo.rdpass = &deferedResources.renderpass;
	pipelineInfo.subpass = 0;
	pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
	pipelineInfo.vertexOffsets = { 3 };

	waterDisplacementPipelien->pipelineDescriptorsLayouts = std::move(waterDescriptors);

	waterDisplacementPipelien->buildPipelineLayout(device.getVulkanDevice());
	waterDisplacementPipelien->buildPipeline(device.getVulkanDevice(), pipelineInfo);

	//Water pipeline added to renderManager;
	renderManager->renderPipelines.push_back(std::move(waterDisplacementPipelien));








	//Pipeline to output just a color or an albedo texture, without any fancy calculations
	std::unique_ptr<TUGEV::RenderPipeline> dummyPipeline(new TUGEV::RenderPipeline("Dummy", 7));

	std::unique_ptr<TUGEV::Descriptors> dummyDescriptors(new TUGEV::Descriptors());

	//DescriptorSetLayou used for MPV uniform
	std::unique_ptr<TUGEV::DescriptorSetLayout > dummydescriptorSetLayout(new TUGEV::DescriptorSetLayout("MVP"));


	bindings.clear();

	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0));

	//This descriptor uses 1 COMBINED IMAGE SAMPLER;
	dummydescriptorSetLayout->resourceQuantities.numBuffer = 1;

	//Build DescritporSetLayout
	dummydescriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
	//Layout added to VerticalBlur Descriptors
	dummyDescriptors->descriptorSetLayouts.push_back(std::move(dummydescriptorSetLayout));

	//Second Layout
	std::unique_ptr<TUGEV::DescriptorSetLayout > dummyColorLayout(new TUGEV::DescriptorSetLayout("Color"));
	bindings.clear();
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));

	dummyColorLayout->resourceQuantities.numBuffer = 1;
	dummyColorLayout->resourceQuantities.numImageSampler = 1;

	dummyColorLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
	dummyDescriptors->descriptorSetLayouts.push_back(std::move(dummyColorLayout));


	//Vertex Atributes // Position and TexCoords
	std::vector<Atributes> atributesDummy{ VEC3,VEC3,VEC2 };


	vertexPath = "Aplication\\Shaders\\dummyPipeline\\vert.spv";
	fragmentPath = "Aplication\\Shaders\\dummyPipeline\\frag.spv";

	//Pipeline States Data


	pipelineInfo.atributes = atributesDummy;
	pipelineInfo.colorAttCount = 4;
	pipelineInfo.cullMode = VK_CULL_MODE_NONE;
	pipelineInfo.dephTest = 1;
	pipelineInfo.depthBias = 0;
	pipelineInfo.fragmentShaderPath = fragmentPath;
	pipelineInfo.vertexShaderPath = vertexPath;
	pipelineInfo.rdpass = &deferedResources.renderpass;
	pipelineInfo.subpass = 0;
	pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
	pipelineInfo.vertexOffsets = { 3 };
	pipelineInfo.vertexBindingCount = 1;
	dummyPipeline->pipelineDescriptorsLayouts = std::move(dummyDescriptors);

	dummyPipeline->buildPipelineLayout(device.getVulkanDevice());
	dummyPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

	//Vertical Blur pipeline added to renderManager;
	renderManager->renderPipelines.push_back(std::move(dummyPipeline));
	




	//Particle System Pipeline
	// Index 8  at renderPipelines Vector
	std::unique_ptr<TUGEV::RenderPipeline> particlePipeline(new TUGEV::RenderPipeline("particlePipeline bloom", 8));

	std::unique_ptr<TUGEV::Descriptors> particleDescriptors(new TUGEV::Descriptors());

	//DescriptorSetLayou used for MPV uniform
	std::unique_ptr<TUGEV::DescriptorSetLayout > particleDescriptorSetLayout(new TUGEV::DescriptorSetLayout("MVP For particles"));

	//Descriptor uses binding 0 for Lumen Image where it will perform Blurring, 1 for Scene Rendered 2 for Constrast/Saturation/Brightness Buffer

	bindings.clear();
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0));


	particleDescriptorSetLayout->resourceQuantities.numBuffer = 1;

	//Build DescritporSetLayout
	particleDescriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);
	//Layout added to VerticalBlur Descriptors
	particleDescriptors->descriptorSetLayouts.push_back(std::move(particleDescriptorSetLayout));

	std::unique_ptr<TUGEV::DescriptorSetLayout > colorLayout (new TUGEV::DescriptorSetLayout("Texture Color particles"));
	bindings.clear();
	bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ,VK_SHADER_STAGE_FRAGMENT_BIT, 0));
	colorLayout->resourceQuantities.numImageSampler = 1;
	colorLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

	particleDescriptors->descriptorSetLayouts.push_back(std::move(colorLayout));

	//Vertex Atributes
	std::vector<Atributes> atributesParticles{ VEC3,VEC3,VEC3 };


	vertexPath = "Aplication\\Shaders\\particles\\teste\\vert.spv";
	fragmentPath = "Aplication\\Shaders\\particles\\teste\\frag.spv";

	//Pipeline States Data

	pipelineInfo.atributes = atributesParticles;
	pipelineInfo.colorAttCount = 4;
	pipelineInfo.cullMode = VK_CULL_MODE_NONE;
	pipelineInfo.dephTest = 1;
	pipelineInfo.depthBias = 0;
	pipelineInfo.fragmentShaderPath = fragmentPath;
	pipelineInfo.vertexShaderPath = vertexPath;
	pipelineInfo.rdpass = &deferedResources.renderpass;
	pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	pipelineInfo.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	pipelineInfo.vertexOffsets = { 3 };
	pipelineInfo.vertexBindingCount = 1;
	pipelineInfo.alphablending = true;

	particlePipeline->pipelineDescriptorsLayouts = std::move(particleDescriptors);

	particlePipeline->buildPipelineLayout(device.getVulkanDevice());
	particlePipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

	renderManager->renderPipelines.push_back(std::move(particlePipeline));



	{

		//Shadow Map pipeline at Index 8 of renderPipelines vector 
		std::unique_ptr<TUGEV::RenderPipeline> shadowMapPipeline(new TUGEV::RenderPipeline("ShadowMap Pipeline", 9));

		//Shadow Map Pipeline Descriptors
		std::unique_ptr<TUGEV::Descriptors> shadowmapDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayou used for MPV uniform
		std::unique_ptr<TUGEV::DescriptorSetLayout > descriptorSetLayout(new TUGEV::DescriptorSetLayout("MVP Uniform ShadowMap"));

		std::vector<VkDescriptorSetLayoutBinding> bindings{
				initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT,0)
		};
		//This descriptor uses 1 UNIFORM BUFFER;
		descriptorSetLayout->resourceQuantities.numBuffer++;

		//Build first descriptorSetLayout of shadowMapPipeline;
		descriptorSetLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		//Layout added to pbrPipeline Descriptors
		shadowmapDescriptors->descriptorSetLayouts.push_back(std::move(descriptorSetLayout));

		//DescriptorSetLayout for Material data
		std::unique_ptr<TUGEV::DescriptorSetLayout  > reflectShadowMapBuffers(new TUGEV::DescriptorSetLayout("Material Data"));

		bindings.clear();
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));



		reflectShadowMapBuffers->resourceQuantities.numImageSampler = 1;
		reflectShadowMapBuffers->resourceQuantities.numBuffer = 1;

		reflectShadowMapBuffers->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		shadowmapDescriptors->descriptorSetLayouts.push_back(std::move(reflectShadowMapBuffers));


		//Vertex Atributes
		std::vector<Atributes> atributes = { VEC3,VEC3,VEC2 };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\shadow\\directional\\color\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\shadow\\directional\\color\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};

		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 0;
		pipelineInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		pipelineInfo.dephTest = 1;
		pipelineInfo.depthBias = 1;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &shadowMapPass.renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		pipelineInfo.vertexOffsets = { 3 };

		shadowMapPipeline->pipelineDescriptorsLayouts = std::move(shadowmapDescriptors);

		shadowMapPipeline->buildPipelineLayout(device.getVulkanDevice());
		shadowMapPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		//PBR pipeline added to renderManager;
		renderManager->renderPipelines.push_back(std::move(shadowMapPipeline));

	}


	{

		//aog pipeline at Index 10 of renderPipelines vector 
		std::unique_ptr<TUGEV::RenderPipeline> aoGPassPipeline(new TUGEV::RenderPipeline("AoGB Pipeline", 10));

		//Shadow Map Pipeline Descriptors
		std::unique_ptr<TUGEV::Descriptors> aoGDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayout for Material data
		std::unique_ptr<TUGEV::DescriptorSetLayout  > aoDescriptorLayout(new TUGEV::DescriptorSetLayout("gBuffer"));

		bindings.clear();
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 3));
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 4));




		aoDescriptorLayout->resourceQuantities.numImageSampler = 3;
		aoDescriptorLayout->resourceQuantities.numBuffer = 1;

		aoDescriptorLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		aoGDescriptors->descriptorSetLayouts.push_back(std::move(aoDescriptorLayout));


		//Vertex Atributes
		std::vector<Atributes> atributes{ NONE };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\aoG\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\aoG\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};


		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 0;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &aoPass.renderpass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
		pipelineInfo.subpass = 0;
		pipelineInfo.vertexOffsets = { 0 };


		aoGPassPipeline->pipelineDescriptorsLayouts = std::move(aoGDescriptors);

		aoGPassPipeline->buildPipelineLayout(device.getVulkanDevice());
		aoGPassPipeline->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		//PBR pipeline added to renderManager;
		renderManager->renderPipelines.push_back(std::move(aoGPassPipeline));


	}


	{
		//gausian Blur SSAO pipeline at Index 11 of renderPipelines vector 
		std::unique_ptr<TUGEV::RenderPipeline> gausianBlur(new TUGEV::RenderPipeline("Gausian Pipeline", 11));

		//Shadow Map Pipeline Descriptors
		std::unique_ptr<TUGEV::Descriptors> gausianDescriptors(new TUGEV::Descriptors());

		//DescriptorSetLayout for Material data
		std::unique_ptr<TUGEV::DescriptorSetLayout  > gausianLayout(new TUGEV::DescriptorSetLayout("gaussian"));

		bindings.clear();
		bindings.push_back(initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0));


		gausianLayout->resourceQuantities.numImageSampler = 1;

		gausianLayout->buildDescriptorSetLayout(device.getVulkanDevice(), bindings);

		gausianDescriptors->descriptorSetLayouts.push_back(std::move(gausianLayout));


		//Vertex Atributes
		std::vector<Atributes> atributes{ NONE };

		const char* vertexPath;
		const char* fragmentPath;

		vertexPath = "Aplication\\Shaders\\aoG\\gausianBlur\\vert.spv";
		fragmentPath = "Aplication\\Shaders\\aoG\\gausianBlur\\frag.spv";

		//Pipeline States Data
		graphicsToolKit::PipelineInfo pipelineInfo{};


		pipelineInfo.atributes = atributes;
		pipelineInfo.colorAttCount = 1;
		pipelineInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		pipelineInfo.dephTest = 0;
		pipelineInfo.depthBias = 0;
		pipelineInfo.fragmentShaderPath = fragmentPath;
		pipelineInfo.vertexShaderPath = vertexPath;
		pipelineInfo.rdpass = &aoPass.gausianSSAORenderPass;
		pipelineInfo.frontFaceClock = VK_FRONT_FACE_CLOCKWISE;
		pipelineInfo.subpass = 0;
		pipelineInfo.vertexOffsets = { 0 };


		gausianBlur->pipelineDescriptorsLayouts = std::move(gausianDescriptors);

		gausianBlur->buildPipelineLayout(device.getVulkanDevice());
		gausianBlur->buildPipeline(device.getVulkanDevice(), pipelineInfo);

		renderManager->renderPipelines.push_back(std::move(gausianBlur));

	}
	//endp
	std::cout << "Created GRAPHICS_PIPELINES\n";

}

void Render::createDepthAttachment()
{
	VkFormat format = device.getSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);

	//Depth in relation to the camera
	graphicsToolKit::createImage(*device.getPhysicalDevice(), *device.getVulkanDevice(), WIDTH, HEIGHT, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthResource.image, depthResource.memory, 0, 1);
	graphicsToolKit::createImageView(*device.getVulkanDevice(), depthResource.view, depthResource.image, format, VK_IMAGE_ASPECT_DEPTH_BIT);

	//depth in lightSpace for shadow Map
	depthMap.format = format; int d = 1024;;
	graphicsToolKit::createImage(*device.getPhysicalDevice(), *device.getVulkanDevice(), d, d, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT  | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthMap.image, depthMap.memory, 0, 1);
	graphicsToolKit::createImageView(*device.getVulkanDevice(), depthMap.view, depthMap.image, format, VK_IMAGE_ASPECT_DEPTH_BIT);

}

void Render::createAttachments()
{
	VkExtent2D extent = presentationEngine.getExtent();
	VkExtent2D extent1 = presentationEngine.getExtent();
	extent1.width /= 4;
	extent1.height /= 4;

	frameBufferAttachments.clear();
	lumenColorImage.clear();
	colorImageAttachment.clear();
	uint32_t numImages = presentationEngine.getImageCount();

	frameBufferAttachments.resize(numImages);
	lumenColorImage.resize(numImages);
	colorImageAttachment.resize(numImages);
	//reflectionResources.reflection.resize(numImages);
	deferedResources.position.resize(numImages);
	deferedResources.normal.resize(numImages);
	deferedResources.material.resize(numImages);

	verticalBlurImage.resize(numImages);
	const uint32_t numMips = static_cast<uint32_t>(floor(log2(std::max(extent.width, extent.height)))) + 1;
	VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
	for (unsigned int i = 0; i < presentationEngine.getImageCount(); i++) {
		createAttachments(format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &colorImageAttachment[i], extent);
		createAttachments(format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, &frameBufferAttachments[i], extent);
		createAttachments(format, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &lumenColorImage[i], extent);
		createAttachments(presentationEngine.getSwapChainFormat(), VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, &verticalBlurImage[i], extent1);


	}

	graphicsToolKit::createCube(&skybox.vertexBuffer, device.getPhysicalDevice(), device.getVulkanDevice(), device.getTransferQueueHandle(), transferPool);


	//Creation of water Textures
	std::string texturePath = "Aplication\\assets\\materials\\water\\normal.png";
	
	format = VK_FORMAT_R8G8B8A8_UNORM;
	//Normal Map
	graphicsToolKit::createTextureImage(texturePath.c_str(), device.getPhysicalDevice(), device.getVulkanDevice(), format, VK_IMAGE_TILING_OPTIMAL, waterResources.normal.image, waterResources.normal.imageMemory, 0, 1, *device.getTransferQueueHandle(),device.getGraphicsQueueHandle() ,cmdPool,transferPool ,waterResources.normal.numMips);
	graphicsToolKit::createImageView(*device.getVulkanDevice(), waterResources.normal.view, waterResources.normal.image, format, VK_IMAGE_ASPECT_COLOR_BIT,VK_IMAGE_VIEW_TYPE_2D,1,waterResources.normal.numMips);
	waterResources.normal.createSampler(*device.getVulkanDevice());
	//DUDV
	texturePath = "Aplication\\assets\\materials\\water\\waterDUDV.png";
	graphicsToolKit::createTextureImage(texturePath.c_str(), device.getPhysicalDevice(), device.getVulkanDevice(), format, VK_IMAGE_TILING_OPTIMAL, waterResources.dudv.image, waterResources.dudv.imageMemory, 0, 1, *device.getTransferQueueHandle(), device.getGraphicsQueueHandle(), cmdPool, transferPool,waterResources.dudv.numMips);
	graphicsToolKit::createImageView(*device.getVulkanDevice(), waterResources.dudv.view, waterResources.dudv.image, format, VK_IMAGE_ASPECT_COLOR_BIT,VK_IMAGE_VIEW_TYPE_2D,1,waterResources.dudv.numMips);
	waterResources.dudv.createSampler(*device.getDevice());
}

void Render::createAmbientOclusionResources()
{
	aoPass.initializeSamples();

	//Create Noise Texture for AoSamples
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	int texWidth =4;
	VkDeviceSize imageSize = texWidth * texWidth * 4 * sizeof(float);
	//VkDeviceSize imageSize = sizeof(glm::vec4)*aoPass.noise.size();
	VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;

	graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


	void* data;
	vkMapMemory(*device.getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, aoPass.noise.data() , static_cast<size_t>(imageSize));
	vkUnmapMemory(*device.getDevice(), stagingBufferMemory);


	graphicsToolKit::createImage(*device.getPhysicalDevice(), *device.getVulkanDevice(), texWidth, texWidth, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, aoPass.noiseTexture.image, aoPass.noiseTexture.imageMemory, 0, 1);
	graphicsToolKit::setImageLayout(*device.getTransferQueueHandle(), *device.getVulkanDevice(), transferPool, aoPass.noiseTexture.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 0);

	graphicsToolKit::copyBufferToImage(stagingBuffer, aoPass.noiseTexture.image, *device.getVulkanDevice(), transferPool, texWidth, texWidth, 0, 1, *device.getTransferQueueHandle());
	graphicsToolKit::setImageLayout(*device.getTransferQueueHandle(), *device.getVulkanDevice(), transferPool, aoPass.noiseTexture.image , format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, 0);
	vkDestroyBuffer(*device.getVulkanDevice(), stagingBuffer, nullptr);
	vkFreeMemory(*device.getVulkanDevice(), stagingBufferMemory, nullptr);
	graphicsToolKit::createImageView(*device.getDevice(), aoPass.noiseTexture.view, aoPass.noiseTexture.image, format, VK_IMAGE_ASPECT_COLOR_BIT);



}

void Render::createMainFrameBuffers()
{
	VkExtent2D extent = presentationEngine.getExtent();


	size_t imageCount = presentationEngine.getImageViews().size();

	mainFrameBuffers.resize(imageCount);

	VkBool32 frameBufferCreation = VK_FALSE;

	for (size_t i = 0; i < imageCount; i++) {
		std::array<VkImageView, 2> attachments = { colorImageAttachment[i].view,lumenColorImage[i].view };

		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = mainRenderPass;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		createInfo.width = WIDTH;
		createInfo.height = HEIGHT;

		VkResult result = vkCreateFramebuffer(*device.getVulkanDevice(), &createInfo, nullptr, &mainFrameBuffers[i]);

		vkTools::checkVkresult(result, "FTC Main FrameBuffers\n");
		frameBufferCreation += result;
	}
	if (frameBufferCreation == 0)std::cout << "Successfully created MainFrameBuffers " << "\n";

	//reflection FrameBuffers
	/*reflectionResources.frameBuffers.resize(imageCount);

	for (size_t i = 0; i < imageCount; i++) {
		std::array<VkImageView, 8> attachments = { reflectionResources.reflection[i].view, deferedResources.position[i].view,depthResource.view,lumenColorImage[i].view,deferedResources.normal[i].view,deferedResources.specular[i].view,deferedResources.albedo[i].view,deferedResources.emission[i].view };
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = reflectionResources.renderpass;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.layers = 1;
		createInfo.width = WIDTH;
		createInfo.height = HEIGHT;
		VkResult result = vkCreateFramebuffer(*device.getDevice(), &createInfo, nullptr, &reflectionResources.frameBuffers[i]);
		vkTools::checkVkresult(result, "FTC Main FrameBuffers\n");
		frameBufferCreation += result;
	}
	if (frameBufferCreation == 0)std::cout << "Successfully created ReflectionFrameBuffers " << "\n";
	*/
}

void Render::createCommandPool()
{
	//Graphics CmdPool
	VkCommandPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
	createInfo.flags = 0;

	VkResult result = vkCreateCommandPool(*device.getVulkanDevice(), &createInfo, nullptr, &cmdPool);

	vkTools::checkVkresult(result, "FTC CommandPool\n");


	//TransferPool CmdPool
	
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = device.getTransferQueueIndex();
	createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

	result = vkCreateCommandPool(*device.getVulkanDevice(), &createInfo, nullptr, &transferPool);

	vkTools::checkVkresult(result, "FTC TranferCommandPool\n");


	if (result == VK_SUCCESS)std::cout << "Successfully created CommandPool\n";

}

void Render::allocateCommandBuffers()
{
	cmdBuffers.resize(presentationEngine.getImageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = cmdPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(cmdBuffers.size());

	VkResult result = vkAllocateCommandBuffers(*device.getVulkanDevice(), &allocInfo, cmdBuffers.data());

	vkTools::checkVkresult(result, "FTA CommandBuffers\n");

	if (result == 0)std::cout << "Sucessfully allocated CommandBuffers\n";

}

void Render::createDrawCommand()
{

	for (int i = 0; i < cmdBuffers.size(); i++) {
		float depthBiasConstant = 1.25f;
		// Slope depth bias factor, applied depending on polygon's slope
		float depthBiasSlope = 1.75f;
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		VkResult result = vkBeginCommandBuffer(cmdBuffers[i], &beginInfo);
		vkTools::checkVkresult(result, "Failed to Begin cmdBuffer\n");



		//ShadowMap pass
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = shadowMapPass.renderpass;
			renderPassInfo.framebuffer = shadowMapPass.frameBuffers[i];
			renderPassInfo.renderArea.offset = { 0,0 };
			VkExtent2D extent{};
			extent.width = 1024;
			extent.height = 1024;
			renderPassInfo.renderArea.extent = extent;

			std::array<VkClearValue, 5> clearValues = {};
			clearValues[0].depthStencil = { 1.f };
			clearValues[1].color = { .3,.3,.3,.0 };
			clearValues[2].color = { .3,.3,.3,.0 };
			clearValues[3].color = { .3,.3,.3,.0 };
			clearValues[4].color = { .3,.3,.3,.0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();
			//
			//vkCmdSetDepthBias(
			//	cmdBuffers[i],
			//	depthBiasConstant,
			//	0.0f,
			//	depthBiasSlope);
			//
			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = 1024;
			viewport.width = 1024;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = 1024;
			rect.extent.height = 1024;
			rect.offset = { 0,0 };
			vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
			VkDeviceSize offsets[1] = { 0 };


			for (int j = 1; j < sceneGraph->root->childs.size(); j++) {

				Entity* node = sceneGraph->root->childs[j].get()->entity.get();
				Model* m = dynamic_cast<Model*>(node);
				PipelineType p = node->getPipelineType();
				VkPipeline pipeline;
				VkPipelineLayout layout;

				if (p == NONE)continue;
					pipeline = renderManager->renderPipelines[1]->pipeline;
					layout = renderManager->renderPipelines[1]->pipelinelayout;

				

				m->renderShadowMap(cmdBuffers[i], pipeline, layout, i, *m->materialset);
			}

			vkCmdEndRenderPass(cmdBuffers[i]);

		}

		//GBUFFER PASS

		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = deferedResources.renderpass;
			renderPassInfo.framebuffer = deferedResources.framebuffers[i];
			renderPassInfo.renderArea.offset = { 0,0 };
			VkExtent2D extent{};
			extent.width = WIDTH; 
			extent.height = HEIGHT;
			renderPassInfo.renderArea.extent = extent;


			std::array<VkClearValue, 5> clearValues = {};
			clearValues[0].depthStencil = { 1.f };
			clearValues[1].color = { .0,.0,.0,.0 };
			clearValues[2].color = { .0,.0,.0,.0 };
			clearValues[3].color = { .0,.0,.0,.0 };
			clearValues[4].color = { .0,.0,.0,.0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

		

			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = WIDTH;
			viewport.width = HEIGHT;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = WIDTH;
			rect.extent.height = HEIGHT;
			rect.offset = { 0,0 };
			vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
			VkDeviceSize offsets[1] = { 0 };


			std::vector<VkDescriptorSet>sets{};
			sets.resize(2);
			sets[0] = waterResources.descriptorSets[i];
			sets[1] = lightDescriptorSets[i];

			{
				VkDeviceSize offsets[1] = { 0 };
				vkCmdBindVertexBuffers(cmdBuffers[i], 0, 1, &skybox.vertexBuffer.buffer, offsets);
				vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[5]->pipeline);
				vkCmdBindDescriptorSets(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[5]->pipelinelayout, 0, 1, &skybox.descriptorsets[i], 0, NULL);
				vkCmdDraw(cmdBuffers[i], 36, 1, 0, 0);
			}
			
			for (int j = 0; j < sceneGraph->root->childs.size(); j++) {

				Entity* node = sceneGraph->root->childs[j].get()->entity.get();
				std::vector<VkDescriptorSet> s;
				s.push_back(waterResources.descriptorSets[i]);
				s.push_back(lightDescriptorSets[i]);

				std::vector<VkDescriptorSet>sets{};
				sets.resize(2);
				sets[0] = waterResources.descriptorSets[i];
				sets[1] = lightDescriptorSets[i];

				switch (node->getPipelineType()) {
				case NONE:
					break;
				case PBR_COOK_TORRANCE:
					node->draw(cmdBuffers[i], renderManager->renderPipelines[0].get()->pipeline, renderManager->renderPipelines[0].get()->pipelinelayout, i, sets);
					break;

				case WATER_DISPLACEMENT:

					node->draw(cmdBuffers[i], renderManager->renderPipelines[6].get()->pipeline, renderManager->renderPipelines[6].get()->pipelinelayout, i, sets);
					break;

				case DUMMY:
					node->draw(cmdBuffers[i], renderManager->renderPipelines[7].get()->pipeline, renderManager->renderPipelines[7].get()->pipelinelayout, i, sets);
					break;


				}


			}

			vkCmdEndRenderPass(cmdBuffers[i]);

		}
		

		//AMBIENT OCLUSION PASS

		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = aoPass.renderpass;
			renderPassInfo.framebuffer = aoPass.frameBuffers[i];
			renderPassInfo.renderArea.offset = { 0,0 };

			VkExtent2D extent{};
			extent.width = WIDTH;
			extent.height = HEIGHT;
			renderPassInfo.renderArea.extent = extent;

			std::array<VkClearValue, 1> clearValues = {};
			clearValues[0].color = { 1.0,1.0,1.0,1.0 };


			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();


			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = WIDTH;
			viewport.width = HEIGHT;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = WIDTH;
			rect.extent.height = HEIGHT;
			rect.offset = { 0,0 };
			vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
			VkDeviceSize offsets[1] = { 0 };

			std::array<VkDescriptorSet, 1 >sets = { aoPass.descriptorsets[i] };
			vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[10]->pipeline);
			vkCmdBindDescriptorSets(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[10]->pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
			vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);
		
			vkCmdEndRenderPass(cmdBuffers[i]);
		}


		//GAUSIAN BLUR ON SSAO PASS

		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = aoPass.gausianSSAORenderPass;
			renderPassInfo.framebuffer = aoPass.gausianFbs[i];
			renderPassInfo.renderArea.offset = { 0,0 };

			VkExtent2D extent{};
			extent.width = WIDTH;
			extent.height = HEIGHT;
			renderPassInfo.renderArea.extent = extent;

			std::array<VkClearValue, 1> clearValues = {};
			clearValues[0].color = { 1.0,1.0,1.0,1.0 };


			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();


			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = WIDTH;
			viewport.width = HEIGHT;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = WIDTH;
			rect.extent.height = HEIGHT;
			rect.offset = { 0,0 };
			vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
			VkDeviceSize offsets[1] = { 0 };

			std::array<VkDescriptorSet, 1 >sets = { aoPass.gausianDescriptorsets[i] };
			vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[11]->pipeline);
			vkCmdBindDescriptorSets(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[11]->pipelinelayout, 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
			vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(cmdBuffers[i]);
		}

	//DEFFERED SHADING PASS
		{
		
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mainRenderPass;
		renderPassInfo.framebuffer = mainFrameBuffers[i];
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = presentationEngine.getExtent();

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { .3f,0.3f, .3f, 1.f };
		clearValues[1].color = { .3f, .3f, .3f, 1.f };
	

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = initializers::viewport(WIDTH, HEIGHT, 0.0f, 1.0f);

		viewport.maxDepth = 1.0f;

		VkRect2D rect = {};
		rect.extent.width = WIDTH;
		rect.extent.height = HEIGHT;
		rect.offset = { 0,0 };

		vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
		vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
		VkDeviceSize offsets[1] = { 0 };

		//Light Calculation and ToneMapping
		

			vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[2]->pipeline);
			VkDescriptorSet descriptorSets[4] = { toneMapDescriptorSets[i] ,lightDescriptorSets[i],depthNirradianceMapDescriptorSet[i],specularBrdf.descriptorset[i]};
			vkCmdBindDescriptorSets(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[2]->pipelinelayout, 0, 4, descriptorSets, 0, NULL);
			vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(cmdBuffers[i]);

		}

		//VERTICAL BLUR PASS
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = verticalBlurRenderPass;
			renderPassInfo.framebuffer = verticalBlurFrameBuffer[i];
			renderPassInfo.renderArea.offset = { 0,0 };
			VkExtent2D extent1 = presentationEngine.getExtent();
			extent1.width /= 4;
			extent1.height /= 4;

			renderPassInfo.renderArea.extent.width = extent1.width;
			renderPassInfo.renderArea.extent.height = extent1.height;
			std::array<VkClearValue, 2> clearValues = {};
			clearValues[0].color = { 1.0f, .0f, .0f, 1.f };
			clearValues[1].color = { 1.0f, .0f, .01f, 1.f };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = extent1.width;
			viewport.width = extent1.width;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = extent1.width;
			rect.extent.height = extent1.width;
			rect.offset = { 0,0 };

			vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
			vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[3]->pipeline);
			vkCmdBindDescriptorSets(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[3]->pipelinelayout, 0, 1, &verticalBlurDescriptorSet[i], 0, NULL);

			vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(cmdBuffers[i]);

		}

		//HorizontalBlurPass
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = horizontalBlurRenderPass;
			renderPassInfo.framebuffer = horizontalBlurFrameBuffer[i];
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = presentationEngine.getExtent();

			std::array<VkClearValue, 1> clearValues = {};
			clearValues[0].color = { .3f, .3f, 0.3f, 1.f };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(cmdBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport = {};
			viewport.height = HEIGHT;
			viewport.width = WIDTH;

			viewport.maxDepth = 1.0f;

			VkRect2D rect = {};
			rect.extent.width = WIDTH;
			rect.extent.height = HEIGHT;
			rect.offset = { 0,0 };

			vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
			vkCmdSetScissor(cmdBuffers[i], 0, 1, &rect);
			vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[4]->pipeline);
			vkCmdBindDescriptorSets(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderManager->renderPipelines[4]->pipelinelayout, 0, 1, &horizontalBlurDescriptorSet[i], 0, NULL);

			vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(cmdBuffers[i]);

		}

		if (vkEndCommandBuffer(cmdBuffers[i]) != VK_SUCCESS) {
			std::cout << "Failed to record CommandBuffer\n";
		}

	}

}

void Render::createLightUniformBuffers() {

	lightUniforms.resize(presentationEngine.getImageCount());
	VkDeviceSize size = sizeof(lightContainer);

	for (unsigned int i = 0; i < lightUniforms.size(); i++) {
		graphicsToolKit::createBuffer(*device.getPhysicalDevice(), device.getVulkanDevice(), size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, lightUniforms[i].buffer, lightUniforms[i].bufferMemory);
	}

	std::vector<VkDescriptorSetLayout> layouts(presentationEngine.getImageCount(), lightDescriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo = initializers::descriptorSetAllocateInfo(dynamicDescriptorPool, layouts.data(), layouts.size());

	VkResult result = vkAllocateDescriptorSets(*device.getVulkanDevice(), &allocInfo, lightDescriptorSets.data());
	vkTools::checkVkresult(result, "FTC Allocate(LIGHT)");

	std::vector<VkWriteDescriptorSet> writes;
	writes.resize(presentationEngine.getImageCount());

	for (unsigned int i = 0; i < lightDescriptorSets.size(); i++) {

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = lightUniforms[i].buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(lightContainer);

		std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
			initializers::writeDescriptorSet(lightDescriptorSets[i], VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,0, &bufferInfo),
		};
		vkUpdateDescriptorSets(*device.getVulkanDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
	}

}

void Render::initiateLightContainer()
{


	lightUbo light2{};
	light2.lightColor = glm::vec3(5,5,4);
	light2.lightPos = glm::vec3(8, 4, -5.0);
	light2.type = 1;
	light2.factor = 1;

	lightUbo light3{};
	light3.lightColor = glm::vec3(4,.5,.5);
	light3.lightPos = glm::vec3(-7, 4, 9.0);
	light3.type = 1;
	light3.factor = 1;
	
	lightUbo light4{};
	light4.lightColor = glm::vec3(.5, 4, 1);
	light4.lightPos = glm::vec3(-11, 3, -8.0);
	light4.type = 1;
	light4.factor = 1;

	lightUbo light5{};
	light5.lightColor = glm::vec3(2, .5, 2);
	light5.lightPos = glm::vec3(9, 2, 11);
	light5.type = 1;
	light5.factor = 1;

	lightUbo ll{};
	ll.lightColor = glm::vec3(1, 1,1);
	ll.lightPos = glm::vec3(900, 1400, 300);
	ll.type = 0;
	ll.factor = 1;

	sceneLights.lights[0] = light2;
	sceneLights.lights[1] = light3;
	sceneLights.lights[2] = light4;
	sceneLights.lights[3] = light5;
	sceneLights.lights[4] = ll;

	//sceneLights.lights[4] = light5;
	//sceneLights.lights[5] = light6;
	//sceneLights.lights[6] = light7;
	//sceneLights.lights[7] = light8;



	sceneLights.numberOfLights = 5;
}


void Render::updateMaterialUniforms(int currentImage)
{
	for (int i = 0; i < materials.size(); i++) {

		void* p;
			vkMapMemory(*device.getVulkanDevice(), materials[i]->uniformBuffers[currentImage].bufferMemory, 0, sizeof(TUGEV::PbrParameters), 0, &p);
			memcpy(p, &materials[i]->pbrParameters, sizeof(TUGEV::PbrParameters));
			vkUnmapMemory(*device.getVulkanDevice(), materials[i]->uniformBuffers[currentImage].bufferMemory);
		
	}

}

void Render::createSyncronizationObjects()
{
	syncObjcs.fencesOn.clear();
	syncObjcs.imagesOn.clear();
	syncObjcs.imageAvaible.clear();
	syncObjcs.renderFinished.clear();


	syncObjcs.renderFinished.resize(MAX_FRAMES);
	syncObjcs.imageAvaible.resize(MAX_FRAMES);
	syncObjcs.fencesOn.resize(MAX_FRAMES);
	syncObjcs.imagesOn.resize(presentationEngine.getImageCount());
	VkSemaphoreCreateInfo semaphInfo{};
	semaphInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES; i++) {
		if (vkCreateSemaphore(*device.getVulkanDevice(), &semaphInfo, nullptr, &syncObjcs.imageAvaible[i]) != VK_SUCCESS ||
			vkCreateSemaphore(*device.getVulkanDevice(), &semaphInfo, nullptr, &syncObjcs.renderFinished[i]) != VK_SUCCESS ||
			vkCreateFence(*device.getVulkanDevice(), &fenceInfo, nullptr, &syncObjcs.fencesOn[i])

			)
		{

			std::cout << "FTC synObjcs\n";
		}
	}
}

void Render::recreateSwapChain()
{
	std::cout << "SwapChain Recreation ... \n";
	int w, h = 0;
	glfwGetFramebufferSize(window, &w, &h);
	while (w == 0 || h == 0) {
		glfwGetFramebufferSize(window, &w, &h);

		glfwWaitEvents();
	}

	rebuild();
	guiResources.destroy(device.getVulkanDevice(), device.getGraphicsQueueHandle(), &instance, window);
	createmGuiInterface();
}

void Render::createTransientCommandPool()
{
	VkCommandPoolCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
	createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

	VkResult result = vkCreateCommandPool(*device.getVulkanDevice(), &createInfo, nullptr, &transientCmdPool);

	vkTools::checkVkresult(result, "FTC CommandPool\n");

	if (result == VK_SUCCESS)std::cout << "Successfully created Transient CommandPool\n";
}

void Render::createDescriptorPool() {

	//std::vector<VkDescriptorPoolSize> sizes;
	//sizes.resize(3);
	//
	//sizes[0].descriptorCount = presentationEngine.getImageCount()+52;
	//sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//sizes[1].descriptorCount = presentationEngine.getImageCount() +39 ;
	//sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//sizes[2].descriptorCount = presentationEngine.getImageCount() * 6 ;
	//sizes[2].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	std::vector<VkDescriptorPoolSize> pool_sizes =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },

		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 20 }
	};

	//This pool will allocate static material data
	graphicsToolKit::createDescriptorPool(device.getVulkanDevice(), pool_sizes, descriptorPool);

	pool_sizes.clear();
	pool_sizes =
	{
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 84 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 40 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 78 }
	};
	//this pool will allocate data tha't could change 
	graphicsToolKit::createDescriptorPool(device.getVulkanDevice(), pool_sizes, dynamicDescriptorPool);

}

void Render::loadSceneData()
{
	DeviceWraper wraper;
	wraper.computePool = &cmdPool;
	wraper.computeQueue = &device.getGraphicsQueueHandle();
	wraper.device = device.getDevice();
	wraper.graphicsPool = &cmdPool;
	wraper.numImages = presentationEngine.getImageCount();
	wraper.physicalDevice = device.getPhysicalDevice();
	wraper.transferPool = &transferPool;
	wraper.transferQueue = device.getTransferQueueHandle();
	wraper.descriptorPool = &descriptorPool;
	wraper.graphicsQueue = &device.getGraphicsQueueHandle();

	VkDescriptorSetLayout* mpvLayout = &renderManager->renderPipelines[0]->pipelineDescriptorsLayouts->descriptorSetLayouts[0]->vulkanDescriptorSetLayout;
	Model* m = static_cast<Model*>(sceneGraph->root->childs[1]->entity.get());

	//load meshes data
	for (int i = 0; i < sceneGraph->root->childs.size(); i++) {

		Entity* e = sceneGraph->root->childs[i]->entity.get();
		e->initiateResources(presentationEngine.getImageCount(), wraper, *mpvLayout);
		e->start();
	}
	//Load materials 

	graphicsToolKit::createSampler(device.getDevice(), textSampler);

	for (int i = 0; i <materials.size(); i++) {
			VkDescriptorSetLayout layout;
			materials[i].get()->device = wraper.device;

		switch (materials[i].get()->materialType) {
		case TUGEV::MaterialType::PBR :
			layout = renderManager->renderPipelines[0]->pipelineDescriptorsLayouts->descriptorSetLayouts[1]->vulkanDescriptorSetLayout;
			materials[i]->loadResource(&wraper, presentationEngine.getImageCount(), layout, textSampler);
			
			break;

		case TUGEV::MaterialType::COLOR:
			layout = renderManager->renderPipelines[7]->pipelineDescriptorsLayouts->descriptorSetLayouts[1]->vulkanDescriptorSetLayout;
			materials[i]->loadResource(&wraper, presentationEngine.getImageCount(), layout, textSampler);
			break;
			
		}
	}

}

void Render::renderCycle()
{ 
	initiateResources();
}

void Render::renderLoop()
{
	//while (!glfwWindowShouldClose(window)) {
	//	glfwPollEvents();
	//
	//	draw();
	//}
	//vkDeviceWaitIdle(*device.getVulkanDevice());

}

void Render::draw()
{

//	syncObjcs.fencesOn[currentFrame];

	vkWaitForFences(*device.getVulkanDevice(), 1, &syncObjcs.fencesOn[currentFrame], VK_FALSE, UINT64_MAX);
	uint32_t imageIndex;

	float time = (float)glfwGetTime();
	TUGEV::TimeStep timeStep = time - lastFrameTIme;
	frameTime = timeStep.getTimeinMilli();
	lastFrameTIme = time;

	VkResult result = vkAcquireNextImageKHR(*device.getVulkanDevice(), presentationEngine.getSwapChain(), UINT64_MAX, syncObjcs.imageAvaible[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		std::cout << "ERROR_OUT_OF_DATE\n";
		recreateSwapChain();
		return;
	}

	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		std::cout << "ERROR_SUBOPTIMAL\n";

		throw std::runtime_error("failed to acquire swap chain image!");
	}
	update(imageIndex,frameTime);


	if (syncObjcs.imagesOn[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(*device.getVulkanDevice(), 1, &syncObjcs.imagesOn[imageIndex], VK_TRUE, UINT64_MAX);
	}


	syncObjcs.imagesOn[imageIndex] = syncObjcs.fencesOn[currentFrame];

	VkSubmitInfo submitInfo = {};
	renderUI(imageIndex);
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncObjcs.imageAvaible[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	std::array<VkCommandBuffer, 2> cmds;
	cmds = { cmdBuffers[imageIndex] ,guiResources.cmdBuffers[imageIndex] };
	submitInfo.commandBufferCount = static_cast<uint32_t>(cmds.size());
	submitInfo.pCommandBuffers = cmds.data();

	VkSemaphore signalSemaphores[] = { syncObjcs.renderFinished[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(*device.getVulkanDevice(), 1, &syncObjcs.fencesOn[currentFrame]);

	VkResult r = vkQueueSubmit(device.getGraphicsQueueHandle(), 1, &submitInfo, syncObjcs.fencesOn[currentFrame]);

	if (r != VK_SUCCESS) {

		std::cout << "QueueSubmit result : " << r << std::endl;
		std::cout << "Current Frame ERROR TIME : " << currentFrame << std::endl;
		std::cout << "ImageIndex  ERROR TIME" << imageIndex << std::endl;
		throw std::runtime_error("failed to submit draw command buffer!");

	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { presentationEngine.getSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(device.getPresentationQueueHandle(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}



	currentFrame = (currentFrame + 1) % MAX_FRAMES;
}



void Render::update(unsigned int currentImage,float frameTIme) {



	float aspect = presentationEngine.getExtent().height / (float)presentationEngine.getExtent().width;
	
	glm::mat4 Identity(1.0f);

	TUGEV::Camera* camera = dynamic_cast<TUGEV::Camera*>(sceneGraph->root->childs[0].get()->entity.get());

	Entity* e = sceneGraph->root->childs[sceneGraph->root->childs.size() - 1]->entity.get();
	glm::mat4 view;

	glm::mat4 projection = camera->getProjectionMatrix();// 

	//The only light that cast shadows
	glm::mat4 depthViewMatrix = lookAt(normalize(sceneLights.lights[4].lightPos)*70.f, glm::vec3(0), glm::vec3(0.0, -1.0, 0.0));

	//glm::mat4 depthProjectionMatrix =  glm::ortho(- .f,10.f,-10.f,10.f,.1f,100.0f);
	//glm::mat4 depthProjectionMatrix = glm::ortho(-10.f,15.f,-10.f,5.f,.1f,150.f);

	//Calculate Frustum BoundingBox to build the depthProjection Matrix
	std::array<float, 6> boundingBox = camera->calculateFrustumInLightSpace(depthViewMatrix);

	//glm::mat4 depthProjectionMatrix = glm::ortho(ortho.x, ortho.y, ortho.z, ortho.w, nearFar.x, nearFar.y);

	glm::mat4 depthProjectionMatrix = glm::ortho(boundingBox[0],boundingBox[1], boundingBox[2], boundingBox[3], nearFar.x,nearFar.y);
	
	view = camera->viewMatrix();

	void* data;
	waterResources.update(*device.getDevice(), currentImage);



	UniformSkybox skyUbo{};
	glm::mat4 captureProjection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
	skyUbo.projection = captureProjection;
	skyUbo.view = view;
	skyUbo.time = (float)glfwGetTime();

	void* data1;
	vkMapMemory(*device.getVulkanDevice(), skybox.uniformBuffers[currentImage].bufferMemory, 0, sizeof(skyUbo), 0, &data1);
	memcpy(data1, &skyUbo, sizeof(skyUbo));
	vkUnmapMemory(*device.getVulkanDevice(), skybox.uniformBuffers[currentImage].bufferMemory);



	glm::mat4 lightMatrix = depthProjectionMatrix * depthViewMatrix;

	void* p;

	sceneLights.eyePos = camera->transform.getPosition();
	sceneLights.lightMatrix = lightMatrix;
	sceneLights.invView = glm::inverse(view);
	sceneLights.invProjection = glm::inverse(projection);

	updateLights(currentImage);

	

	void* data2;

	updateMaterialUniforms(currentImage);

	vkMapMemory(*device.getVulkanDevice(), sceneSettingsUniform[currentImage].bufferMemory, 0, sizeof(SceneSettings), 0, &data2);
	memcpy(data2, &sceneSettings, sizeof(SceneSettings));
	vkUnmapMemory(*device.getVulkanDevice(), sceneSettingsUniform[currentImage].bufferMemory);

	vkMapMemory(*device.getVulkanDevice(), toneMappingBuffer[currentImage].bufferMemory, 0, sizeof(float), 0, &data2);
	memcpy(data2, &sceneSettings.exposure, sizeof(float));
	vkUnmapMemory(*device.getVulkanDevice(), toneMappingBuffer[currentImage].bufferMemory);

	//aoPass.uniforms.radius = .5f;
	//aoPass.uniforms.projection = projection;
	
	VkDeviceSize s = sizeof(glm::vec3) * aoPass.uniforms.kernelSamples.size();
	vkMapMemory(*device.getVulkanDevice(), aoPass.buffers[currentImage].bufferMemory, 0, s, 0, &data2);
	memcpy(data2, &aoPass.uniforms, s);
	vkUnmapMemory(*device.getVulkanDevice(), aoPass.buffers[currentImage] .bufferMemory);

	aoPass.parameters.projection = projection;
	aoPass.parameters.view = view;

	vkMapMemory(*device.getVulkanDevice(), aoPass.parameterBuffer[currentImage].bufferMemory, 0, sizeof(SSAO::Parameters), 0, &data2);
	memcpy(data2, &aoPass.parameters, sizeof(SSAO::Parameters));
	vkUnmapMemory(*device.getVulkanDevice(), aoPass.parameterBuffer[currentImage].bufferMemory);

	vkMapMemory(*device.getVulkanDevice(), visualizationBuffer[currentImage].bufferMemory, 0, sizeof(Diagnosis), 0, &data2);
	memcpy(data2, &diagnosis, sizeof(Diagnosis));
	vkUnmapMemory(*device.getVulkanDevice(), visualizationBuffer[currentImage].bufferMemory);


	for (int i = 0; i < sceneGraph->root->childs.size(); i++) {

		TUGEV::SceneNode* node = sceneGraph->root->childs[i].get();
		node->update(currentImage, Identity, &view, projection, lightMatrix,frameTime);
			
	}

}

void Render::renderCleanUp()
{

	vkDestroyDescriptorSetLayout(*device.getVulkanDevice(), lightDescriptorSetLayout, nullptr);

	
	vkDestroySampler(*device.getDevice(), textSampler, nullptr);

	for (auto& material : materials)material->destroy(*device.getDevice());


	waterResources.destroy(*device.getVulkanDevice());

	renderManager->destroyRenderManager(*device.getVulkanDevice());

	//vkFreeMemory(*device.getVulkanDevice(), skybox.vertexBuffer.bufferMemory, nullptr);
	//vkDestroyBuffer(*device.getVulkanDevice(), skybox.vertexBuffer.buffer, nullptr);
	
	skybox.destroy(*device.getVulkanDevice());
	specularBrdf.destroy(*device.getVulkanDevice());

	
	aoPass.destroy(*device.getVulkanDevice());

	for (unsigned int i = 0; i < presentationEngine.getImageCount(); i++) {

		vkFreeMemory(*device.getVulkanDevice(), lightUniforms[i].bufferMemory, nullptr);
		vkDestroyBuffer(*device.getVulkanDevice(), lightUniforms[i].buffer, nullptr);

		vkFreeMemory(*device.getVulkanDevice(), toneMappingBuffer[i].bufferMemory, nullptr);
		vkDestroyBuffer(*device.getVulkanDevice(), toneMappingBuffer[i].buffer, nullptr);

		vkFreeMemory(*device.getVulkanDevice(), visualizationBuffer[i].bufferMemory, nullptr);
		vkDestroyBuffer(*device.getVulkanDevice(), visualizationBuffer[i].buffer, nullptr);


		vkFreeMemory(*device.getVulkanDevice(), sceneSettingsUniform[i].bufferMemory, nullptr);
		vkDestroyBuffer(*device.getVulkanDevice(), sceneSettingsUniform[i].buffer, nullptr);



		//vkFreeMemory(*device.getVulkanDevice(), skybox.skyUniform[i].bufferMemory, nullptr);
		//vkDestroyBuffer(*device.getVulkanDevice(), skybox.skyUniform[i].buffer, nullptr);
	}
	shadowMapPass.destroy(*device.getDevice());
	vkDestroySampler(*device.getVulkanDevice(), samplerBloom, nullptr);
	vkDestroySampler(*device.getVulkanDevice(), shadowMapSampler, nullptr);
	//vkDestroySampler(*device.getVulkanDevice(), skybox.sampler, nullptr);

	cleaned = true;
	for (unsigned int i = 0; i < sceneGraph->root->childs.size(); i++) {
		sceneGraph->root->childs[i].get()->entity.get()->destroy();
	}

	for (size_t i = 0; i < MAX_FRAMES; i++) {
		vkDestroySemaphore(*device.getVulkanDevice(), syncObjcs.imageAvaible[i], nullptr);
		vkDestroySemaphore(*device.getVulkanDevice(), syncObjcs.renderFinished[i], nullptr);
		vkDestroyFence(*device.getVulkanDevice(), syncObjcs.fencesOn[i], nullptr);
	}

	vkDestroyCommandPool(*device.getVulkanDevice(), cmdPool, nullptr);
	vkDestroyCommandPool(*device.getVulkanDevice(), transientCmdPool, nullptr);
	vkDestroyCommandPool(*device.getVulkanDevice(), transferPool, nullptr);

	for (auto frameBuffer : mainFrameBuffers) {
		vkDestroyFramebuffer(*device.getVulkanDevice(), frameBuffer, nullptr);
	}
	for (auto frameBuffer : verticalBlurFrameBuffer) {
		vkDestroyFramebuffer(*device.getVulkanDevice(), frameBuffer, nullptr);
	}
	for (auto frameBuffer : horizontalBlurFrameBuffer) {
		vkDestroyFramebuffer(*device.getVulkanDevice(), frameBuffer, nullptr);
	}
	for (auto frameBuffer : shadowMapPass.frameBuffers) {
		vkDestroyFramebuffer(*device.getVulkanDevice(), frameBuffer, nullptr);
	}

	vkDestroyDescriptorPool(*device.getVulkanDevice(), descriptorPool, nullptr);
	vkDestroyDescriptorPool(*device.getVulkanDevice(), dynamicDescriptorPool, nullptr);

	vkDestroyImageView(*device.getVulkanDevice(), depthResource.view, nullptr);
	vkDestroyImage(*device.getVulkanDevice(), depthResource.image, nullptr);
	vkFreeMemory(*device.getVulkanDevice(), depthResource.memory, nullptr);

	vkDestroyImageView(*device.getVulkanDevice(), depthMap.view, nullptr);
	vkDestroyImage(*device.getVulkanDevice(), depthMap.image, nullptr);
	vkFreeMemory(*device.getVulkanDevice(), depthMap.memory, nullptr);

	for (unsigned int i = 0; i < presentationEngine.getImageCount(); i++) {

		vkDestroyImageView(*device.getVulkanDevice(), frameBufferAttachments[i].view, nullptr);
		vkDestroyImage(*device.getVulkanDevice(), frameBufferAttachments[i].image, nullptr);
		vkFreeMemory(*device.getVulkanDevice(), frameBufferAttachments[i].memory, nullptr);


		vkDestroyImageView(*device.getVulkanDevice(), lumenColorImage[i].view, nullptr);
		vkDestroyImage(*device.getVulkanDevice(), lumenColorImage[i].image, nullptr);
		vkFreeMemory(*device.getVulkanDevice(), lumenColorImage[i].memory, nullptr);

		vkDestroyImageView(*device.getVulkanDevice(), colorImageAttachment[i].view, nullptr);
		vkDestroyImage(*device.getVulkanDevice(), colorImageAttachment[i].image, nullptr);
		vkFreeMemory(*device.getVulkanDevice(), colorImageAttachment[i].memory, nullptr);

		vkDestroyImageView(*device.getVulkanDevice(), verticalBlurImage[i].view, nullptr);
		vkDestroyImage(*device.getVulkanDevice(), verticalBlurImage[i].image, nullptr);
		vkFreeMemory(*device.getVulkanDevice(), verticalBlurImage[i].memory, nullptr);

	}

	deferedResources.destroy(device.getVulkanDevice());



	vkDestroyRenderPass(*device.getVulkanDevice(), mainRenderPass, nullptr);
	vkDestroyRenderPass(*device.getVulkanDevice(), verticalBlurRenderPass, nullptr);
	vkDestroyRenderPass(*device.getVulkanDevice(), horizontalBlurRenderPass, nullptr);
	vkDestroyRenderPass(*device.getVulkanDevice(), shadowMapPass.renderpass, nullptr);

	if (enableValidationlayer)debuger.cleanDebugger();

	guiResources.destroy(device.getVulkanDevice(), device.getGraphicsQueueHandle(), &instance, window);

	presentationEngine.cleanUp();

	device.cleanUp();

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);


	glfwTerminate();
	system("pause\n");
}
