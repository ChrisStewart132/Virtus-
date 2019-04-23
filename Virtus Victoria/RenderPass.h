#pragma once
#include <vulkan/vulkan.h>

#include <array>

#include "Setup.h"

class RenderPass
{
private:
	Setup* setupPtr;
	
	//VkFramebuffer* swapChainFrameBuffers;


	void createRenderPass();
	//void createFramebuffers();

public:
	
	RenderPass(Setup* _setupPtr);
	~RenderPass();

	VkRenderPass renderPass;

	void destroy();
};


