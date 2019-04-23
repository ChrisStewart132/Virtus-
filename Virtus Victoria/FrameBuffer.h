#pragma once
#pragma once
#include <vulkan/vulkan.h>

#include <array>

#include "Setup.h"
#include "RenderPass.h"


class FrameBuffer
{
private:
	Setup* setupPtr;
	RenderPass* renderPassPtr;
	
	void createColorResources();
	void createDepthResources();
	void createFrameBuffers();
	
public:
	FrameBuffer(Setup *_setupPtr, RenderPass *_renderPassPtr);
	~FrameBuffer();

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	VkImage depthImage;
	VkImageView depthImageView;
	VkDeviceMemory depthImageMemory;
	VkFramebuffer swapChainFramebuffers;					//vkDestroyFramebuffer(device,swapChainFramebuffer,nullptr);
	void destroy();
};

