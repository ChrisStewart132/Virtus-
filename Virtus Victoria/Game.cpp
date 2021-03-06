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
	g20mm.ammo = 1;
	g20mm.fireRate = 0.3f;
	g20mm.muzzleVelocity = 72;
	g20mm.barrel = glm::vec3(0.05f, -0.031f, -0.251f);
	g20mm.mountingPoint = glm::vec3(0.0f);//gun models 0,0,0 position = mounting position
	gunTypeList.push_back(g20mm);

	//load turret types
	turret t20mm;
	t20mm.horizontalRotationPoint = glm::vec3(0.0f);
	t20mm.rotationSpeed = glm::vec3(100.0f);
	t20mm.mountingPoint = glm::vec3(0.0f);
	t20mm.gunMountingPoint = glm::vec3(0.2f, 1.9f, 0.0f);
	turretTypeList.push_back(t20mm);

	//lights (max 4 renderable)
	float c = 0.1f;
	float intensityTemp = 0.05f;
	glm::vec3 posTemp = glm::vec3(0, 2, 0);
	light light1;
	light1.pos = posTemp;
	light1.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(intensityTemp);
	createLight(light1);
	light light2;
	light2.pos = posTemp;
	light2.colour = glm::vec3(1, c, c);
	light1.intensity = glm::vec3(intensityTemp);
	createLight(light2);
	light light3;
	light3.pos = posTemp;
	light3.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(intensityTemp);
	createLight(light3);
	light light4;
	light4.pos = posTemp;
	light4.colour = glm::vec3(c, c, c);
	light1.intensity = glm::vec3(intensityTemp);
	createLight(light4);

}

void Game::createLight(light _light) {
	lights.push_back(_light);
}



void Game::createGun()
{
	glm::vec3 offset = glm::vec3(0.5f, 1.5, 10.f);
	//entity player;
	player.gunPtr = &gunTypeList[0];
	modelsPtr->loadModel("20mm gun");
	player.gunUnitListIndex = modelsPtr->unitList.size() - 1;
	modelsPtr->unitList[player.gunUnitListIndex].antiGravity = true;
	modelsPtr->unitList[player.gunUnitListIndex].move(offset);

	for (uint32_t i = 0; i < player.gunPtr->ammo; i++) {
		modelsPtr->loadModel("20mm round");
		if (i == 0) {
			player.projectileUnitListIndex = modelsPtr->unitList.size() - 1;
		}
		modelsPtr->unitList[player.projectileUnitListIndex + i].move(offset);
		modelsPtr->unitList[player.projectileUnitListIndex + i].antiGravity = true;
	}

	entityList.push_back(player);

}

void Game::shootGun(entity _entity)
{
	for (int i = 0; i < _entity.gunPtr->ammo; i++) {
		if (modelsPtr->unitList[_entity.projectileUnitListIndex + i].v == glm::vec3(0.0f)) {//todo need to specify available ammo
			modelsPtr->unitList[_entity.projectileUnitListIndex + i].v = modelsPtr->unitList[_entity.projectileUnitListIndex + i].getDirection() * glm::vec3(_entity.gunPtr->muzzleVelocity);
			modelsPtr->unitList[_entity.projectileUnitListIndex + i].antiGravity = false;
			break;
		}
	}
}

