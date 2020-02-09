#pragma once
#include <vulkan/vulkan.h>
#include <fstream>
#include <vector>

#include "Setup.h"
#include "Vertex.h"
#include "DescriptorSetLayout.h"
#include "RenderPass.h"

struct pipelineInfo {

	VkColorComponentFlags colours = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
	float rasterLineWidth = 3.0f;//thickness of lines when rasterizing in line mode
	std::string fragShader="frag";
	std::string vertShader="vert";
};

struct textureInfo {
	uint32_t textureIndex = 0;
};

class Pipeline
{
private:
	Setup* setupPtr;
	DescriptorSetLayout* descriptorSetLayoutPtr;
	RenderPass* renderPassPtr;

	void createGraphicsPipeline();

	std::vector<char> readFile(const std::string& filename);
	void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);
public:
	
	Pipeline(Setup* _setupPtr, DescriptorSetLayout* _descriptorSetLayoutPtr, RenderPass* _renderPassPtr, pipelineInfo _pipelineInfo);
	~Pipeline();

	pipelineInfo settings;

	VkPipelineLayout pipelineLayout;					//vkDestroyPipelineLayout(device,pipelineLayout,nullptr);
	VkPipeline graphicsPipeline;						//vkDestroyPipeline(device,graphicsPipeline,nullptr);


	void destroy();
};

