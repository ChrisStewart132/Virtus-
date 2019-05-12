#pragma once
#include "unit.h"
#include "glm\glm.hpp"
#include <algorithm>//std::min

class Physics
{
private:
	float g = 9.81f/144.0f;//gravity m/s2
	float densityAir = 1.225f;//kg/m^3

	float kineticEnergy(float _m, float _v);
	float momentum(float _m, float _v);
	glm::vec2 quadratic(float _a, float _b, float _c);
public:
	Physics();
	~Physics();

	void force(unit& _unit);
	void collision(unit& _unit, unit& _unit2,bool groundCollision=false);
};

