#pragma once
#define STB_IMAGE_IMPLEMENTATION

//TODO
//CAMERA
//CUSTOM PIPELINE CREATION WITH CONFIG STRUCT
//MORE PRECISE HIT DETECTION THAN BOUNDING BOX
//MODEL SPACE SPLIT INTO 3D CUBES, FOR HIT DETECTION
//

//2d map grid system-create grid, sort units into grid, as units move self update grid position, hit detection only checks units in same grid.

//change way sentry turrets are loaded in model class

//functions to rotate turret, rotate gun, aim at a point, shoot single bullet

//test
#include "sonar.h"
//classes
#include "Models.h"
#include "Window.h"
#include "Setup.h"


#include "RenderPass.h"
#include "Pipeline.h"
#include "DescriptorSetLayout.h"
#include "FrameBuffer.h"

#include "Descriptors.h"

#include "Renderer.h"


//structs
#include "config.h"

//vulkan SDK 1.0.77.0
int main() {
	//notes
	//todo
	//drag equation (using velocity vector and relevant surface area/drag coef
	//visualised hitboxes need correcting: triangles only visible from certain orientations (+x e.g.)
	//rotation dynamics
	
	//hitboxes, rotate hitboxes in transform, collision detection, correct unit COM, construct class (combined COM), torque/moments


	config simpleConfig;
	simpleConfig.windowWidth = 1920.0f/1.2f;
	simpleConfig.windowHeight = 1061.0f / 1.2f;
	simpleConfig.viewWidth = simpleConfig.windowWidth;
	simpleConfig.viewHeight = simpleConfig.windowHeight;
	simpleConfig.swapChainCount = 1;//not implemented
	simpleConfig.lighting = 1;//frag colour green??1
	simpleConfig.textures = 0;
	simpleConfig.mip = true;
	simpleConfig.antialiasing = true;//must be false if msaa=1
	simpleConfig.msaaSamples = VK_SAMPLE_COUNT_8_BIT;
	simpleConfig.samepleShading = true;

	Window virtusWindow(&simpleConfig);
	Setup setup(virtusWindow.getWindow(), &simpleConfig);
	
	//specify unit types
	const uint32_t unitTypeCount = 6;//update descriptors.h unittypecount manually
	unitType unitTypeArray[unitTypeCount];

	unitTypeArray[0].name = "terrain";
	unitTypeArray[0].modelPath = "Models/" + unitTypeArray[0].name + ".obj";
	unitTypeArray[0].texturePath = "Textures/" + unitTypeArray[0].name + ".jpg";
	unitTypeArray[0].textureIndex = 0;
	unitTypeArray[0].mass = 9999999999999;
	
	unitTypeArray[1].name = "20mm round";
	unitTypeArray[1].modelPath = "Models/" + unitTypeArray[1].name + ".obj";
	unitTypeArray[1].texturePath = "Textures/" + unitTypeArray[1].name + ".jpg";
	unitTypeArray[1].textureIndex = 1;
	unitTypeArray[1].mass = 0.1f;

	unitTypeArray[2].name = "20mm gun";
	unitTypeArray[2].modelPath = "Models/" + unitTypeArray[2].name + ".obj";
	unitTypeArray[2].texturePath = "Textures/" + unitTypeArray[2].name + ".jpg";
	unitTypeArray[2].textureIndex = 2;
	unitTypeArray[2].mass = 5;
	
	unitTypeArray[3].name = "turret";
	unitTypeArray[3].modelPath = "Models/" + unitTypeArray[3].name + ".obj";
	unitTypeArray[3].texturePath = "Textures/" + unitTypeArray[3].name + ".jpg";
	unitTypeArray[3].textureIndex = 3;
	unitTypeArray[3].mass = 10;

	unitTypeArray[4].name = "pyramid";
	unitTypeArray[4].modelPath = "Models/" + unitTypeArray[4].name + ".obj";
	unitTypeArray[4].texturePath = "Textures/" + unitTypeArray[4].name + ".jpg";
	unitTypeArray[4].textureIndex = 4;
	unitTypeArray[4].mass = 999999;

	unitTypeArray[5].name = "t62";
	unitTypeArray[5].modelPath = "Models/" + unitTypeArray[5].name + ".obj";
	unitTypeArray[5].texturePath = "Textures/" + unitTypeArray[5].name + ".jpg";
	unitTypeArray[5].textureIndex = 5;
	unitTypeArray[5].mass = 30000;

	//load all models into unitTypes and load main vertex buffer with units
	Models models(unitTypeArray, unitTypeCount);	
	Game game(&virtusWindow, &models);
	game.createSentry();
	models.transformUnits();
	models.loadHitboxes();
	//render starting screen
	//create new config with specified settings
	RenderPass renderPass(&setup);	
	DescriptorSetLayout descriptorSetLayout(&setup,&models);

	std::vector<Pipeline> pipelineVector;
	pipelineInfo pipelineFillInfo;
	if (!simpleConfig.lighting) {
		pipelineFillInfo.fragShader = "frag2";
		pipelineFillInfo.vertShader = "vert2";
	}
	Pipeline pipelineFill(&setup, &descriptorSetLayout, &renderPass, pipelineFillInfo);
	pipelineVector.push_back(pipelineFill);

	pipelineInfo pipelineLineInfo;
	pipelineLineInfo.polygonMode = VK_POLYGON_MODE_LINE;
	Pipeline pipelineLine(&setup, &descriptorSetLayout, &renderPass, pipelineLineInfo);
	pipelineVector.push_back(pipelineLine);
	
	pipelineInfo pipelinePointInfo;
	pipelinePointInfo.polygonMode = VK_POLYGON_MODE_POINT;
	Pipeline pipelinePoint(&setup, &descriptorSetLayout, &renderPass, pipelinePointInfo);
	pipelineVector.push_back(pipelinePoint);

	Descriptors descriptors(&setup, &models, &descriptorSetLayout);//requires unit type count specified in header(for number of textures)
	FrameBuffer frameBuffer(&setup, &renderPass);
	Renderer renderer(&setup, &descriptors, &renderPass, &frameBuffer, pipelineVector.data(), &models, &virtusWindow, &game);
	renderer.run();
	renderer.destroy();
	frameBuffer.destroy();
	descriptors.destroy();

	pipelineFill.destroy();
	pipelineLine.destroy();
	pipelinePoint.destroy();

	descriptorSetLayout.destroy();
	renderPass.destroy();
	setup.destroy();
	return 0;
}