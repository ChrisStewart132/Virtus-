#pragma once

#include <vulkan/vulkan.h>
#include <array>

#include "Setup.h"
#include "Models.h"


class DescriptorSetLayout
{
private:
	Setup* setupPtr;
	Models* modelsPtr;

	void createDescriptorSetLayout();

public:
	DescriptorSetLayout(Setup* _setupPtr, Models* _modelsPtr);
	~DescriptorSetLayout();
	VkDescriptorSetLayout descriptorSetLayout;

	void destroy();
};

