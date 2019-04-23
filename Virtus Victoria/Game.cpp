#include "Game.h"



Game::Game(Window* _windowPtr, Models* _modelsptr)
{
	windowPtr = _windowPtr;
	modelsPtr = _modelsptr;

	//load projectile types
	projectile p20mm;
	p20mm.drag = 0.02f;
	projectileTypeList.push_back(p20mm);

	//load gun types
	gun g20mm;
	g20mm.ammo = 100;
	g20mm.fireRate = 0.2f;
	g20mm.barrel = glm::vec3(0.05f, -0.031f, -0.251f);
	g20mm.mountingPoint = glm::vec3(0.0f);//gun models 0,0,0 position = mounting position
	gunTypeList.push_back(g20mm);

	//load turret types
	turret t20mm;
	t20mm.horizontalRotationPoint = glm::vec3(0.0f);
	t20mm.rotationSpeed = glm::vec3(30.0f);
	t20mm.mountingPoint = glm::vec3(0.0f);
	t20mm.gunMountingPoint = glm::vec3(0.2f, 1.9f, 0.0f);
	turretTypeList.push_back(t20mm);

	//lights (max 4 renderable)
	float c = 0.3f;
	light light1;
	light1.pos = glm::vec3(100.0f, 300.0f, -100.0f);
	light1.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(1.0f);
	createLight(light1);
	light light2;
	light2.pos = glm::vec3(-100.0f, 100.0f, 0.0f);
	light2.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(1.0f);
	createLight(light2);
	light light3;
	light3.pos = glm::vec3(0.0f, 100.0f, -100.0f);
	light3.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(1.0f);
	createLight(light3);
	light light4;
	light4.pos = glm::vec3(0.0f, 100.0f, 100.0f);
	light4.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(1.0f);
	createLight(light4);
}

void Game::createLight(light _light) {
	lights.push_back(_light);
}

void Game::createSentry()
{//unit pointers from model class not transferring to sentryTypeList!
	sentry sentry;
	sentry.turretPtr = &turretTypeList[0];
	sentry.gunPtr = &gunTypeList[0];
	sentry.projectilePtr = &projectileTypeList[0];

	modelsPtr->loadModel("turret");
	unit* turretUnitPtr = modelsPtr->unitList.data();
	sentry.turretUnitListIndex = modelsPtr->unitList.size() - 1;//1

	modelsPtr->loadModel("20mm gun");
	sentry.gunUnitListIndex = modelsPtr->unitList.size() - 1;//2
	modelsPtr->unitList[sentry.gunUnitListIndex].antiGravity = true;

	for (uint32_t i = 0; i < sentry.gunPtr->ammo; i++) {
		modelsPtr->loadModel("20mm round");	
		if (i == 0) {
			sentry.projectileUnitListIndex = modelsPtr->unitList.size() - 1;//3-32
		}
		modelsPtr->unitList[sentry.projectileUnitListIndex+i].antiGravity = true;
	}	

	modelsPtr->unitList[sentry.gunUnitListIndex].move(sentry.turretPtr->gunMountingPoint);//move gun to turret mounting point
	modelsPtr->unitList[sentry.gunUnitListIndex].yawPos = modelsPtr->unitList[sentry.gunUnitListIndex].pos - sentry.turretPtr->gunMountingPoint;//set yaw pos as centre of turret
	modelsPtr->unitList[sentry.gunUnitListIndex].yawPos.y += sentry.turretPtr->gunMountingPoint.y;//raise yawPos to height of gun mount (for pitch calc)
	
	glm::vec3 projectileOffset = sentry.turretPtr->gunMountingPoint+glm::vec3(0,0,-1)+sentry.gunPtr->barrel;//sentry.gunPtr->barrel
	for (uint32_t i = 0; i < sentry.gunPtr->ammo; i++) {
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].move(projectileOffset);
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos = modelsPtr->unitList[sentry.gunUnitListIndex + i].pos - sentry.turretPtr->gunMountingPoint;
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos.y += sentry.turretPtr->gunMountingPoint.y;
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos.z -= -1;
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos -= sentry.gunPtr->barrel;
	}
	
	sentryList.push_back(sentry);
}

