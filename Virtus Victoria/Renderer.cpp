#include "Renderer.h"



Renderer::Renderer(Setup *_setupPtr, Descriptors *_descriptorsPtr, RenderPass *_renderPassPtr, FrameBuffer *_frameBufferPtr, Pipeline *_pipelinePtr, Models *_modelsPtr, Window *_windowPtr, Game *_gamePtr)
{
	setupPtr = _setupPtr;
	descriptorsPtr = _descriptorsPtr;
	renderPassPtr = _renderPassPtr;
	frameBufferPtr = _frameBufferPtr;
	pipelinePtr = _pipelinePtr;
	modelsPtr = _modelsPtr;
	windowPtr = _windowPtr;
	gamePtr = _gamePtr;
	createCommandBuffers();
	createSemaphores();
}


Renderer::~Renderer()
{
	
}

void Renderer::destroy()
{
	vkDestroySemaphore(setupPtr->device, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(setupPtr->device, renderFinishedSemaphore, nullptr);
}

void Renderer::createCommandBuffers()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = setupPtr->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = commandBufferCount;

	vkAllocateCommandBuffers(setupPtr->device, &allocInfo, commandBuffers);

	for (int i = 0; i < commandBufferCount; i++) {
		//below for each imag (only 1 atm)for(;;){}
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPassPtr->renderPass;
		renderPassInfo.framebuffer = frameBufferPtr->swapChainFramebuffers;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { setupPtr->configPtr->windowWidth,setupPtr->configPtr->windowHeight };

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[0].graphicsPipeline);


		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &descriptorsPtr->vertexBuffer, offsets);
		//vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &descriptorsPtr->vertexBuffer2, offsets);

		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[0].pipelineLayout, 0, 1, descriptorsPtr->descriptorSets, 0, nullptr);
		//
		vkCmdDraw(commandBuffers[i], modelsPtr->vertices.size(), 1, 0, 0);//vertex count, instance cnt,first vertex,first instance
		//vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);
		
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void Renderer::createSemaphores()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(setupPtr->device, &semaphoreInfo, nullptr, &imageAvailableSemaphore);	
	vkCreateSemaphore(setupPtr->device, &semaphoreInfo, nullptr, &renderFinishedSemaphore);
}

void Renderer::drawFrame()
{
	//begin render
	uint32_t imageIndex;
	vkAcquireNextImageKHR(setupPtr->device, setupPtr->swapChains[0], std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);//3rd option=nanosecond timeout wait for each image

	update();

	//record data -begin command buffer and render pass
	//change vertices data (move shapes)
	//bind        -vkBindBufferMemory vertex buffer, vkmapMemory-vertexbuffer-vkunmapMemory, bind pipeline, vertex, and index buffer
	//draw        -vkdraw indexed
	//unbind      -vkend renderpass, end command buffer recording

	//end render
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = commandBufferCount;
	submitInfo.pCommandBuffers = commandBuffers;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

	vkQueueSubmit(setupPtr->queue, 1, &submitInfo, VK_NULL_HANDLE);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
	presentInfo.swapchainCount = setupPtr->configPtr->swapChainCount;
	presentInfo.pSwapchains = setupPtr->swapChains.data();
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(setupPtr->queue, &presentInfo);
	vkQueueWaitIdle(setupPtr->queue);
}

