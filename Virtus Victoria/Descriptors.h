#pragma once
#include <vulkan/vulkan.h>
#include <array>

#include <stb_image.h>

#include "Setup.h"
#include "Models.h"
#include "DescriptorSetLayout.h"

#define MAX_LIGHTS 4
struct light {
	glm::vec3 pos = glm::vec3(10.0f);
	glm::vec3 colour = glm::vec3(0.5f);
	glm::vec3 intensity = glm::vec3(0.5f);//0-1
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 lightPosition[MAX_LIGHTS];
	glm::vec3 lightColour[MAX_LIGHTS];
	glm::vec3 lightIntensity[MAX_LIGHTS];
	glm::vec3 ambient;
};

const uint32_t unitTypeCount = 9;

class Descriptors
{
private:
	Setup *setupPtr;
	Models *modelsPtr;
	DescriptorSetLayout* descriptorSetLayoutPtr;
	

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void generateMipmaps(VkImage image, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	void createTextureImage(std::string TEXTURE_PATH, uint32_t index);

	void createTextureImages();
	void createTextureImageViews();
	void createTextureSamplers();

	void createUniformBuffer();

	void createDescriptorPool();
	void createDescriptorSets();

	
	void createVertexBuffer();
	
	
public:
	Descriptors(Setup *_setupPtr, Models *_modelsPtr, DescriptorSetLayout *_descriptorSetLayoutPtr );
	~Descriptors();

	

	uint32_t mipLevels[unitTypeCount];								//createimageviews sets eacg mipLevels
	VkImage textureImages[unitTypeCount];								//vkDestroyImage(device, textureImage, nullptr);
	VkDeviceMemory textureImageMemory[unitTypeCount];					//vkFreeMemory(device, textureImageMemory, nullptr);
	
	VkImageView textureImageViews[unitTypeCount];						//vkDestroyImageView(device, textureImageView, nullptr);
	VkSampler textureSamplers[unitTypeCount];							//vkDestroySampler(device, textureSampler, nullptr);

	VkBuffer uniformBuffer;								//vkDestroyBuffer(device,uniformBuffer,nullptr);
	VkDeviceMemory uniformBufferMemory;					//vkFreeMemory(device,uniformBufferMemory,nullptr);

	VkDescriptorPool descriptorPool;					//vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	VkDescriptorSet descriptorSets[1];

	VkBuffer vertexBuffer;								//vkDestroyBuffer(device,vertexBuffer,nullptr);
	VkDeviceMemory vertexBufferMemory;					//vkFreeMemory(device,vertexBufferMemory,nullptr);

	void destroy();
};

