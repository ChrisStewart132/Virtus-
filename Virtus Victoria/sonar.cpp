#include "sonar.h"
/*REQUIRED
#include "windows.h"
#include <mmsystem.h>//sound
#pragma comment(lib, "winmm.lib")
#include <glm/glm.hpp>	
#include <iostream>
#include <vector>	
*/

void sonar::run(glm::vec2 pos, std::vector<glm::vec2> &targetArray, float heading=0.0f)
{
	//calculate closest target to crosshair
	float angle;
	float closestAngle;
	glm::vec2 target;
	int targetIndex = 0;
	int c[64];//max target 
	for (int i = 0; i < targetArray.size(); i++) {
		angle = (atan2(pos.x - targetArray[i].x, pos.y - targetArray[i].y))*(180.0f / 3.14159265359f);//0 to 180 is turning left from -z axis
		angle -= heading;		
		if (angle < 0) { angle *= -1; }
		c[i] = angle;
	}
	for (int i = 0; i < targetArray.size(); i++) {
		if (i == 0) {
			closestAngle = c[i];
		}
		else {
			if (c[i] < closestAngle) {
				targetIndex = i;
			}
		}
	}
	//std::cout << targetIndex << std::endl;
	//calculate angle between player heading and target pos
	target = targetArray[targetIndex];
	angle = (atan2(pos.x - target.x, pos.y - target.y))*(180.0f / 3.14159265359f);//0 to 180 is turning left from -z axis
	angle -= heading;
	//printf("angle: %f\ ,", angle);
	//play appropriate sound based off delta angle

	int delta1,delta2,key;

	if (angle == 0) { key = 0; }
	else if (angle < 0) {
		if (angle > -5) {
			key = -0;
		}
		else {
			key = 15;
		}
		/*
		else if(angle>-30) {
			key = -45;
		}
		else if (angle > -45) {
			key = -30;
		}
		else if (angle > -60) {
			key = -45;
		}
		else if (angle > -75) {
			key = -60;
		}
		else if (angle > -90) {
			key = -75;
		}
		else {
			key = -90;
		}
		*/
	}
	else {//angle>0
		if (angle < 5) {
			key = 0;
		}
		else {
			key = 15;
		}
		/*
		else if (angle< 30) {
			key = 45;
		}
		else if (angle < 45) {
			key = 30;
		}
		else if (angle < 60) {
			key = 45;
		}
		else if (angle < 75) {
			key = 60;
		}
		else if (angle < 90) {
			key = 75;
		}
		else {
			key = 90;
		}
		*/
	}

	
	//std::cout << "key: " << key << std::endl;

	if (key == 0) {
		PlaySound(TEXT("sonar sounds/0.wav"), NULL, SND_ASYNC);
	}												
	else if (key = 15) {							
		PlaySound(TEXT("sonar sounds/15.wav"), NULL,SND_ASYNC);
	}		

	/*

	else if (key = 30) {							
		PlaySound(TEXT("sonar sounds/30.wav"), NULL,SND_ASYNC);
	}												
	else if (key = 45) {							
		PlaySound(TEXT("sonar sounds/45.wav"), NULL,SND_ASYNC);
	}												
	else if (key = 60) {							
		PlaySound(TEXT("sonar sounds/60.wav"), NULL,SND_ASYNC);
	}												
	else if (key = 75) {						
		PlaySound(TEXT("sonar sounds/75.wav"), NULL,SND_ASYNC);
	}												
	else if (key = 90) {							
		PlaySound(TEXT("sonar sounds/far.wav"), NULL,SND_ASYNC);
	}												
	else if (key = -15){							
		PlaySound(TEXT("sonar sounds/15.wav"), NULL,SND_ASYNC);
	}												
	else if (key = -30){							
		PlaySound(TEXT("sonar sounds/30.wav"), NULL,SND_ASYNC);
	}												
	else if (key = -45){							
		PlaySound(TEXT("sonar sounds/45.wav"), NULL,SND_ASYNC);
	}												
	else if (key = -60){							
		PlaySound(TEXT("sonar sounds/60.wav"), NULL,SND_ASYNC);
	}												
	else if (key = -75){							
		PlaySound(TEXT("sonar sounds/75.wav"), NULL,SND_ASYNC);
	}												
	else if (key = -90){							
		PlaySound(TEXT("sonar sounds/far.wav"), NULL,SND_ASYNC);
	}
	Sleep(1);
	*/
}

sonar::sonar()
{
}


sonar::~sonar()
{
}
