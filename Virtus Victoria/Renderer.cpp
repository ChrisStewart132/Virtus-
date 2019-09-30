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
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr->graphicsPipeline);
		VkDeviceSize offsets[] = { 0 };


		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, &descriptorsPtr->vertexBuffer, offsets);
		//vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr->pipelineLayout, 0, 1, descriptorsPtr->descriptorSets, 0, nullptr);
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

void Renderer::update()
{
	//start acquire swapchain image done in run function
	//update called in middle of run();
	//record command buffer vkBeginCommandBuffer
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = nullptr; // Optional
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

	moveUnits();//change vertex data
				//update and bind ubo
	updateUniformBuffer();//vkmapMemory-ubo object-vkunmapMemory
						  //bind vertex data
	vkBindBufferMemory(setupPtr->device, descriptorsPtr->vertexBuffer, descriptorsPtr->vertexBufferMemory, 0);
	size_t bufferSize = sizeof(modelsPtr->vertices[0]) * modelsPtr->vertices.size();
	void* data;
	vkMapMemory(setupPtr->device, descriptorsPtr->vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, modelsPtr->vertices.data(), bufferSize);
	vkUnmapMemory(setupPtr->device, descriptorsPtr->vertexBufferMemory);
	//choose pipeline
	int pipelineIndex;
	if (windowPtr->isKeyPressed(GLFW_KEY_1)) {
		pipelineIndex = 1;
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);//line
	}
	else if (windowPtr->isKeyPressed(GLFW_KEY_2)) {
		pipelineIndex = 2;
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);//point
	}
	else {
		pipelineIndex = 0;
		vkCmdBindPipeline(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].graphicsPipeline);//fill
	}
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffers[0], 0, 1, &descriptorsPtr->vertexBuffer, offsets);
	//vkCmdBindIndexBuffer(commandBuffers[0], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	uint32_t hitboxVertexCount = modelsPtr->unitList.size() * 36;//every unit automatically has a 36 vertex hitbox rendered 
	textureInfo texturePushConstant;
	vkCmdBindDescriptorSets(commandBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelinePtr[pipelineIndex].pipelineLayout, 0, 1, &descriptorsPtr->descriptorSets[0], 0, nullptr);
	if (setupPtr->configPtr->textures) {
		for (uint32_t i = 0; i < modelsPtr->unitTypeList.size(); i++) {
			texturePushConstant.textureIndex = i;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT,0,sizeof(texturePushConstant),&texturePushConstant);
			//vkCmdDrawIndexed(commandBuffers[0], indices.size(), 1, 0, 0, 0);
			for (uint32_t j = 0; j < modelsPtr->unitList.size(); j++) {//for every single unit loaded
				if (modelsPtr->unitList[j].unitTypePtr->textureIndex == i) {
					vkCmdDraw(commandBuffers[0], modelsPtr->unitList[j].unitTypePtr->vertices.size(), 1, modelsPtr->unitList[j].vertexStart, 0);//for every texture, 1 draw call per unit loaded
				}
			}
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_1)) {
			texturePushConstant.textureIndex = 4;
			vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT,0,sizeof(texturePushConstant),&texturePushConstant);
			vkCmdDraw(commandBuffers[0], hitboxVertexCount, 1, modelsPtr->vertices.size()-hitboxVertexCount, 0);//for every texture, 1 draw call per unit loaded
		}		
	}
	else {//
		texturePushConstant.textureIndex = 4;
		vkCmdPushConstants(commandBuffers[0], pipelinePtr[pipelineIndex].pipelineLayout,VK_SHADER_STAGE_VERTEX_BIT,0,sizeof(texturePushConstant),&texturePushConstant);
		
		if (windowPtr->isKeyPressed(GLFW_KEY_1)) {
			hitboxVertexCount = 0;
		}
		vkCmdDraw(commandBuffers[0], modelsPtr->vertices.size()-hitboxVertexCount, 1, 0, 0);//for every texture, 1 draw call per unit loaded	
	}
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
	while (!glfwWindowShouldClose(setupPtr->windowPtr)) {		
		glfwPollEvents();	
		input();		
		drawFrame();	
		//float fps = windowPtr->getfps();
		//printf("fps: %f\n", fps);
	}
	vkDeviceWaitIdle(setupPtr->device);
	glfwDestroyWindow(setupPtr->windowPtr);
	glfwTerminate();
}

