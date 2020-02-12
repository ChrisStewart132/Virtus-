#pragma once
#include <glm/glm.hpp>

class math6 {
private:

public:

	float distance(glm::vec3 v1, glm::vec3 v2) {
		float distance;
		distance = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
		return distance;
	}

	float magnitude(glm::vec3 vec1) {
		float magnitude;
		magnitude = sqrt(pow(vec1.x, 2) + pow(vec1.y, 2) + pow(vec1.z, 2));
		return magnitude;
	}

	float dotProduct(glm::vec3 vec1, glm::vec3 vec2) {
		float dot;
		dot = (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
		return dot;
	}

	float calcAngle(glm::vec3 vec1, glm::vec3 vec2) {
		float product,angle;
		product = dotProduct(vec1, vec2);
		angle = acos(product / ((magnitude(vec1) * magnitude(vec2))));
		return angle;
	}

	glm::vec3 crossProduct(glm::vec3 vec1, glm::vec3 vec2) {
		glm::vec3 cross;
		//float angle = calcAngle(vec1, vec2);
		//cross = magnitude(vec1) * magnitude(vec2) * cos(angle);
		//cross.x = (vec)
		return cross;
	}

	//chain rule
	//dz/dx = 
};
