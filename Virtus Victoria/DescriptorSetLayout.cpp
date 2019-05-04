#include "DescriptorSetLayout.h"



DescriptorSetLayout::DescriptorSetLayout(Setup* _setupPtr, Models *_modelsPtr)
{
	setupPtr = _setupPtr;
	modelsPtr = _modelsPtr;
	createDescriptorSetLayout();
}


DescriptorSetLayout::~DescriptorSetLayout()
{
	
}

void DescriptorSetLayout::destroy()
{
	vkDestroyDescriptorSetLayout(setupPtr->device, descriptorSetLayout, nullptr);
}

void DescriptorSetLayout::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = modelsPtr->unitTypeList.size();// modelsPtr->unitTypeList.size();//1 texture sampler per texture (1 texture per unitType)
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());;
	layoutInfo.pBindings = bindings.data();
	VkResult result;
	result=vkCreateDescriptorSetLayout(setupPtr->device, &layoutInfo, nullptr, &descriptorSetLayout);
#ifdef _DEBUG
	std::cout << result << std::endl;
#endif
}