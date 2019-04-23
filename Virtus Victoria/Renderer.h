#pragma once
#include <vulkan/vulkan.h>
#include "sonar.h"
#include "Setup.h"
#include "Descriptors.h"
#include "FrameBuffer.h"
#include "Pipeline.h"
#include "Models.h"
#include "Window.h"
#include "Game.h"
#include "Physics.h"

const uint32_t commandBufferCount = 1;

//
class Renderer
{
private:
	//double t = 0;
	double dt = 1.0 / 144.0;
	
	sonar sonar;

	Setup *setupPtr;
	Descriptors *descriptorsPtr;
	RenderPass *renderPassPtr;
	FrameBuffer *frameBufferPtr;
	Pipeline *pipelinePtr;
	Models *modelsPtr;
	Window *windowPtr;
	Game *gamePtr;
	Physics physics;

	UniformBufferObject ubo;

	double mousePosX, mousePosY;
	glm::vec3 cameraPosition = { 0.0f,2.0f,3.0f };
	glm::vec3 cameraPoint = { 0.0f,2.0f,0.0f };

	
	VkCommandBuffer commandBuffers[commandBufferCount];						//deleted with commandPool auto
	void createCommandBuffers();


	VkSemaphore imageAvailableSemaphore;				//vkDestroySemaphore(device,imageAvailableSemaphore,nullptr);
	VkSemaphore renderFinishedSemaphore;				//vkDestroySemaphore(device,renderFinishedSemaphore,nullptr);
	void createSemaphores();

	

	void drawFrame();
	void update();
	void input();
	void updateUniformBuffer();
	void moveUnits();

public:
	Renderer(Setup *_setupPtr, Descriptors *_descriptorsPtr, RenderPass *_renderPassPtr, FrameBuffer *_frameBufferPtr, Pipeline *_pipelinePtr, Models *_modelsPtr, Window *_windowPtr, Game *_gamePtr);

	~Renderer();

	void run();
	void destroy();
};