\
void Renderer::update()
{
	//start acquire swapchain image done in run function
	//update called in middle of run();
	//record command buffer vkBeginCommandBuffer
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	//beginInfo.pInheritanceInfo = nullptr; // Optional
	vkBeginCommandBuffer(commandBuffers[0], &beginInfo);
	//vkCmdBeginRenderPass
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPassPtr->renderPass;
	renderPassInfo.framebuffer = frameBufferPtr->swapChainFramebuffers;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = { setupPtr->configPtr->windowWidth,setupPtr->configPtr->windowHeight };
	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();
	vkCmdBeginRenderPass(commandBuffers[0], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


	//////////////////bind first pipeline 
	int pipelineIndex;
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)setupPtr->configPtr->windowWidth;
	viewport.height = (float)setupPtr->configPtr->windowHeight;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	if (windowPtr->isKeyPressed(GLFW_KEY_4)) {
		scissor.extent = { setupPtr->configPtr->viewWidth, setupPtr->configPtr->viewHeight };
		scissor.offset = { (int32_t)(setupPtr->configPtr->windowWidth - setupPtr->configPtr->viewWidth) / 2, (int32_t)(setupPtr->configPtr->windowHeight - setupPtr->configPtr->viewHeight) / 2 };
	}
	else {
		scissor.extent = { setupPtr->configPtr->windowWidth, setupPtr->configPtr->windowHeight };
		scissor.offset = { 0,0 };
	}

	std::array<VkViewport, 1> viewports;
	viewports[0] = viewport;
	std::array<VkRect2D, 1> scissors;
	scissors[0] = scissor;
	vkCmdSetViewport(commandBuffers[0], 0, viewports.size(), viewports.data());
	vkCmdSetScissor(commandBuffers[0], 0, scissors.size(), scissors.data());

	pipelineIndex = 0;
	vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);//fill
	//////////////////bind first pipeline 
	uint32_t hitboxVertexCount = modelsPtr->unitList.size() * 36;//every unit automatically has a 36 vertex hitbox rendered 
	textureInfo texturePushConstant;

	moveUnits();//change vertex data

	//update and bind ubo
	updateUniformBuffer();//vkmapMemory-ubo object-vkunmapMemory

	if (1) {
		//bind vertex data
		vkBindBufferMemory(setupPtr->device, descriptorsPtr->vertexBuffer, descriptorsPtr->vertexBufferMemory, 0);
		size_t bufferSize = sizeof(modelsPtr->vertices[0]) * modelsPtr->vertices.size();
		void* data;
		vkMapMemory(setupPtr->device, descriptorsPtr->vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, modelsPtr->vertices.data(), bufferSize);
		vkUnmapMemory(setupPtr->device, descriptorsPtr->vertexBufferMemory);
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[0], 0, 1, &descriptorsPtr->vertexBuffer, offsets);
		//vkCmdBindIndexBuffer(commandBuffers[0], indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	

	
	//main draw pass
	vkCmdBindDescriptorSets(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].pipelineLayout, 0, 1, &descriptorsPtr->descriptorSets[0], 0, nullptr);
	if (setupPtr->configPtr->textures) {
		for (uint32_t i = 0; i < modelsPtr->unitTypeList.size(); i++) {//for each unit type and therefore each texture
			texturePushConstant.textureIndex = i;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(texturePushConstant),&texturePushConstant);
			//vkCmdDrawIndexed(commandBuffers[0], indices.size(), 1, 0, 0, 0);
			for (uint32_t j = 0; j < modelsPtr->unitList.size(); j++) {//for every single unit loaded
				if (modelsPtr->unitList[j].unitTypePtr->textureIndex == i) {//if the unit type matches the unit
					vkCmdDraw(commandBuffers[0], modelsPtr->unitList[j].unitTypePtr->vertices.size(), 1, modelsPtr->unitList[j].vertexStart, 0);//for every texture, 1 draw call per unit loaded with current texture
				}
			}
		}
	}
	else {//
		texturePushConstant.textureIndex = 5;
		vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(texturePushConstant),&texturePushConstant);
		vkCmdDraw(commandBuffers[0], modelsPtr->vertices.size()-hitboxVertexCount, 1, 0, 0);//1 draw call for all units
	}
	


	if (windowPtr->isKeyPressed(GLFW_KEY_1) || 0) {
		//bind vertex2 data (hitbox / debug)
		vkBindBufferMemory(setupPtr->device, descriptorsPtr->vertexBuffer2, descriptorsPtr->vertexBufferMemory2, 0);
		size_t bufferSize2 = sizeof(modelsPtr->vertices2[0]) * modelsPtr->vertices2.size();
		void* data2;
		vkMapMemory(setupPtr->device, descriptorsPtr->vertexBufferMemory2, 0, bufferSize2, 0, &data2);
		memcpy(data2, modelsPtr->vertices2.data(), bufferSize2);
		vkUnmapMemory(setupPtr->device, descriptorsPtr->vertexBufferMemory2);
		VkDeviceSize offsets2[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[0], 0, 1, &descriptorsPtr->vertexBuffer2, offsets2);

		//render vertex2 
		pipelineIndex = 1;
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);//line
		vkCmdBindDescriptorSets(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].pipelineLayout, 0, 1, &descriptorsPtr->descriptorSets[0], 0, nullptr);
		texturePushConstant.textureIndex = 5;
		vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);
		vkCmdDraw(commandBuffers[0], modelsPtr->vertices2.size(), 1, 0, 0);//1 draw call for all units
	}








	//rendering hitboxes with lines polygon mode pipeline to main viewport
	if (windowPtr->isKeyPressed(GLFW_KEY_1) && 0) {
		pipelineIndex = 1;
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);//line

		uint32_t hitboxVertexCount = modelsPtr->unitList.size() * 36;//every unit automatically has a 36 vertex hitbox rendered 
		textureInfo texturePushConstant;
		vkCmdBindDescriptorSets(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].pipelineLayout, 0, 1, &descriptorsPtr->descriptorSets[0], 0, nullptr);
		if (setupPtr->configPtr->textures) {
			//if (windowPtr->isKeyPressed(GLFW_KEY_1)) {//if pressing 1 will be rendering line mode, also render the hitboxes of each unit here
				texturePushConstant.textureIndex = 4;
				vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);//or VK_SHADER_STAGE_VERTEX_BIT if push uses this flag
				vkCmdDraw(commandBuffers[0], hitboxVertexCount, 1, modelsPtr->vertices.size() - hitboxVertexCount, 0);//
			//}
		}
		else {//
			texturePushConstant.textureIndex = 5;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);
			vkCmdDraw(commandBuffers[0], hitboxVertexCount, 1, modelsPtr->vertices.size() - hitboxVertexCount, 0);//1 draw call for all units
		}
	}

	//render 2nd viewport if()
	if (windowPtr->isKeyPressed(GLFW_KEY_3) || 0) {	
		pipelineIndex = 3;
		float viewportSizeFactor = 4;//1 = full scale higher for smaller left bottom viewport
		viewports[0].minDepth = 0.0f;
		viewports[0].maxDepth = 0.9f;
		viewports[0].width = (float)setupPtr->configPtr->windowWidth / viewportSizeFactor;
		viewports[0].height = (float)setupPtr->configPtr->windowHeight / viewportSizeFactor;
		if (viewportSizeFactor==1){
			viewports[0].x = 0;
			viewports[0].y = 0;
		}
		else {
			viewports[0].x = (viewports[0].width / 10);
			viewports[0].y = setupPtr->configPtr->windowHeight - viewports[0].height - (viewports[0].width / 10);
		}
		scissors[0].extent = { (uint32_t)viewports[0].width,(uint32_t)viewports[0].height };
		scissors[0].offset = { (int32_t)viewports[0].x,(int32_t)viewports[0].y };
		vkCmdSetViewport(commandBuffers[0], 0, viewports.size(), viewports.data());
		vkCmdSetScissor(commandBuffers[0], 0, scissors.size(), scissors.data());
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);

		if (setupPtr->configPtr->textures & 0) {
			for (uint32_t i = 0; i < modelsPtr->unitTypeList.size(); i++) {//for each unit type and therefore each texture
				texturePushConstant.textureIndex = i;
				vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);
				//vkCmdDrawIndexed(commandBuffers[0], indices.size(), 1, 0, 0, 0);
				for (uint32_t j = 0; j < modelsPtr->unitList.size(); j++) {//for every single unit loaded
					if (modelsPtr->unitList[j].unitTypePtr->textureIndex == i) {//if the unit type matches the unit
						vkCmdDraw(commandBuffers[0], modelsPtr->unitList[j].unitTypePtr->vertices.size(), 1, modelsPtr->unitList[j].vertexStart, 0);//for every texture, 1 draw call per unit loaded with current texture
					}
				}
			}
		}
		else {//
			texturePushConstant.textureIndex = 5;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);
			vkCmdDraw(commandBuffers[0], modelsPtr->vertices.size() - hitboxVertexCount, 1, 0, 0);//1 draw call for all units
		}

		//render 2nd viewport hitboxes with hitbo pipeline
		pipelineIndex = 4;
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);
		//if (windowPtr->isKeyPressed(GLFW_KEY_1)) {
			//hitboxVertexCount = modelsPtr->unitList.size() * 36;//every unit automatically has a 36 vertex hitbox rendered 
			//texturePushConstant;
			//vkCmdBindDescriptorSets(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].pipelineLayout, 0, 1, &descriptorsPtr->descriptorSets[0], 0, nullptr);
		if (setupPtr->configPtr->textures) {
			//if (windowPtr->isKeyPressed(GLFW_KEY_1)) {//if pressing 1 will be rendering line mode, also render the hitboxes of each unit here
			texturePushConstant.textureIndex = 4;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);//or VK_SHADER_STAGE_VERTEX_BIT if push uses this flag
			vkCmdDraw(commandBuffers[0], hitboxVertexCount, 1, modelsPtr->vertices.size() - hitboxVertexCount, 0);//
		//}
		}
		else {//
			texturePushConstant.textureIndex = 5;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(texturePushConstant), &texturePushConstant);
			vkCmdDraw(commandBuffers[0], hitboxVertexCount, 1, modelsPtr->vertices.size() - hitboxVertexCount, 0);//1 draw call for all units
		}
		//}
	}



	//end////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	vkCmdEndRenderPass(commandBuffers[0]);
	vkEndCommandBuffer(commandBuffers[0]);
}

