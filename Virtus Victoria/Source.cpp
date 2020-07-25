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
	bool hasTexture=false;
};

//vulkan SDK 1.0.77.0
int main() {
	//notes
	//todo
	//drag equation (using velocity vector and relevant surface area/drag coef
	//visualised hitboxes need correcting: triangles only visible from certain orientations (+x e.g.)
	//rotation dynamics
	
	//hitboxes, rotate hitboxes in transform, collision detection, correct unit COM, construct class (combined COM), torque/moments

	//readBMP("Untitled.bmp");

	config simpleConfig;

	simpleConfig.windowWidth = 2560.0/1.f;
	simpleConfig.windowHeight = 1421.0 / 1.f;
	simpleConfig.viewWidth = 100;// simpleConfig.windowWidth / 1000.1f;
	simpleConfig.viewHeight = 100;// simpleConfig.windowHeight / 1000.1f;

	simpleConfig.swapChainCount = 1;//not implemented

	simpleConfig.lighting = 1;//frag colour green??1

	simpleConfig.textures = 1;
	simpleConfig.mip = 1;

	simpleConfig.antialiasing = 1;//must be false if msaa=1
	simpleConfig.msaaSamples = VK_SAMPLE_COUNT_8_BIT;//1,2,4,8
	simpleConfig.samepleShading = 1;

	simpleConfig.transparentWindow = 0;
	simpleConfig.windowTransparency = 0.9f;

	//simpleConfig.multipleViewports = 1;

	simpleConfig.check();

	Window virtusWindow(&simpleConfig);
	Setup setup(virtusWindow.getWindow(), &simpleConfig);
	
	//specify unit types
	const uint32_t unitTypeCount = 9;//update descriptors.h unittypecount manually and frag shader tex sampler array
	unitType unitTypeArray[unitTypeCount];
	std::vector<asset> assets;
	assets.push_back({ "terrain",99999999.0f,true });
	assets.push_back({ "20mm round",0.1f });
	assets.push_back({ "20mm gun",5 });
	assets.push_back({ "turret",10 });
	assets.push_back({ "pyramid",999999 ,true });
	assets.push_back({ "t62",30000 });
	assets.push_back({ "t-90a",50000 });
	assets.push_back({ "a4",13500 });
	assets.push_back({ "ak47",10 });


	for (int i = 0; i < unitTypeCount; i++) {
		unitTypeArray[i].name = assets[i].name;
		unitTypeArray[i].modelPath = "Models/" + unitTypeArray[i].name + ".obj";
		if (assets[i].hasTexture) {
			unitTypeArray[i].texturePath = "Textures/" + unitTypeArray[i].name + ".jpg";
		}
		else {
			unitTypeArray[i].texturePath = "Textures/default.jpg";
		}	
		unitTypeArray[i].textureIndex = i;
		unitTypeArray[i].mass = assets[i].mass;
	}

	//load all models into unitTypes and load main vertex buffer with units
	Models models(unitTypeArray, unitTypeCount);	
	Game game(&virtusWindow, &models);

	game.createSentry();
	game.createGun();
	models.transformUnits();
	models.loadHitboxes();

	//todo
	//render starting screen
	//create new config with specified settings
	//todo

	RenderPass renderPass(&setup);	
	DescriptorSetLayout descriptorSetLayout(&setup,&models);

	std::vector<Pipeline> pipelineVector;

	pipelineInfo pipelineFillInfo;
	pipelineFillInfo.polygonMode = VK_POLYGON_MODE_FILL;
	if (!simpleConfig.lighting) {
		pipelineFillInfo.fragShader = "frag2";
		pipelineFillInfo.vertShader = "vert2";
	}
	Pipeline pipelineFill(&setup, &descriptorSetLayout, &renderPass, pipelineFillInfo);
	pipelineVector.push_back(pipelineFill);

	pipelineInfo pipelineLineInfo;
	pipelineLineInfo.polygonMode = VK_POLYGON_MODE_LINE;
	if (!simpleConfig.lighting) {
		pipelineLineInfo.fragShader = "frag2";
		pipelineLineInfo.vertShader = "vert2";
	}
	Pipeline pipelineLine(&setup, &descriptorSetLayout, &renderPass, pipelineLineInfo);
	pipelineVector.push_back(pipelineLine);
	
	pipelineInfo pipelinePointInfo;
	pipelinePointInfo.polygonMode = VK_POLYGON_MODE_POINT;
	if (!simpleConfig.lighting) {
		pipelinePointInfo.fragShader = "frag2";
		pipelinePointInfo.vertShader = "vert2";
	}
	Pipeline pipelinePoint(&setup, &descriptorSetLayout, &renderPass, pipelinePointInfo);
	pipelineVector.push_back(pipelinePoint);

	pipelineInfo pipelineInfo2;
	pipelineInfo2.polygonMode = VK_POLYGON_MODE_LINE;
	pipelineInfo2.colours = VK_COLOR_COMPONENT_G_BIT;
	pipelineInfo2.rasterLineWidth = 3.f;
	if (!simpleConfig.lighting) {
		pipelineInfo2.fragShader = "frag2";
		pipelineInfo2.vertShader = "vert2";
	}
	Pipeline pipelineFill2(&setup, &descriptorSetLayout, &renderPass, pipelineInfo2);
	pipelineVector.push_back(pipelineFill2);

	pipelineInfo hitboxPipelineInfo;
	hitboxPipelineInfo.polygonMode = VK_POLYGON_MODE_LINE;
	hitboxPipelineInfo.colours = VK_COLOR_COMPONENT_G_BIT;
	hitboxPipelineInfo.rasterLineWidth = 0.666f;
	if (!simpleConfig.lighting) {
		hitboxPipelineInfo.fragShader = "frag2";
		hitboxPipelineInfo.vertShader = "vert2";
	}
	Pipeline hitboxPipeline(&setup, &descriptorSetLayout, &renderPass, hitboxPipelineInfo);
	pipelineVector.push_back(hitboxPipeline);




	Descriptors descriptors(&setup, &models, &descriptorSetLayout);//requires unit type count specified in header(for number of textures),same with frag shader (tex sampler count)
	FrameBuffer frameBuffer(&setup, &renderPass);
	Renderer renderer(&setup, &descriptors, &renderPass, &frameBuffer, pipelineVector.data(), &models, &virtusWindow, &game);
	renderer.run();




	//closing
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