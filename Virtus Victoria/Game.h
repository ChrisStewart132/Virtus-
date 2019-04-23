#pragma once
#include "unit.h"
#include <glm/glm.hpp>						//vectors mats
#include <glm/gtc/matrix_transform.hpp>		//transformation matrix
#include <iostream>
#include <math.h>
#include <vector>
#include "Window.h"
#include "Models.h"
#include "Descriptors.h"

class Game
{
private:
	Window* windowPtr;
	Models* modelsPtr;

	bool rotatingCamera = false;
	glm::vec2 cameraClickedPos;
	glm::vec2 cameraAngle = { 0.0f, 0.0f };

	
public:
	std::vector<light> lights;
	void createLight(light _light);

	std::vector<projectile> projectileTypeList;
	std::vector<gun> gunTypeList;
	std::vector<turret> turretTypeList;
	std::vector<sentry> sentryTypeList;

	std::vector<sentry> sentryList;
	void createSentry();
	void aimSentry(sentry _sentry, glm::vec3 _enemy);
	void shootSentry(sentry _sentry);


	void rotate(glm::vec3& _vector, float _angle, char axis, glm::vec3 centreDistance = { 0.0f,0.0f,0.0f });
	void moveCamera(double mousePosX, double mousePosY, glm::vec3 &cameraPosition, glm::vec3 &cameraPoint);
	glm::mat4 fpsCamera(double mousePosX, double mousePosY, glm::vec3 &cameraPosition, glm::vec3 &cameraPoint);
	float calculateAngle(glm::vec2 v1, glm::vec2 v2);
	Game(Window* _windowPtr, Models* _modelptr);
	~Game();
};

