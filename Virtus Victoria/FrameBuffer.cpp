#include "FrameBuffer.h"



FrameBuffer::FrameBuffer(Setup *_setupPtr, RenderPass *_renderPassPtr)
{
	setupPtr = _setupPtr;
	renderPassPtr = _renderPassPtr;
	if (setupPtr->configPtr->antialiasing) {
		createColorResources();
	}
	createDepthResources();
	createFrameBuffers();
}


FrameBuffer::~FrameBuffer()
{
	
}

void FrameBuffer::destroy()
{
	if (setupPtr->configPtr->antialiasing) {
		vkDestroyImageView(setupPtr->device, colorImageView, nullptr);
		vkDestroyImage(setupPtr->device, colorImage, nullptr);
		vkFreeMemory(setupPtr->device, colorImageMemory, nullptr);
	}
	vkDestroyImageView(setupPtr->device, depthImageView, nullptr);
	vkDestroyImage(setupPtr->device, depthImage, nullptr);
	vkFreeMemory(setupPtr->device, depthImageMemory, nullptr);
	vkDestroyFramebuffer(setupPtr->device, swapChainFramebuffers, nullptr);
}


void FrameBuffer::createColorResources() {
	VkFormat colorFormat = setupPtr->swapchainImageFormat;

	setupPtr->createImage(setupPtr->configPtr->windowWidth, setupPtr->configPtr->windowHeight, 1, setupPtr->configPtr->msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	colorImageView = setupPtr->createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	setupPtr->transitionImageLayout(colorImage, colorFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
}

void FrameBuffer::createDepthResources()
{
	VkFormat depthFormat = setupPtr->findDepthFormat();;
	setupPtr->createImage(setupPtr->configPtr->windowWidth, setupPtr->configPtr->windowHeight, 1, setupPtr->configPtr->msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);

	VkImageViewCreateInfo viewInfoDepth = {};
	viewInfoDepth.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfoDepth.image = depthImage;
	viewInfoDepth.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfoDepth.format = depthFormat;
	viewInfoDepth.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	viewInfoDepth.subresourceRange.baseMipLevel = 0;
	viewInfoDepth.subresourceRange.levelCount = 1;
	viewInfoDepth.subresourceRange.baseArrayLayer = 0;
	viewInfoDepth.subresourceRange.layerCount = 1;

	vkCreateImageView(setupPtr->device, &viewInfoDepth, nullptr, &depthImageView);

	setupPtr->transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);

}

void FrameBuffer::createFrameBuffers()
{
	if (setupPtr->configPtr->antialiasing) {
		std::array<VkImageView, 3> attachments = {//IMPORTANT: DEPTH IMAGE MUST ALLIGN WITH RENDERPASS DEPTH ATTACHMENT
			colorImageView,depthImageView, setupPtr->swapChainImageViews[0]

		};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPassPtr->renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = setupPtr->configPtr->windowWidth;
		framebufferInfo.height = setupPtr->configPtr->windowHeight;
		framebufferInfo.layers = 1;
		VkResult result;
		result = vkCreateFramebuffer(setupPtr->device, &framebufferInfo, nullptr, &swapChainFramebuffers);
#ifdef _DEBUG
		std::cout << result << std::endl;
#endif
	}
	else {
		std::array<VkImageView, 2> attachments = {//IMPORTANT: DEPTH IMAGE MUST ALLIGN WITH RENDERPASS DEPTH ATTACHMENT
			setupPtr->swapChainImageViews[0],depthImageView

		};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPassPtr->renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = setupPtr->configPtr->windowWidth;
		framebufferInfo.height = setupPtr->configPtr->windowHeight;
		framebufferInfo.layers = 1;
		VkResult result;
		result = vkCreateFramebuffer(setupPtr->device, &framebufferInfo, nullptr, &swapChainFramebuffers);
#ifdef _DEBUG
		std::cout << result << std::endl;
#endif
	}
	
	
}