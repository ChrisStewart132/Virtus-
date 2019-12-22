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

//
#include "CRS.h"

//asset importing
struct asset {
	const char* name;
	float mass;
};

//vulkan SDK 1.0.77.0
int main() {
	//notes
	//todo
	//drag equation (using velocity vector and relevant surface area/drag coef
	//visualised hitboxes need correcting: triangles only visible from certain orientations (+x e.g.)
	//rotation dynamics
	
	//hitboxes, rotate hitboxes in transform, collision detection, correct unit COM, construct class (combined COM), torque/moments

	readBMP("Untitled.bmp");


	config simpleConfig;
	simpleConfig.windowWidth = 1920.0f/1.f;
	simpleConfig.windowHeight = 1061.0f / 1.f;
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
	const uint32_t unitTypeCount = 6;//update descriptors.h unittypecount manually and frag shader tex sampler array
	unitType unitTypeArray[unitTypeCount];
	std::vector<asset> assets;
	assets.push_back({ "terrain",99999999.0f });
	assets.push_back({ "20mm round",0.1f });
	assets.push_back({ "20mm gun",5 });
	assets.push_back({ "turret",10 });
	assets.push_back({ "pyramid",999999 });
	assets.push_back({ "t62",30000 });
	for (int i = 0; i < unitTypeCount; i++) {
		unitTypeArray[i].name = assets[i].name;
		unitTypeArray[i].modelPath = "Models/" + unitTypeArray[i].name + ".obj";
		unitTypeArray[i].texturePath = "Textures/" + unitTypeArray[i].name + ".jpg";
		unitTypeArray[i].textureIndex = i;
		unitTypeArray[i].mass = assets[i].mass;
	}

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

	Descriptors descriptors(&setup, &models, &descriptorSetLayout);//requires unit type count specified in header(for number of textures),same with frag shader (tex sampler count)
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