void Game::aimSentry(sentry _sentry, glm::vec3 _enemy)
{
	modelsPtr->unitList[_sentry.turretUnitListIndex].aim(modelsPtr->vertices, _enemy, { 0,1,0 },_sentry.turretPtr->rotationSpeed.y);//aim turret
	modelsPtr->unitList[_sentry.gunUnitListIndex].aim(modelsPtr->vertices, _enemy, { 1,1,0 }, _sentry.turretPtr->rotationSpeed.y);//aim gun
	for (uint32_t i = 0; i < _sentry.gunPtr->ammo; i++) {
		if (modelsPtr->unitList[_sentry.projectileUnitListIndex + i].v.z==0) {//if projectile not live rotate with barrel
			modelsPtr->unitList[_sentry.projectileUnitListIndex + i].aim(modelsPtr->vertices, _enemy, { 1,1,0 }, _sentry.turretPtr->rotationSpeed.y);//aim projectiles
		}
	}
																																  /*
	glm::vec3 gunRotation;
	gunRotation.y = modelsPtr->unitList[_sentry.turretUnitListIndex].euler.y- modelsPtr->unitList[_sentry.gunUnitListIndex].euler.y;
	if (gunRotation.y > 0) {
		gunRotation.y = _sentry.turretPtr->rotationSpeed.y*-1;
	}
	else if (gunRotation.y < 0) {
		gunRotation.y = _sentry.turretPtr->rotationSpeed.y;
	}
	
	modelsPtr->unitList[_sentry.gunUnitListIndex].turn(modelsPtr->vertices, gunRotation.y , 'y', modelsPtr->unitList[_sentry.turretUnitListIndex].pos);
	for (uint32_t i = 0; i < _sentry.gunPtr->ammo; i++) {
		modelsPtr->unitList[_sentry.projectileUnitListIndex+i].turn(modelsPtr->vertices, gunRotation.y, 'y', modelsPtr->unitList[_sentry.turretUnitListIndex].pos);
	}
	*/
	
}

void Game::shootSentry(sentry _sentry)
{
	for (int i = 0; i < _sentry.gunPtr->ammo; i++) {	
			if (modelsPtr->unitList[_sentry.projectileUnitListIndex + i].v == glm::vec3(0.0f)) {
				modelsPtr->unitList[_sentry.projectileUnitListIndex + i].v = modelsPtr->unitList[_sentry.projectileUnitListIndex + i].getDirection()*glm::vec3(100.0f);
				modelsPtr->unitList[_sentry.projectileUnitListIndex + i].antiGravity = false;
				break;
			}
	}
}