void Renderer::input()
{
	//mouseScroll = 0;
	//windowClass->getMouseScroll(mouseScroll);//mouseScroll = +1/-1 || 0
	//std::cout << mouseScroll << std::endl;


	windowPtr->getMousePos(mousePosX, mousePosY);
	//std::cout << "mouse x: " << mousePosX << " ,mouse y: " << mousePosY << std::endl;
}

void Renderer::run()
{
	windowPtr->centreMouse();//set mouse cursor to centre of screen(width,height)/2
	Sleep(100);
	while (!glfwWindowShouldClose(setupPtr->windowPtr)) {		
		glfwPollEvents();	
		input();//cache mouse pos x,y in render class		
		drawFrame();//main function	
		float fps = windowPtr->getfps();	
		if (windowPtr->isKeyPressed(GLFW_KEY_ESCAPE) | windowPtr->isMousePressed(2)) {
			glfwSetWindowShouldClose(setupPtr->windowPtr, 1);
		}
	}
	vkDeviceWaitIdle(setupPtr->device);
	glfwDestroyWindow(setupPtr->windowPtr);
	glfwTerminate();
}

void Renderer::updateUniformBuffer()
{	
	ubo.model = glm::mat4();
	windowPtr->centreMouse();
	windowPtr->getMousePos(mousePosX, mousePosY);
	ubo.view = gamePtr->fpsCamera(mousePosX, mousePosY, cameraPosition, cameraPoint);

	//gamePtr->moveCamera(mousePosX, mousePosY, cameraPosition, cameraPoint);
	//ubo.view = glm::lookAt(cameraPosition, cameraPoint, glm::vec3(0.0f, 1.0f, 0.0f));//eye,centre,up

	//ubo.view = glm::lookAt(glm::vec3(2, 0, 0), glm::vec3(2, 0, -1), glm::vec3(0, 1, 0));
	ubo.proj = glm::perspective(glm::radians(90.0f), setupPtr->configPtr->windowWidth / (float)setupPtr->configPtr->windowHeight, 0.1f, 1000.0f);
	ubo.proj[1][1] *= -1;// makes the y axis projected to the same as vulkans
	//ubo.lightPosition = cameraPosition;//lightPos;//cameraPosition;
	static light sun;
	sun.pos = { 0,2,-2 };
	sun.colour = glm::vec3(1.0f, 0.0f, 0.0f);
	sun.intensity = glm::vec3(1.0f);
	for (uint32_t i = 0; i < MAX_LIGHTS; i++) {//only shaders first lights inside light vector, sort for closes position todo
		ubo.lightPosition[i] = modelsPtr->unitList[4].pos + glm::vec3(0, 1, 0);
		//ubo.lightPosition[i] = glm::vec3(2, 5, -3);
		ubo.lightIntensity[i] = glm::vec3(1);
		ubo.lightColour[i] = glm::vec3(0.8, 0.5, 0.5);//brg		
		//if (i < gamePtr->lights.size()) {
		//	ubo.lightPosition[i] = gamePtr->lights[i].pos;
		//	ubo.lightColour[i] = gamePtr->lights[i].colour;
		//	ubo.lightIntensity[i] = gamePtr->lights[i].intensity;
		//}
		//else {
		//	//ubo.lightPosition[i] = sun.pos;
		//	//ubo.lightColour[i] = sun.colour;
		//	//ubo.lightIntensity[i] = sun.intensity;
		//}
	}
	ubo.ambient = glm::vec3(0.4f);
	if (!setupPtr->configPtr->lighting) {
		ubo.ambient = glm::vec3(1.0f);
	}
	
	

	void* UBOdata;
	vkMapMemory(setupPtr->device, descriptorsPtr->uniformBufferMemory, 0, sizeof(ubo), 0, &UBOdata);
	memcpy(UBOdata, &ubo, sizeof(ubo));
	vkUnmapMemory(setupPtr->device, descriptorsPtr->uniformBufferMemory);


}



