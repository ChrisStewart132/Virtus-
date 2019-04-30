#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "config.h"
//Setup class
//instance,surface,device,queue,swapchains,shader creation functions
//future: choose best physical device, check physical device swap chain capabilities to change config, e.g present mode, number of swapchains

class Setup
{
private:
	VkResult result;
	void createInstance();
	void createSurface();
	void createDevice();
	void createSwapChain();
	void createCommandPool();
public:
	GLFWwindow* windowPtr;
	Setup();
	Setup(GLFWwindow *_glfwWindow, config *_configPtr);
	~Setup();
	VkInstance instance;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue queue;
	uint32_t queueFamilyIndex;


	VkColorSpaceKHR swapchainImageColourSpace;
	VkFormat swapchainImageFormat;
	std::vector<VkSwapchainKHR> swapChains;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	VkCommandPool commandPool;															//vkDestroyCommandPool(device,commandPool,nullptr);		

	config *configPtr;

	//helper functions
	VkSampleCountFlagBits Setup::getMaxUsableSampleCount();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView Setup::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	bool hasStencilComponent(VkFormat format);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	//helper functions

	void destroy();
};