void Game::createSentry()
{//unit pointers from model class not transferring to sentryTypeList!
	sentry sentry;
	sentry.turretPtr = &turretTypeList[0];
	sentry.gunPtr = &gunTypeList[0];
	sentry.projectilePtr = &projectileTypeList[0];

	modelsPtr->loadModel("turret");
	//unit* turretUnitPtr = modelsPtr->unitList.data();
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
	
	glm::vec3 projectileOffset = sentry.turretPtr->gunMountingPoint+glm::vec3(0,0,-0.8F)+sentry.gunPtr->barrel;//sentry.gunPtr->barrel
	for (uint32_t i = 0; i < sentry.gunPtr->ammo; i++) {
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].move(projectileOffset);
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos = modelsPtr->unitList[sentry.gunUnitListIndex + i].pos - sentry.turretPtr->gunMountingPoint;
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos.y += sentry.turretPtr->gunMountingPoint.y;
		modelsPtr->unitList[sentry.projectileUnitListIndex + i].yawPos.z -= -0.8F;
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
			if (modelsPtr->unitList[_sentry.projectileUnitListIndex + i].v == glm::vec3(0.0f)) {//todo need to specify available ammo
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
	int width = 1920;
	int height = 1080;
	float CameraSpeedFactor = 1000.0f;//1000-0
	float cameraAreaPercentage = 10.0f;
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
	//todo control cameraPosition movement through physics not here (only control cameraPoint relative to cameraPosition)
	const glm::vec3 up = glm::vec3(0, 1, 0);
	const glm::vec3 right = glm::vec3(1, 0, 0);
	const glm::vec3 forward = glm::vec3(0, 0, -1);
	static float roll = 0, pitch = 0, yaw = 0;//camera bearing
	float yRot, xRot;
	glm::vec3 position = cameraPosition;

	

	//move camera back to 0,0 and default orientation
	glm::mat4 t = glm::translate(glm::mat4(1.0f), -position);
	glm::mat4 x = glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), right);
	glm::mat4 y = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), up);
	glm::mat4 matrix = x*y;//roation matrix
	cameraPoint = t * glm::vec4(cameraPoint, 1.0f);//point-pos, 0,0,-1
	cameraPoint = matrix * glm::vec4(cameraPoint, 1.0f);
	//move camera back to 0,0 and default orientation

	///////move gun
	for (int i = modelsPtr->unitList[player.gunUnitListIndex].vertexStart; i < modelsPtr->unitList[player.gunUnitListIndex].vertexStart + modelsPtr->unitList[player.gunUnitListIndex].unitTypePtr->vertices.size(); i++) {
		modelsPtr->vertices[i].pos = t * glm::vec4(modelsPtr->vertices[i].pos, 1.0f);
		modelsPtr->vertices[i].pos = matrix * glm::vec4(modelsPtr->vertices[i].pos, 1.0f);
	}
	//move ammo in gun
	for (int i = 0; i < player.gunPtr->ammo;i++) {
		if (modelsPtr->unitList[player.projectileUnitListIndex + i].antiGravity) {
			for (int j = modelsPtr->unitList[player.projectileUnitListIndex + i].vertexStart; j < modelsPtr->unitList[player.projectileUnitListIndex + i].vertexStart + modelsPtr->unitList[player.projectileUnitListIndex + i].unitTypePtr->vertices.size(); j++) {
				modelsPtr->vertices[j].pos = t * glm::vec4(modelsPtr->vertices[j].pos, 1.0f);
				modelsPtr->vertices[j].pos = matrix * glm::vec4(modelsPtr->vertices[j].pos, 1.0f);
			}
		}
	}
	////////

	yRot = ((float(windowPtr->getWidth()) / 2) - mousePosX) / 10;
	xRot = ((float(windowPtr->getHeight()) / 2) - mousePosY-0.5) / 10;

	//printf("orientation : %f = %f\n", pitch, xRot);

	if (xRot + pitch >= 80) {
		xRot = 0;
	}else if(xRot + pitch <= -80) {
		xRot = 0;
	}

	static bool jumping;
	static int jumpTimer;
	if (jumping) {
		jumpTimer += 1;
		if (jumpTimer > (80)) {
			jumpTimer = 0;
			jumping = false;
		}
	}
	float dx = 0;
	float dy = 0;
	float dz = 0;
	const float strafeFactor = 0.05f;
	if (windowPtr->isKeyPressed(GLFW_KEY_W) | windowPtr->isMousePressed(4)) {
		dz -= 1 * strafeFactor;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_S) | windowPtr->isMousePressed(3)) {
		dz += 1 * strafeFactor;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_A)) {
		dx -= 1 * strafeFactor;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_D)) {
		dx += 1 * strafeFactor;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_SPACE) && !jumping) {
		dy += 2;
		jumping = true;
	}
	if (windowPtr->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
		dy -= 1 * strafeFactor;
	}
	glm::vec3 fwd(0, 0, dz);
	glm::vec3 side(dx, 0, 0);
	glm::vec3 vertical(0, dy, 0);

	y = glm::rotate(glm::mat4(1.0f), glm::radians(yaw + yRot), up);
	x = glm::rotate(glm::mat4(1.0f), glm::radians(pitch + xRot), right);
	t = glm::translate(glm::mat4(1.0f), position);
	matrix = y*x;//roation matrix
	fwd = matrix * glm::vec4(fwd, 1.0f);
	side = matrix * glm::vec4(side, 1.0f);
	vertical = matrix * glm::vec4(vertical, 1.0f);
	cameraPoint = matrix * glm::vec4(cameraPoint, 1.0f);//rotate point+
	cameraPoint = t * glm::vec4(cameraPoint, 1.0f);

	///////
	for (int i = modelsPtr->unitList[player.gunUnitListIndex].vertexStart; i < modelsPtr->unitList[player.gunUnitListIndex].vertexStart + modelsPtr->unitList[player.gunUnitListIndex].unitTypePtr->vertices.size(); i++) {
		modelsPtr->vertices[i].pos = matrix * glm::vec4(modelsPtr->vertices[i].pos, 1.0f);
		modelsPtr->vertices[i].pos = t * glm::vec4(modelsPtr->vertices[i].pos, 1.0f);
		modelsPtr->vertices[i].pos += fwd + side + vertical;
		modelsPtr->vertices[i].pos.y -= fwd.y + side.y + vertical.y;
	}
	for (int i = 0; i < player.gunPtr->ammo; i++) {
		if (modelsPtr->unitList[player.projectileUnitListIndex + i].antiGravity) {
			for (int j = modelsPtr->unitList[player.projectileUnitListIndex + i].vertexStart; j < modelsPtr->unitList[player.projectileUnitListIndex + i].vertexStart + modelsPtr->unitList[player.projectileUnitListIndex + i].unitTypePtr->vertices.size(); j++) {
				modelsPtr->vertices[j].pos = matrix * glm::vec4(modelsPtr->vertices[j].pos, 1.0f);
				modelsPtr->vertices[j].pos = t * glm::vec4(modelsPtr->vertices[j].pos, 1.0f);
				modelsPtr->vertices[j].pos += fwd + side + vertical;
				modelsPtr->vertices[j].pos.y -= fwd.y + side.y + vertical.y;
			}
		}
	}
	////////

	cameraPoint += fwd + side + vertical;
	cameraPoint.y -= fwd.y + side.y;
	cameraPosition += fwd + side + vertical;
	cameraPosition.y -= fwd.y + side.y;
	if (cameraPosition.y > 1.86f) {
		cameraPoint.y -= 9.81f / 144.f;
		cameraPosition.y -= 9.81f / 144.f;
	}
	if (cameraPosition.y < 1.86f) {
		cameraPoint.y += 1.86f - cameraPosition.y;
		cameraPosition.y += 1.86f - cameraPosition.y;
	}


	yaw += yRot;
	pitch += xRot;

	if (yaw > 360.0f) { yaw -= 360.0f; }
	if (pitch > 80) { pitch -= 80; }
	if (yaw < -360.0f) { yaw += 360.0f; }
	if (pitch < -80) { pitch += 80; }

	for (int i = 0; i < player.gunPtr->ammo; i++) {
		modelsPtr->unitList[player.projectileUnitListIndex + i].angle.y = yaw;
		modelsPtr->unitList[player.projectileUnitListIndex + i].angle.x = pitch;
	}
	modelsPtr->unitList[player.gunUnitListIndex].angle.y = yaw;
	modelsPtr->unitList[player.gunUnitListIndex].angle.x = pitch;




	//modelsPtr->unitList[4].move(-modelsPtr->unitList[4].pos);
	//modelsPtr->unitList[4].move(cameraPosition + glm::vec3(0.3f, -0.2f, -0.2f));





	windowPtr->centreMouse();
	return glm::lookAt(cameraPosition, cameraPoint, glm::vec3(0.0f, 1.0f, 0.0f));
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
