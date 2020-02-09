#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "Vertex.h"
#include <glm/glm.hpp>		
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
//optomisations: hitbox functions seperate



struct hitbox {
	glm::vec3 max, min;
	glm::vec3 cubeVertexColour = { 1.0f,1.0f,1.0f };
	std::vector<Vertex> cubeVertices;

	void createCubeMesh() {//create 36 vertices, only call once!
		//36 vertices (cube) each nhitbox
		for (int j = 0; j < 18; j++) {
			Vertex vertex;
			if (j == 0) {//
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 1) {
				vertex.pos = { max.x,min.y,max.z };
			}
			else if (j == 2) {
				vertex.pos = { max.x,max.y,max.z };
			}				//				
			else if (j == 3) {
				vertex.pos = { max.x,max.y,max.z };
			}
			else if (j == 4) {
				vertex.pos = { min.x,max.y,max.z };
			}
			else if (j == 5) {
				vertex.pos = { min.x,min.y,max.z };
			}//						 		
			else if (j == 6) {
				vertex.pos = { max.x,min.y,max.z };
			}
			else if (j == 7) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 8) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 9) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 10) {
				vertex.pos = { max.x,max.y,max.z };
			}
			else if (j == 11) {
				vertex.pos = { max.x,min.y,max.z };
			}//						 		
			else if (j == 12) {
				vertex.pos = { min.x,max.y,max.z };
			}
			else if (j == 13) {
				vertex.pos = { max.x,max.y,max.z };
			}
			else if (j == 14) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 15) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 16) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 17) {
				vertex.pos = { min.x,max.y,max.z };
			}
			vertex.colour = cubeVertexColour;
			vertex.texCoord = { 0.0f,0.0f };
			vertex.normal = { 0.5f,0.5f,0.5f };
			cubeVertices.push_back(vertex);
		}
		for (int j = 35; j > 17; j--) {
			Vertex vertex;
			if (j == 18) {//
				vertex.pos = { min.x,min.y,min.z };
			}
			else if (j == 19) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 20) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 21) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 22) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 23) {
				vertex.pos = { min.x,min.y,min.z };
			}//								
			else if (j == 24) {
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 25) {
				vertex.pos = { min.x,min.y,min.z };
			}
			else if (j == 26) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 27) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 28) {
				vertex.pos = { min.x,max.y,max.z };
			}
			else if (j == 29) {
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 30) {
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 31) {
				vertex.pos = { max.x,min.y,max.z };
			}
			else if (j == 32) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 33) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 34) {
				vertex.pos = { min.x,min.y,min.z };
			}
			else if (j == 35) {
				vertex.pos = { min.x,min.y,max.z };
			}
			vertex.colour = cubeVertexColour;
			vertex.texCoord = { 0.0f,0.0f };
			vertex.normal = { 0.5f,0.5f,0.5f };
			cubeVertices.push_back(vertex);
		}
	}

	void updateCubeMesh() {
		//36 vertices (cube) each nhitbox
		for (int j = 0; j < 18; j++) {
			Vertex vertex;
			if (j == 0) {//
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 1) {
				vertex.pos = { max.x,min.y,max.z };
			}
			else if (j == 2) {
				vertex.pos = { max.x,max.y,max.z };
			}				//				
			else if (j == 3) {
				vertex.pos = { max.x,max.y,max.z };
			}
			else if (j == 4) {
				vertex.pos = { min.x,max.y,max.z };
			}
			else if (j == 5) {
				vertex.pos = { min.x,min.y,max.z };
			}//						 		
			else if (j == 6) {
				vertex.pos = { max.x,min.y,max.z };
			}
			else if (j == 7) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 8) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 9) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 10) {
				vertex.pos = { max.x,max.y,max.z };
			}
			else if (j == 11) {
				vertex.pos = { max.x,min.y,max.z };
			}//						 		
			else if (j == 12) {
				vertex.pos = { min.x,max.y,max.z };
			}
			else if (j == 13) {
				vertex.pos = { max.x,max.y,max.z };
			}
			else if (j == 14) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 15) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 16) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 17) {
				vertex.pos = { min.x,max.y,max.z };
			}
			vertex.colour = cubeVertexColour;
			vertex.texCoord = { 0.0f,0.0f };
			vertex.normal = { 0.5f,0.5f,0.5f };
			cubeVertices[j] = vertex;
		}
		for (int j = 35; j > 17; j--) {
			Vertex vertex;
			if (j == 18) {//
				vertex.pos = { min.x,min.y,min.z };
			}
			else if (j == 19) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 20) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 21) {
				vertex.pos = { max.x,max.y,min.z };
			}
			else if (j == 22) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 23) {
				vertex.pos = { min.x,min.y,min.z };
			}//								
			else if (j == 24) {
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 25) {
				vertex.pos = { min.x,min.y,min.z };
			}
			else if (j == 26) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 27) {
				vertex.pos = { min.x,max.y,min.z };
			}
			else if (j == 28) {
				vertex.pos = { min.x,max.y,max.z };
			}
			else if (j == 29) {
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 30) {
				vertex.pos = { min.x,min.y,max.z };
			}
			else if (j == 31) {
				vertex.pos = { max.x,min.y,max.z };
			}
			else if (j == 32) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 33) {
				vertex.pos = { max.x,min.y,min.z };
			}
			else if (j == 34) {
				vertex.pos = { min.x,min.y,min.z };
			}
			else if (j == 35) {
				vertex.pos = { min.x,min.y,max.z };
			}
			vertex.colour = cubeVertexColour;
			vertex.texCoord = { 0.0f,0.0f };
			vertex.normal = { 0.5f,0.5f,0.5f };
			cubeVertices[j] = vertex;
		}
	}

	void calculateHitbox(std::vector<Vertex>& vertices,uint32_t _vertexCount, uint32_t _vertexStart = 0)	{//updates max and min vectors based off iterating all unit vertices (using unitTypePtr default orientation)
		for (uint32_t u = _vertexStart; u < _vertexStart + _vertexCount; u++) {//calc unit max and min x,y square face
			if (u == _vertexStart) {
				max.x = vertices[u].pos.x;
				min.x = vertices[u].pos.x;
				max.y = vertices[u].pos.y;
				min.y = vertices[u].pos.y;
				max.z = vertices[u].pos.z;
				min.z = vertices[u].pos.z;
			}
			else {
				if (vertices[u].pos.x < min.x) { min.x = vertices[u].pos.x; }
				if (vertices[u].pos.x > max.x) { max.x = vertices[u].pos.x; }
				if (vertices[u].pos.y < min.y) { min.y = vertices[u].pos.y; }
				if (vertices[u].pos.y > max.y) { max.y = vertices[u].pos.y; }
				if (vertices[u].pos.z < min.z) { min.z = vertices[u].pos.z; }
				if (vertices[u].pos.z > max.z) { max.z = vertices[u].pos.z; }
			}
		}
	}

	bool hitboxCollision(hitbox & box1, hitbox & box2)
	{
		if (box1.max.y > box2.min.y && box1.min.y < box2.max.y) {
			if (box1.max.z > box2.min.z && box1.min.z < box2.max.z) {
				if (box1.max.x > box2.min.x && box1.min.x < box2.max.x) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	bool pointInside(glm::vec3 point)
	{
		if (max.y > point.y && min.y < point.y) {
			if (max.z > point.z && min.z < point.z) {
				if (max.x > point.x && min.x < point.x) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	void moveCubeVertices(glm::vec3 _translation) {
		for (int i = 0; i < cubeVertices.size(); i++) {
			cubeVertices[i].pos += _translation;
		}
		max += _translation;
		min += _translation;
	}
};

struct unitType {	
	std::string name;
	std::string modelPath;
	std::string texturePath;
	uint32_t textureIndex;
	std::vector<Vertex> vertices;
	glm::vec3 COM = { 0.0f,0.0f,0.0f };
	float mass;
	//hitbox hitbox;
	unitType() {
		//hitbox.max = glm::vec3(0.0f);
		//hitbox.min = glm::vec3(0.0f);
		//hitbox.calculateHitbox(vertices);
	}
	~unitType() {}

};

class unit {
private:
	static void rotate(glm::vec3& _vector, float _angle, char axis, glm::vec3 centreDistance)
	{
		float angle;
		angle = _angle*(3.14159265359f / 180.0f);//3.14 radians = 180degrees
	
		_vector -= centreDistance;
		if (axis == 'x' || axis == 'X') {
			glm::mat3 cwMatX(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, cos(angle), -sin(angle)), glm::vec3(0.0f, sin(angle), cos(angle)));//rotate around x
			_vector = cwMatX * _vector;
		}
		else if (axis == 'y' || axis == 'Y') {
			glm::mat3 cwMatY(glm::vec3(cos(angle), 0.0f, sin(angle)), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-sin(angle), 0.0f, cos(angle)));//rotate around y
			_vector = cwMatY * _vector;
		}
		else if (axis == 'z' || axis == 'Z') {
			glm::mat3 cwMatZ(glm::vec3(cos(angle), sin(angle), 0.0f), glm::vec3(-sin(angle), cos(angle), 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));//rotate around z
			_vector = cwMatZ * _vector;
		}
		else {
			//std::cout << "rotate: invalid axis\n";
		}
		_vector += centreDistance;
	}

	static glm::vec3 calculateAngle(glm::vec3 v1, glm::vec3 v2) {
		//calculate angle between unit and target (subtract units existing euler angles
		glm::vec3 angle;
		//angle.x = (atan2(v1.x - v2.x, v1.z - v2.z))*(180.0f / 3.14159265359f);//0 to 180 is turning left from -z axis
		//angle.y = (atan2(v1.y - v2.y, v1.z - v2.z))*(180.0f / 3.14159265359f);//0 to 180 is down from z axis
		//angle.z = (atan2(v1.z - v2.z, v1.x - v2.x))*(180.0f / 3.14159265359f);//roll
		angle.y = (atan2(v1.x - v2.x, v1.z - v2.z))*(180.0f / 3.14159265359f);//0 to 180 is turning left from -z axis
		angle.x = -(atan2(v1.y - v2.y, v1.z - v2.z))*(180.0f / 3.14159265359f);//0 to 180 is down from z axis
		angle.z = (atan2(v1.z - v2.z, v1.x - v2.x))*(180.0f / 3.14159265359f);//roll
		return angle;
	}

public:
	float calcMagnitude(float value) {
		float mag = value;
		if (mag < 0) { mag *= -1; }
		return mag;
	}

	float magnitude(glm::vec3 v) {
		float mag = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		return mag;
	}

	unitType* unitTypePtr;
	uint32_t vertexStart;
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 floor = pos;
	glm::vec3 angle = glm::vec3(0);
	const glm::vec3 up = glm::vec3(0, 1, 0);
	const glm::vec3 right = glm::vec3(1, 0, 0);
	const glm::vec3 forward = glm::vec3(0, 0, -1);
	const glm::vec3 scale = glm::vec3(1);
	glm::vec3 translation = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0);
	glm::mat4 matrix = glm::mat4(1.0f);
	glm::vec3 pitchPos = glm::vec3(0);
	glm::vec3 yawPos = glm::vec3(0);//gun offset from turret base (0,0) used in transform and aim calculation
	//glm::vec3 rollPos = glm::vec3(0);
	//physics
	float m;//mass kg
	//float radius = 0.1f;//radius m
	bool stationary = false;
	bool antiGravity = false;
	//glm::vec3 a = glm::vec3(0);
	glm::vec3 v = glm::vec3(0);//velocity
	glm::vec3 w = glm::vec3(0);//angular velocity
	//glm::vec3 aw = glm::vec3(0);//angular acceleration
	hitbox hitbox;
	unit(unitType* _unitTypePtr, uint32_t _vertexStart) {
		unitTypePtr = _unitTypePtr;
		vertexStart = _vertexStart;
		pos = unitTypePtr->COM;
		//hitbox = unitTypePtr->hitbox;
		hitbox.calculateHitbox(unitTypePtr->vertices, unitTypePtr->vertices.size());
		hitbox.createCubeMesh();
		m = unitTypePtr->mass;
	}
	~unit() {	}

	glm::vec3 getDirection() {//returns direction vector for forward velocity movement
		glm::vec3 dir = forward;
		glm::mat4 yaw = glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), up);
		glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), right);
		dir = yaw * pitch * glm::vec4(dir, 1.0f);
		return dir;
	}

	void updateFloor(std::vector<Vertex> &_vertices) {
		for (uint32_t i = vertexStart; i < vertexStart + unitTypePtr->vertices.size(); i++) {
			if (i == vertexStart) {
				floor = _vertices[i].pos;
			}
			else {
				if (_vertices[i].pos.y < floor.y) {
					floor = _vertices[i].pos;
				}
			}
		}
		//return floor;
	}

	void move(glm::vec3 _move, bool local=false) {
		if (!local) {
			translation += _move;
		}
		else {
			translation += (_move*angle);
		}		
	}

	void turn(std::vector<Vertex> &_vertices, float _angle, char axis, glm::vec3 centreDistance = { 0.0f,0.0f,0.0f }) {
		if (axis == 'x') {
			rotation.x += _angle;
		}
		if (axis == 'y') {
			rotation.y += _angle;
		}
		if (axis == 'z') {
			rotation.z += _angle;
		}	
	}

	void aim(std::vector<Vertex> &_vertices,glm::vec3 _target,glm::vec3 _axes,float _turnRate) {
		glm::vec3 A;
		//A = calculateAngle(pos+pitchPos, _target);	
		A = calculateAngle(pos + yawPos, _target);
		float mr = 0.5f;
		float tr = _turnRate;
		if (_axes.x == 1) {//pitch calc not working as intended
			if (angle.y > 90 || angle.y < -90) {
				A.x += angle.x;
			}
			else {
				A.x -= angle.x;
			}
				if (A.x > 180.0f) { A.x -= 360.0f; }//170--179=349, 
				else if (A.x < -180.0f) { A.x += 360.0f; }//-170-179

				float pitch = calcMagnitude(A.x);
				if (pitch < mr) {}//if A(rotation) is less than mr (minimum rotation) no aiming

				else if (A.x > 0) { rotation.x += tr; }//pos+ rotation ACW
				else if (A.x < 0) { rotation.x -= tr; }		
		}

		A = calculateAngle(pos + yawPos, _target);
		if (_axes.y == 1) {//yaw	
				A.y -= angle.y;
				if (A.y > 180.0f) { A.y -= 360.0f; }
				else if (A.y < -180.0f) { A.y += 360.0f; }

				float yaw = calcMagnitude(A.y);
				if (yaw < mr) {}
				else if (A.y > 0) { rotation.y += tr; }
				else if (A.y < 0) { rotation.y -= tr; }		
		}
		//if (_axes.z == 1) {}
	}
	
	void transform(std::vector<Vertex> &_vertices) {	
		//return mesh back to original orientation (negative angle and translation matrix)
		glm::mat4 t = glm::translate(glm::mat4(1.0f), -pos- yawPos);
		glm::mat4 y = glm::rotate(glm::mat4(1.0f), glm::radians(-angle.y), up);
		glm::mat4 x = glm::rotate(glm::mat4(1.0f), glm::radians(-angle.x), right);
		//glm::mat4 z = glm::rotate(glm::mat4(1.0f), glm::radians(-angle.z), forward);
		matrix = x*y;	
		for (uint32_t i = vertexStart; i < vertexStart + unitTypePtr->vertices.size(); i++) {
			_vertices[i].pos = t * glm::vec4(_vertices[i].pos, 1.0f);//move to 0,0,0
			_vertices[i].pos = matrix * glm::vec4(_vertices[i].pos, 1.0f);//rotate to 0,0,0	
		}
		//transform mesh back to previous orientation + added rotation/translation
		glm::mat4 newTransfrom = glm::mat4(1.0f);
		glm::mat4 tr = glm::translate(glm::mat4(1.0f), pos+translation + yawPos);
		glm::mat4 ry = glm::rotate(glm::mat4(1.0f), glm::radians(angle.y + rotation.y), up);	
		glm::mat4 rx = glm::rotate(glm::mat4(1.0f), glm::radians(angle.x + rotation.x), right);		
		//glm::mat4 rz = glm::rotate(glm::mat4(1.0f), glm::radians(angle.z+rotation.z), forward);
		newTransfrom = ry*rx;	
		for (uint32_t i = vertexStart; i < vertexStart + unitTypePtr->vertices.size(); i++) {
			_vertices[i].pos = newTransfrom * glm::vec4(_vertices[i].pos, 1.0f);//move back to orientation+rotation
			_vertices[i].pos = tr * glm::vec4(_vertices[i].pos, 1.0f);//move back to pos+movement
		}

		if (translation != glm::vec3(0)) {
			//hitbox.moveCubeVertices(translation);
		}

		angle += rotation;
		pos += translation;
		//pitchPos += translation;
		//yawPos += translation;	
		translation = glm::vec3(0);//translation calculated for each loop/frame
		rotation = glm::vec3(0);//rotation calculated for each loop/frame
		if (angle.x > 360.0f) { angle.x -= 360.0f; }
		if (angle.y > 360.0f) { angle.y -= 360.0f; }
		if (angle.z > 360.0f) { angle.z -= 360.0f; }
		if (angle.x < -360.0f) { angle.x += 360.0f; }
		if (angle.y < -360.0f) { angle.y += 360.0f; }
		if (angle.z < -360.0f) { angle.z += 360.0f; }
		
	}
};

struct construct {

};

//misc struts
struct projectile {
	float drag = 0.20f;
	
	
};

struct gun {
	uint32_t ammo;
	float fireRate;
	double cooldown = 0;
	glm::vec3 barrel;
	glm::vec3 mountingPoint;
};

struct turret {
	glm::vec3 mountingPoint, gunMountingPoint, horizontalRotationPoint, rotationSpeed;
};


struct sentry {
	projectile *projectilePtr;
	gun *gunPtr;
	turret *turretPtr;
	uint32_t turretUnitListIndex, gunUnitListIndex, projectileUnitListIndex;
};