void Renderer::moveUnits()
{
	//mouse point
	double mouseNormalizedX = (2.0f * mousePosX) / setupPtr->configPtr->windowWidth - 1.0f;
	double mouseNormalizedY = 1.0f - (2.0f * mousePosY) / setupPtr->configPtr->windowHeight;
	glm::vec2 mouseNormalized(mouseNormalizedX, -mouseNormalizedY);//vulkan uses -y as up so inverting y axis from cortesian mouse coordinates, can be done in ubo.view[1][1]*=-1
	//printf("x:%f,y:%f\n", mouseNormalized.x, mouseNormalized.y);//cartesian coordinates (0,0 = mid, pos x is right, pos y is up)
	glm::vec3 mouseEye =inverse(ubo.proj) * glm::vec4(mouseNormalized,-1.0f, 1.0f);//4th dimension controls translation, 1=point/position where 0=vector/line
	glm::vec3 mouseWorld = inverse(ubo.view) * glm::vec4(mouseEye, 0.0f);
	mouseWorld = normalize(mouseWorld);
	//printf("x:%f,y:%f,z:%f\n", mouseWorld.x, mouseWorld.y, mouseWorld.z);

	glm::vec3 gradient;
	static int d;
	gradient = mouseWorld;
	if (windowPtr->isMousePressed(1)) {//right click SPAWNS A CUBE INFRONT OF CAMERA
		d++;
		//modelsPtr->unitList[3].move(-modelsPtr->unitList[3].pos);//move back to 0,0
		//modelsPtr->unitList[3].move(cameraPosition);
		//modelsPtr->unitList[3].move(gradient);
		//modelsPtr->unitList[3].v = gradient * glm::vec3(d);
		//printf("%i\n",d);
		if (d == 1) {
			modelsPtr->loadCube();
			//printf("cube created\n");
		}
		modelsPtr->unitList[modelsPtr->unitList.size() - 1].move(-modelsPtr->unitList[modelsPtr->unitList.size() - 1].pos);//move back to 0,0
		modelsPtr->unitList[modelsPtr->unitList.size() - 1].move((cameraPosition + (gradient*glm::vec3(2))));
		modelsPtr->unitList[modelsPtr->unitList.size() - 1].v = gradient * glm::vec3(d);

		for (int i = 1; i < modelsPtr->unitList.size(); i++) {//for each unit, except terrain [0]
			bool collision = false;
			if (math6.raySquareIntersection(gradient, cameraPosition, modelsPtr->unitList[i].hitbox.max, modelsPtr->unitList[i].hitbox.min, modelsPtr->unitList[i].hitbox.max.z,glm::vec3(0,0,1))) {
				collision = true;
			}
			else if (math6.raySquareIntersection(gradient, cameraPosition, modelsPtr->unitList[i].hitbox.max, modelsPtr->unitList[i].hitbox.min, modelsPtr->unitList[i].hitbox.min.z, glm::vec3(0, 0, 1))) {
				collision = true;
			}
			else if (math6.raySquareIntersection(gradient, cameraPosition, modelsPtr->unitList[i].hitbox.max, modelsPtr->unitList[i].hitbox.min, modelsPtr->unitList[i].hitbox.max.x, glm::vec3(1, 0, 0))) {
				collision = true;
			}
			else if (math6.raySquareIntersection(gradient, cameraPosition, modelsPtr->unitList[i].hitbox.max, modelsPtr->unitList[i].hitbox.min, modelsPtr->unitList[i].hitbox.min.x, glm::vec3(1, 0, 0))) {
				collision = true;
			}
			else if (math6.raySquareIntersection(gradient, cameraPosition, modelsPtr->unitList[i].hitbox.max, modelsPtr->unitList[i].hitbox.min, modelsPtr->unitList[i].hitbox.max.y, glm::vec3(0, 1, 0))) {
				collision = true;
			}
			else if (math6.raySquareIntersection(gradient, cameraPosition, modelsPtr->unitList[i].hitbox.max, modelsPtr->unitList[i].hitbox.min, modelsPtr->unitList[i].hitbox.min.y, glm::vec3(0, 1, 0))) {
				collision = true;
			}

			if (collision & !modelsPtr->unitList[i].antiGravity) {
				modelsPtr->unitList[i].v.y += 1.f;
				if (i==2) {
					//printf("pyramid collision\n");
				}
			}
		}
	}
	else {
		d = 0;
	}

	

	glm::vec3 enemy = modelsPtr->unitList[1].pos;
	if (1) {//wasd inputs
		const float n = 144.0f * dt;
		//if (windowPtr->isKeyPressed(GLFW_KEY_W)) {
		//	//modelsPtr->unitList[1].move({ 0.0, 0.0, -n });
		//	modelsPtr->unitList[1].v += glm::vec3(0.0, 0.0, -n);
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_S)) {
		//	//modelsPtr->unitList[1].move({ 0.0, 0.0, n });
		//	modelsPtr->unitList[1].v += glm::vec3(0.0, 0.0, n);
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_A)) {
		//	//modelsPtr->unitList[1].move({ -n, 0.0, 0.0f });
		//	modelsPtr->unitList[1].v += glm::vec3(-n, 0.0, 0);
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_D)) {
		//	//modelsPtr->unitList[1].move({ n, 0.0, 0.0f });
		//	modelsPtr->unitList[1].v += glm::vec3(n, 0.0, 0);
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_Q)) {
		//	//modelsPtr->unitList[1].move({ 0.0,n, 0.0f });
		//	modelsPtr->unitList[1].v += glm::vec3(0.0, n, 0);
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_E)) {
		//	//modelsPtr->unitList[1].move({ 0.0, -n, 0.0f });
		//	modelsPtr->unitList[1].v += glm::vec3(0.0, -n, 0);
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_Z)) {
		//	//modelsPtr->unitList[1].turn(modelsPtr->vertices, -n, 'x');
		//	modelsPtr->unitList[1].v += glm::vec3(0.0, -n,0.0 );
		//}
		//if (windowPtr->isKeyPressed(GLFW_KEY_C)) {
		//	//modelsPtr->unitList[1].turn(modelsPtr->vertices, n, 'x');
		//	modelsPtr->unitList[1].v += glm::vec3(0.0, n,0.0);
		//}

		if (windowPtr->isMousePressed(0)) {//gun ptr cooldown being changed so effects all guns referencing it, 
			/*if (gamePtr->sentryList[0].gunPtr->cooldown <= 0) {
				gamePtr->sentryList[0].gunPtr->cooldown = gamePtr->sentryList[0].gunPtr->fireRate;
				gamePtr->shootSentry(gamePtr->sentryList[0]);
			}*/

			if (gamePtr->entityList[0].gunPtr->cooldown <= 0) {
				gamePtr->entityList[0].gunPtr->cooldown = gamePtr->entityList[0].gunPtr->fireRate;
				gamePtr->shootGun(gamePtr->entityList[0]);
			}
		}
		/*for (uint32_t i = 0; i < gamePtr->sentryList.size(); i++) {
			if (gamePtr->sentryList[0].gunPtr->cooldown > 0) {
				gamePtr->sentryList[0].gunPtr->cooldown -= dt;
			}
		}*/
		for (uint32_t i = 0; i < gamePtr->entityList.size(); i++) {
			if (gamePtr->entityList[0].gunPtr->cooldown > 0) {
				gamePtr->entityList[0].gunPtr->cooldown -= dt;
			}
		}
	}

	gamePtr->aimSentry(gamePtr->sentryList[0], enemy);
	
	//float gunAngleX = modelsPtr->unitList[gamePtr->sentryList[0].gunUnitListIndex].angle.x;
	//std::cout << "gun pitch: " << gunAngleX << "\n";

	/*
	if (windowPtr->isKeyPressed(GLFW_KEY_T)) {
		sonar.frame++;
		if (sonar.frame % 10 == 0) {
			glm::vec2 t1 = glm::vec2(modelsPtr->unitList[n].pos.x, modelsPtr->unitList[n].pos.z);
			glm::vec2 t2 = glm::vec2(modelsPtr->unitList[n+1].pos.x, modelsPtr->unitList[n+1].pos.z);
			glm::vec2 t3 = glm::vec2(modelsPtr->unitList[n+2].pos.x, modelsPtr->unitList[n+2].pos.z);
			std::vector<glm::vec2> tArray;
			tArray.push_back(t1);
			tArray.push_back(t2);
			tArray.push_back(t3);
			float heading = modelsPtr->unitList[gamePtr->sentryList[0].turretUnitListIndex].angle.y;
			//std::cout << heading << std::endl;
			sonar.run(glm::vec2(0), tArray, heading);
			//std::cout << pos2.x << std::endl;
			sonar.frame = 0;
		}
	}
	*/

	//printf("z:3vs1: %f:%f\n", modelsPtr->unitList[3].hitbox.max.z, modelsPtr->unitList[1].hitbox.min.z);
	//printf("velocity: %f,%f,%f\n", modelsPtr->unitList[1].v.x, modelsPtr->unitList[1].v.y, modelsPtr->unitList[1].v.z);

	





	for (uint32_t i = 1; i < modelsPtr->unitList.size(); i++) {	//unitList[0] = terrain
		
		modelsPtr->unitList[i].hitbox.calculateHitbox(modelsPtr->vertices, modelsPtr->unitList[i].unitTypePtr->vertices.size(), modelsPtr->unitList[i].vertexStart);//re-calc max/min
		modelsPtr->unitList[i].hitbox.updateCubeMesh();//re-calc cube vertices from unit max/min

		//modelsPtr->unitList[i].updateFloor(modelsPtr->vertices);//updating unit floors, iterating all unit vertices
		if (!modelsPtr->unitList[i].antiGravity) {//if unit is not antiGravity (resisting gravity)
			physics.force(modelsPtr->unitList[i]);//gravity acting on all units (g/fps
		}
		
		physics.collision(modelsPtr->unitList[i], modelsPtr->unitList[0],true);//check collision with ground unit
		
		
		for (int p = 0; p < gamePtr->entityList[0].gunPtr->ammo; p++) {
			if (i == gamePtr->entityList[0].projectileUnitListIndex + p) {
				physics.collision(modelsPtr->unitList[i], modelsPtr->unitList[3]);//check sentry[0] projectiles against unit 3 
			}
		}
		
		
		

		

		if (windowPtr->isKeyPressed(GLFW_KEY_7)) {//raise units
			if (!modelsPtr->unitList[i].antiGravity) {
				modelsPtr->unitList[i].v.y += 1 + (i * 0.1f);
			}
		}
		else if (windowPtr->isKeyPressed(GLFW_KEY_8)) {
			if (!modelsPtr->unitList[i].antiGravity) {//lower units
				modelsPtr->unitList[i].v.y -= 1 + (i * 0.1f);
			}
		}

		if (windowPtr->isKeyPressed(GLFW_KEY_9)) {//rotate units +
			if (!modelsPtr->unitList[i].antiGravity) {//spawned blocks bugged todo
				modelsPtr->unitList[i].rotation.y += 2 * i;
			}
		}
		else if (windowPtr->isKeyPressed(GLFW_KEY_0)) {//rotate units -
			if (!modelsPtr->unitList[i].antiGravity) {
				modelsPtr->unitList[i].rotation.y -= 1 * i;
			}
		}

		if (windowPtr->isKeyPressed(GLFW_KEY_5)) {//move units back (z+)
			if (!modelsPtr->unitList[i].antiGravity) {
				modelsPtr->unitList[i].v.z += 0.1 * i;
			}
		}

		modelsPtr->unitList[i].move(modelsPtr->unitList[i].v * glm::vec3(dt));//translate all units by their velocity / fps
		modelsPtr->unitList[i].rotation *= glm::vec3(dt);//adjust degree/s rotation speed to fps

		//if (modelsPtr->unitList[i].translation == glm::vec3(0.0f) & modelsPtr->unitList[i].rotation == glm::vec3(0.0f)) {	}//dont transform objects which dont move
		//else {modelsPtr->unitList[i].transform(modelsPtr->vertices);//rotate all units to face forward(-z) at 0,0; then rotate/transform unit along x,y axis
		//}			
	}

	modelsPtr->updateHitboxes();//update vertex buffer position of all hitboxes (1 for each unit, transformed by unit translation)
	modelsPtr->transformUnits();

	if (windowPtr->isKeyPressed(GLFW_KEY_6)) {
		Sleep(100);//slow motion
	}
}
