#pragma once

#include "windows.h"
#include <mmsystem.h>//sound
#pragma comment(lib, "winmm.lib")
#include <glm/glm.hpp>	
#include <iostream>
#include <vector>	

class sonar
{
public:
	int frame = 0;
	void run(glm::vec2 pos, std::vector<glm::vec2> &targetArray, float heading);
	sonar();
	~sonar();
};