void Game::rotate(glm::vec3& _vector, float _angle, char axis, glm::vec3 centreDistance)
{
	float angle;
	angle = _angle*(3.14159265359f / 180.0f);//3.14 radians = 180degrees
	glm::mat3 cwMatX(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, cos(angle), -sin(angle)), glm::vec3(0.0f, sin(angle), cos(angle)));//rotate around x
	glm::mat3 cwMatY(glm::vec3(cos(angle), 0.0f, sin(angle)), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-sin(angle), 0.0f, cos(angle)));//rotate around y
	glm::mat3 cwMatZ(glm::vec3(cos(angle), sin(angle), 0.0f), glm::vec3(-sin(angle), cos(angle), 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotate around z

	_vector -= centreDistance;
	if (axis == 'x' || axis == 'X') {
		_vector = cwMatX * _vector;
	}
	else if (axis == 'y' || axis == 'Y') {
		_vector = cwMatY * _vector;
	}
	else if (axis == 'z' || axis == 'Z') {
		_vector = cwMatZ * _vector;
	}
	else {
		//std::cout << "rotate: invalid axis\n";
	}
	_vector += centreDistance;
}

void Game::moveCamera(double mousePosX, double mousePosY, glm::vec3 &cameraPosition, glm::vec3 &cameraPoint)
{
	float width = 1920;
	float height = 1080;
	float CameraSpeedFactor = 1000.0f;//1000-0
	float cameraAreaPercentage = 8.0f;
	float wasdSpeed = 0.0f;
	if (windowPtr->isMousePressed(GLFW_MOUSE_BUTTON_3)) {
		if (!rotatingCamera) {
			rotatingCamera = true;
			cameraClickedPos.x = mousePosX;
			cameraClickedPos.y = mousePosY;
		}
		glm::vec2 angle = { 0.0f,0.0f };
		angle.x = (mousePosX - cameraClickedPos.x) / 100.0f;
		angle.y = (mousePosY - cameraClickedPos.y) / 100.0f;


		cameraAngle.y = 90 - atan((cameraPosition.z - cameraPoint.z) / (cameraPosition.y - cameraPoint.y)) * (180.0f / 3.141592f);
		//std::cout << cameraAngle.y << std::endl;
		if (cameraPosition.z >= cameraPoint.z) {
			if (cameraAngle.y < 10.0f) {
				if (angle.y > 0.0f) {
					rotate(cameraPosition, angle.y, 'x', cameraPoint);
				}
			}
			else if (cameraAngle.y > 80.0) {
				if (angle.y < 0.0f) {
					rotate(cameraPosition, angle.y, 'x', cameraPoint);
				}
			}
			else {
				rotate(cameraPosition, angle.y, 'x', cameraPoint);
			}
		}
		else if (cameraPosition.z < cameraPoint.z) {
			if (cameraAngle.y > 170.0f) {
				if (angle.y > 0.0f) {
					rotate(cameraPosition, -angle.y, 'x', cameraPoint);
				}
			}
			else if (cameraAngle.y < 100.0) {
				if (angle.y < 0.0f) {
					rotate(cameraPosition, -angle.y, 'x', cameraPoint);
				}
			}
			else {
				rotate(cameraPosition, -angle.y, 'x', cameraPoint);
			}
		}
		rotate(cameraPosition, angle.x, 'y', cameraPoint);
	}
	else {
		rotatingCamera = false;

		//calc camera translate direction
		float deltaX, deltaZ;
		deltaX = (cameraPosition.x - cameraPoint.x);
		deltaZ = (cameraPosition.z - cameraPoint.z);
		float magnitude;
		if (deltaX > 0 && deltaZ > 0) {
			magnitude = deltaX + deltaZ;
		}
		else if (deltaX < 0 && deltaZ>0) {
			magnitude = -deltaX + deltaZ;
		}
		else if (deltaZ < 0 && deltaX > 0) {
			magnitude = deltaX - deltaZ;
		}
		else if (deltaX < 0 && deltaZ < 0) {
			magnitude = -(deltaX + deltaZ);
		}
		float gradient = deltaZ / deltaX;
		float xSpeed = 0.0f;
		float zSpeed = 0.0f;
		float topSpeed = 0.0f;
		if (!isinf(gradient)) {
			if (mousePosX < (width / cameraAreaPercentage) || windowPtr->isKeyPressed(GLFW_KEY_A)) {//left
				topSpeed = ((mousePosX - (width / cameraAreaPercentage)) / CameraSpeedFactor);
				if (windowPtr->isKeyPressed(GLFW_KEY_A)) {
					topSpeed = -wasdSpeed / CameraSpeedFactor;
				}
				xSpeed = (topSpeed / magnitude)*deltaZ;
				zSpeed = -(topSpeed / magnitude)*deltaX;
			}
			else if (mousePosX >(width - (width / cameraAreaPercentage)) || windowPtr->isKeyPressed(GLFW_KEY_D)) {//right
				topSpeed = (mousePosX - (width - (width / cameraAreaPercentage))) / CameraSpeedFactor;
				if (windowPtr->isKeyPressed(GLFW_KEY_D)) {
					topSpeed = wasdSpeed / CameraSpeedFactor;
				}
				xSpeed = (topSpeed / magnitude)*deltaZ;
				zSpeed = -(topSpeed / magnitude)*deltaX;
			}
			cameraPosition.x += xSpeed;
			cameraPoint.x += xSpeed;
			cameraPosition.z += zSpeed;
			cameraPoint.z += zSpeed;
			if (mousePosY < (height / (cameraAreaPercentage/2.0f)) || windowPtr->isKeyPressed(GLFW_KEY_W)) {//up
				topSpeed = (mousePosY - (height / (cameraAreaPercentage / 2.0f))) / CameraSpeedFactor;
				if (windowPtr->isKeyPressed(GLFW_KEY_W)) {
					topSpeed = -wasdSpeed / CameraSpeedFactor;
				}
				xSpeed = (topSpeed / (magnitude))*deltaX;
				zSpeed = (topSpeed / (magnitude))*deltaZ;

			}
			else if (mousePosY >(height - (height / (cameraAreaPercentage / 2.0f))) || windowPtr->isKeyPressed(GLFW_KEY_S)) {//down
				topSpeed = (mousePosY - (height - (height / (cameraAreaPercentage / 2.0f)))) / CameraSpeedFactor;
				if (windowPtr->isKeyPressed(GLFW_KEY_S)) {
					topSpeed = wasdSpeed / CameraSpeedFactor;
				}
				xSpeed = (topSpeed / (magnitude))*deltaX;
				zSpeed = (topSpeed / (magnitude))*deltaZ;
			}
			cameraPosition.x += xSpeed;
			cameraPoint.x += xSpeed;
			cameraPosition.z += zSpeed;
			cameraPoint.z += zSpeed;
		}
		else {
			if (mousePosX < (width / cameraAreaPercentage) || windowPtr->isKeyPressed(GLFW_KEY_A)) {//left
				if (windowPtr->isKeyPressed(GLFW_KEY_A)) {
					xSpeed = -wasdSpeed / CameraSpeedFactor;
				}
				else {
					xSpeed = ((mousePosX - (width / cameraAreaPercentage)) / CameraSpeedFactor);
				}
			}
			else if (mousePosX >(width - (width / cameraAreaPercentage)) || windowPtr->isKeyPressed(GLFW_KEY_D)) {//right
				if (windowPtr->isKeyPressed(GLFW_KEY_D)) {
					xSpeed = wasdSpeed / CameraSpeedFactor;
				}
				else {
					xSpeed = (mousePosX - (width - (width / cameraAreaPercentage))) / CameraSpeedFactor;
				}
			}
			cameraPosition.x += xSpeed;
			cameraPoint.x += xSpeed;
			if (mousePosY < (height / (cameraAreaPercentage / 2.0f)) || windowPtr->isKeyPressed(GLFW_KEY_W)) {//up
				if (windowPtr->isKeyPressed(GLFW_KEY_W)) {
					zSpeed = -wasdSpeed / CameraSpeedFactor;
				}
				else {
					zSpeed = (mousePosY - (height / (cameraAreaPercentage / 2.0f))) / CameraSpeedFactor;
				}
			}
			else if (mousePosY >(height - (height / (cameraAreaPercentage / 2.0f))) || windowPtr->isKeyPressed(GLFW_KEY_S)) {//down
				if (windowPtr->isKeyPressed(GLFW_KEY_S)) {
					zSpeed = wasdSpeed / CameraSpeedFactor;
				}
				else {
					zSpeed = (mousePosY - (height - (height / (cameraAreaPercentage / 2.0f)))) / CameraSpeedFactor;
				}
			}
			cameraPosition.z += zSpeed;
			cameraPoint.z += zSpeed;
		}

		//zooming
		if (windowPtr->isKeyPressed(GLFW_KEY_Q)) {//in
			//cameraPosition.z += 1.2f;
			//cameraPosition.y += 0.2f;
		}
		else if (windowPtr->isKeyPressed(GLFW_KEY_E)) {//out
			//cameraPosition.z -= 1.2f;
			//cameraPosition.y -= 0.2f;
		}
	}	
}

glm::mat4 Game::fpsCamera(double mousePosX, double mousePosY, glm::vec3 &cameraPosition, glm::vec3 &cameraPoint)
{
	//global values
	float roll, pitch, yaw;
	glm::vec3 eyeVector;
	glm::mat4 viewMatrix;
	//global values

	float dx = 0;
	float dz = 0;
	if (windowPtr->isKeyPressed(GLFW_KEY_W)) {
		dz = -2;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_S)) {
		dz = 2;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_A)) {
		dz = -2;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_D)) {
		dz = 2;
	}
	
	return glm::mat4(0);
}

