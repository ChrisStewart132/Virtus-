#pragma once
struct config {
	uint32_t windowWidth = 1920;
	uint32_t windowHeight = 1080;
	uint32_t viewWidth = 1280;
	uint32_t viewHeight = 720;

	uint32_t swapChainCount = 1;//not finished

	bool lighting = true;
	bool textures = false;
	bool mip = false;
	bool antialiasing = false;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	bool samepleShading = false;
	bool transparentWindow = true;
	float windowTransparency = 0.5f;
	bool renderingToWindow = true;//not finished
	bool multipleViewports = true;

	config() {
		check();
	}

	void config::check() {
		if (!textures) { mip = false; }
	}
};