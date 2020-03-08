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

	bool raySquareIntersection(glm::vec3 ray, glm::vec3 rayPos, glm::vec3 max, glm::vec3 min, float plane, glm::vec3 planeAxis) {
		float y, m, x, c;
		glm::vec3 rayPlaneIntersection(0);

		//if (rayPos.z > 0 && max.z < 0 || rayPos.z < 0 && max.z > 0) {//cam -z, obj +z or vice versa
		//	x = (abs(rayPos.z) + abs(max.z)) / abs(ray.z);//ray distance between camera z plane and target z plane (c = rayPos.y)
		//}
		//else {
		//	x = abs(abs(rayPos.z) - abs(max.z)) / abs(ray.z);
		//}



		//todo
		//if planeaxis.z=1
		//rayplane=raypos.z,rayGradient=ray.z
		float rayPlane, rayGradient;
		if (planeAxis.z) {
			rayPlane = rayPos.z;
			rayGradient = ray.z;
		}else if(planeAxis.y) {
			rayPlane = rayPos.y;
			rayGradient = ray.y;
		}
		else if (planeAxis.x) {
			rayPlane = rayPos.x;
			rayGradient = ray.x;
		}


		//x = (abs(rayPlane) + abs(plane)) / abs(rayGradient);
		x = abs(rayPlane - plane) / abs(rayGradient);

		if (planeAxis.y == 0) {
			//y 
			//x = (abs(rayPos.z) + abs(plane)) / abs(ray.z);
			m = ray.y;
			c = rayPos.y;
			y = (m * x) + c;
			if (y<max.y && y > min.y) {
				rayPlaneIntersection.y = true;
			}
			else {
				rayPlaneIntersection.y = false;
			}
		}
		if (planeAxis.x == 0) {
			//x 
			//x = (abs(rayPos.z) + abs(plane)) / abs(ray.z);
			m = ray.x;
			c = rayPos.x;
			y = (m * x) + c;
			if (y<max.x && y > min.x) {
				rayPlaneIntersection.x = true;
			}
			else {
				rayPlaneIntersection.x = false;
			}
		}
		if (planeAxis.z == 0) {
			//z
			//x = (abs(rayPos.z) + abs(plane)) / abs(ray.z);
			m = ray.z;
			c = rayPos.z;
			y = (m * x) + c;
			if (y<max.z && y > min.z) {
				rayPlaneIntersection.z = true;
			}
			else {
				rayPlaneIntersection.z = false;
			}
		}

		//if (rayPos.z > 0 && max.z < 0 || rayPos.z < 0 && max.z > 0) {//cam -z, obj +z or vice versa
		//	x = (abs(rayPos.z) + abs(max.z)) / abs(ray.z);//ray distance between camera z plane and target z plane (c = rayPos.y)
		//}
		//else {
		//	x = abs(abs(rayPos.z) - abs(max.z)) / abs(ray.z);
		//}
		//m = ray.y;
		//c = rayPos.y;
		//y = (m * x) + c;
		//if (y<max.y && y > min.y) {
		//	rayPlaneIntersection.first = true;
		//}
		//else {
		//	rayPlaneIntersection.first = false;
		//}
		//
		//x = (abs(rayPos.z) + abs(max.z)) / abs(ray.z);
		//m = ray.x;
		//c = rayPos.x;
		//y = (m * x) + c;
		//if (y<max.x && y > min.x) {
		//	rayPlaneIntersection.second = true;
		//}
		//else {
		//	rayPlaneIntersection.second = false;
		//}







		if (rayPlaneIntersection.x + rayPlaneIntersection.y + rayPlaneIntersection.z > 1) {
			return true;
		}
		else {
			return false;
		}
		return 0;
	}

};
