#include "Descriptors.h"



Descriptors::Descriptors(Setup *_setupPtr, Models *_modelsPtr, DescriptorSetLayout *_descriptorSetLayoutPtr)
{
	setupPtr = _setupPtr;
	modelsPtr = _modelsPtr;
	descriptorSetLayoutPtr = _descriptorSetLayoutPtr;
	
	

	createUniformBuffer();
	createVertexBuffer();

	/*
	unitTypeCount = modelsPtr->unitTypeList.size();
	mipLevels = new uint32_t(unitTypeCount);
	textureImages = new VkImage(unitTypeCount);
	textureImageMemory = new VkDeviceMemory(unitTypeCount);

	textureImageViews = new VkImageView(unitTypeCount);
	textureSamplers = new VkSampler(unitTypeCount);
	*/

	createTextureImages();
	createTextureImageViews();
	createTextureSamplers();

	createDescriptorPool();
	createDescriptorSets();
	
}


Descriptors::~Descriptors()
{
	//delete[] mipLevels, textureImages, textureImageMemory, textureImageViews, textureSamplers;
}

void Descriptors::destroy()
{
	
	

	vkDestroyBuffer(setupPtr->device, uniformBuffer, nullptr);
	vkFreeMemory(setupPtr->device, uniformBufferMemory, nullptr);
	vkDestroyBuffer(setupPtr->device, vertexBuffer, nullptr);
	vkFreeMemory(setupPtr->device, vertexBufferMemory, nullptr);

	
	for (uint32_t i = 0; i < unitTypeCount; i++) {
		
		vkDestroyImage(setupPtr->device, textureImages[i], nullptr);
		vkDestroyImageView(setupPtr->device, textureImageViews[i], nullptr);
		vkDestroySampler(setupPtr->device, textureSamplers[i], nullptr);
		vkFreeMemory(setupPtr->device, textureImageMemory[i], nullptr);
		
	}
	

	vkDestroyDescriptorPool(setupPtr->device, descriptorPool, nullptr);
	
}






	
void Descriptors::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = setupPtr->beginSingleTimeCommands();
	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};
	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	setupPtr->endSingleTimeCommands(commandBuffer);
}

void Descriptors::generateMipmaps(VkImage image, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {
	VkCommandBuffer commandBuffer = setupPtr->beginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit = {};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth / 2, mipHeight / 2, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	setupPtr->endSingleTimeCommands(commandBuffer);
}

void Descriptors::createTextureImage(std::string TEXTURE_PATH, uint32_t index)
{

	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	mipLevels[index] = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = imageSize;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkCreateBuffer(setupPtr->device, &bufferInfo, nullptr, &stagingBuffer);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(setupPtr->device, stagingBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = setupPtr->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkAllocateMemory(setupPtr->device, &allocInfo, nullptr, &stagingBufferMemory);
	vkBindBufferMemory(setupPtr->device, stagingBuffer, stagingBufferMemory, 0);

	void* data;
	vkMapMemory(setupPtr->device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(setupPtr->device, stagingBufferMemory);

	stbi_image_free(pixels);

	setupPtr->createImage(texWidth, texHeight,mipLevels[index], VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImages[index], textureImageMemory[index]);

	setupPtr->transitionImageLayout(textureImages[index], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels[index]);
	copyBufferToImage(stagingBuffer, textureImages[index], static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	//transitionImageLayout(textureImages[loadedTextures], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);


	vkDestroyBuffer(setupPtr->device, stagingBuffer, nullptr);
	vkFreeMemory(setupPtr->device, stagingBufferMemory, nullptr);

	generateMipmaps(textureImages[index], texWidth, texHeight, mipLevels[index]);

}

void Descriptors::createTextureImages() {
	for (uint32_t i = 0; i < unitTypeCount; i++) {
		createTextureImage(modelsPtr->unitTypeList[i].texturePath, i);
	}
}

void Descriptors::createTextureImageViews()
{
	for (uint32_t i = 0; i < unitTypeCount; i++) {
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = textureImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = mipLevels[i];
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		vkCreateImageView(setupPtr->device, &viewInfo, nullptr, &textureImageViews[i]);
	}
}

void Descriptors::createTextureSamplers()
{
	for (uint32_t i = 0; i < unitTypeCount; i++) {
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;//VK_TRUE
		samplerInfo.maxAnisotropy = 16;//16
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0; // Optional
		samplerInfo.maxLod = static_cast<float>(mipLevels[i]);
		samplerInfo.mipLodBias = 0; // Optional

		vkCreateSampler(setupPtr->device, &samplerInfo, nullptr, &textureSamplers[i]);
	}	
}

void Descriptors::createUniformBuffer()
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(UniformBufferObject);
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(setupPtr->device, &bufferInfo, nullptr, &uniformBuffer);

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(setupPtr->device, uniformBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = setupPtr->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkAllocateMemory(setupPtr->device, &allocInfo, nullptr, &uniformBufferMemory);
	vkBindBufferMemory(setupPtr->device, uniformBuffer, uniformBufferMemory, 0);
}


void Descriptors::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 1;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = unitTypeCount;
	

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 1;

	vkCreateDescriptorPool(setupPtr->device, &poolInfo, nullptr, &descriptorPool);
}

void Descriptors::createDescriptorSets() {
	VkDescriptorSetLayout layouts[] = { descriptorSetLayoutPtr->descriptorSetLayout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	vkAllocateDescriptorSets(setupPtr->device, &allocInfo, descriptorSets);

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = uniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	std::vector<VkDescriptorImageInfo> textureImageInfoArray;
	for (uint32_t i = 0; i < unitTypeCount; i++) {
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageViews[i];
		imageInfo.sampler = textureSamplers[i];
		textureImageInfoArray.push_back(imageInfo);
	}

	

	VkWriteDescriptorSet descriptorWrites[2] = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = descriptorSets[0];
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = descriptorSets[0];
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = textureImageInfoArray.size();
	descriptorWrites[1].pImageInfo = textureImageInfoArray.data();

	vkUpdateDescriptorSets(setupPtr->device, (uint32_t)2, descriptorWrites, 0, nullptr);
	
}



void Descriptors::createVertexBuffer()
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//bufferInfo.size = sizeof(modelsPtr->vertices[0]) * modelsPtr->vertices.size();
	bufferInfo.size = 128000000;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkCreateBuffer(setupPtr->device, &bufferInfo, nullptr, &vertexBuffer);
	printf("vertex buffer size: %i\n", bufferInfo.size);
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(setupPtr->device, vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = setupPtr->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vkAllocateMemory(setupPtr->device, &allocInfo, nullptr, &vertexBufferMemory);
	vkBindBufferMemory(setupPtr->device, vertexBuffer, vertexBufferMemory, 0);

	//
	void* data;
	vkMapMemory(setupPtr->device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, modelsPtr->vertices.data(), (size_t)sizeof(modelsPtr->vertices[0]) * modelsPtr->vertices.size());
	vkUnmapMemory(setupPtr->device, vertexBufferMemory);
}