void Renderer::updateUniformBuffer()
{	
	ubo.model = glm::mat4();
	gamePtr->moveCamera(mousePosX, mousePosY, cameraPosition, cameraPoint);
	//ubo.view = glm::lookAt(cameraPosition, cameraPoint, glm::vec3(0.0f, 1.0f, 0.0f));//eye,centre,up
	ubo.view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	ubo.proj = glm::perspective(glm::radians(70.0f), setupPtr->configPtr->windowWidth / (float)setupPtr->configPtr->windowHeight, 0.1f, 1000.0f);
	ubo.proj[1][1] *= -1;// makes the y axis projected to the same as vulkans
	//ubo.lightPosition = cameraPosition;//lightPos;//cameraPosition;
	light sun;
	sun.pos = { 200,200,-200 };
	sun.colour = glm::vec3(0.0f, 0.0f, 0.5f);
	sun.intensity = glm::vec3(0.5f);
	for (uint32_t i = 0; i < MAX_LIGHTS; i++) {//only shaders first lights inside light vector, sort for closes position todo
		if (i < gamePtr->lights.size()-1) {
			ubo.lightPosition[i] = gamePtr->lights[i].pos;
			ubo.lightColour[i] = gamePtr->lights[i].colour;
			ubo.lightIntensity[i] = gamePtr->lights[i].intensity;
		}
		else {
			ubo.lightPosition[i] = sun.pos;
			ubo.lightColour[i] = sun.colour;
			ubo.lightIntensity[i] = sun.intensity;
		}
	}
	ubo.ambient = glm::vec3(0.5f);
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
	glm::vec2 mouseNormalized(mouseNormalizedX, mouseNormalizedY);
	//printf("x:%f,y:%f\n", mouseNormalized.x, mouseNormalized.y);//cartesian coordinates (0,0 = mid, pos x is right, pos y is up)
	glm::vec3 mouseEye =inverse(ubo.proj) * glm::vec4(mouseNormalized,-1.0f, 1.0f);
	//ubo.view = glm::mat4(1.0f);
	ubo.view[1][1] *= -1;//vulkan -y is up, this is corrected here and in ubo update function
	glm::vec3 mouseWorld = inverse(ubo.view) * glm::vec4(mouseEye, 1.0f);
	mouseWorld = normalize(mouseWorld);

	printf("x:%f,y:%f,z:%f\n", mouseWorld.x, mouseWorld.y, mouseWorld.z);
	glm::vec3 test;
	test = mouseWorld * glm::vec3(2);
	if (windowPtr->isMousePressed(1)) {
		modelsPtr->unitList[1].move(-modelsPtr->unitList[1].pos);//move back to 0,0
		modelsPtr->unitList[1].move(test);
	}

	//double fps = windowPtr->getfps();

	glm::vec3 enemy = modelsPtr->unitList[1].pos;
	if (1) {//wasd inputs
		const float n = 60.0f * dt;
		if (windowPtr->isKeyPressed(GLFW_KEY_W)) {
			//modelsPtr->unitList[1].move({ 0.0, 0.0, -n });
			modelsPtr->unitList[1].v += glm::vec3(0.0, 0.0, -n);
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_S)) {
			//modelsPtr->unitList[1].move({ 0.0, 0.0, n });
			modelsPtr->unitList[1].v += glm::vec3(0.0, 0.0, n);
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_A)) {
			//modelsPtr->unitList[1].move({ -n, 0.0, 0.0f });
			modelsPtr->unitList[1].v += glm::vec3(-n, 0.0, 0);
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_D)) {
			//modelsPtr->unitList[1].move({ n, 0.0, 0.0f });
			modelsPtr->unitList[1].v += glm::vec3(n, 0.0, 0);
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_Q)) {
			//modelsPtr->unitList[1].move({ 0.0,n, 0.0f });
			modelsPtr->unitList[1].v += glm::vec3(0.0, n, 0);
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_E)) {
			//modelsPtr->unitList[1].move({ 0.0, -n, 0.0f });
			modelsPtr->unitList[1].v += glm::vec3(0.0, -n, 0);
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_Z)) {
			//modelsPtr->unitList[1].turn(modelsPtr->vertices, -n, 'x');
			modelsPtr->unitList[1].v += glm::vec3(0.0, -n,0.0 );
		}
		if (windowPtr->isKeyPressed(GLFW_KEY_C)) {
			//modelsPtr->unitList[1].turn(modelsPtr->vertices, n, 'x');
			modelsPtr->unitList[1].v += glm::vec3(0.0, n,0.0);
		}

		if (windowPtr->isKeyPressed(GLFW_KEY_SPACE)) {
			if (gamePtr->sentryList[0].gunPtr->cooldown <= 0) {
				gamePtr->sentryList[0].gunPtr->cooldown = gamePtr->sentryList[0].gunPtr->fireRate;
				gamePtr->shootSentry(gamePtr->sentryList[0]);
			}

		}
		for (uint32_t i = 0; i < gamePtr->sentryList.size(); i++) {
			if (gamePtr->sentryList[0].gunPtr->cooldown > 0) {
				gamePtr->sentryList[0].gunPtr->cooldown -= dt;
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
		if (i == 1) {//bullet
			//physics.collision(modelsPtr->unitList[i], modelsPtr->unitList[3]);//t62 and wasd bullet
		}

		for (int p = 0; p < gamePtr->sentryList[0].gunPtr->ammo;p++) {
			if (i == gamePtr->sentryList[0].projectileUnitListIndex+p) {
				//physics.collision(modelsPtr->unitList[i], modelsPtr->unitList[3]);//check sentry[0] projectiles against t62 
			}
		}
		for (uint32_t j = 1; j < modelsPtr->unitList.size(); j++) {
			if (i != j) {
				//physics.collision(modelsPtr->unitList[i], modelsPtr->unitList[j]);
			}
		}

		if (i != 3) {
			//physics.collision(modelsPtr->unitList[i], modelsPtr->unitList[3]);//check collision with t62
		}

		if (windowPtr->isKeyPressed(GLFW_KEY_7)) {
			if (!modelsPtr->unitList[i].antiGravity) {
				modelsPtr->unitList[i].v.y += 1 + (i * 0.1f);
			}
		}
		else if (windowPtr->isKeyPressed(GLFW_KEY_8)) {
			if (!modelsPtr->unitList[i].antiGravity) {
				modelsPtr->unitList[i].v.y -= 1 + (i * 0.1f);
			}
		}

		modelsPtr->unitList[i].move(modelsPtr->unitList[i].v * glm::vec3(dt));//translate all units by their velocity / fps
		modelsPtr->unitList[i].rotation *= glm::vec3(dt);//adjust degree/s rotation speed to fps

		//if (modelsPtr->unitList[i].translation == glm::vec3(0.0f) & modelsPtr->unitList[i].rotation == glm::vec3(0.0f)) {	}//dont transform objects which dont move
		//else {modelsPtr->unitList[i].transform(modelsPtr->vertices);//rotate all units to face forward(-z) at 0,0; then rotate/transform unit along x,y axis
		//}			
	}

	//printf("floor:%f\n", modelsPtr->unitList[1].hitbox.min.y);

	modelsPtr->updateHitboxes();//update vertex buffer position of all hitboxes (1 for each unit, transformed by unit translation)
	modelsPtr->transformUnits();

	if (windowPtr->isKeyPressed(GLFW_KEY_6)) {
		Sleep(20);//slow motion
	}
}