float Game::calculateAngle(glm::vec2 v1, glm::vec2 v2)
{
	float angle;
	/*
	glm::vec3 datum = v2 - v1;

	if (datum.x == 0) {
		if (datum.z == 0) {
			angle.x = 0;
		}
		else if (datum.z > 0) {
			angle.x = 0.0f;
		}
		else {
			angle.x = 180.0f;
		}
	}
	else if (datum.z == 0) {
		if (datum.x == 0) {
			angle.x = 0;
		}
		else if (datum.x > 0) {
			angle.x = 90.0f;
		}
		else {
			angle.x = -90.0f;
		}
	}
	else {
		if (datum.x > 0) {

		}
		else if (datum.z > 0) {

		}
	}
	*/
	
	angle = (atan2(v1.x - v2.x, v1.y - v2.y))*(180.0f / 3.14159265359f);

	/*
	float magnitude1, magnitude2, dotProduct;
	magnitude1 = sqrt((pow(v1.x,2) + pow(v1.y,2)));
	magnitude2 = sqrt((pow(v2.x, 2) + pow(v2.y, 2)));
	dotProduct = v1.x*v2.x + v2.y*v1.y;
	angle = acos(dotProduct / (magnitude1*magnitude2));
	*/
	return angle;
}

Game::~Game()
{
}
