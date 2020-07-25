#include "Physics.h"



float Physics::kineticEnergy(float _m, float _v)
{
	float E = 0.5 * _m * pow(_v, 2);
	return E;
}

float Physics::momentum(float _m, float _v)
{
	float P = _m*_v;
	return P;
}

glm::vec2 Physics::quadratic(float _a, float _b, float _c)
{
	float x1 = (-_b + sqrt(pow(_b, 2) - (4 * _a*_c))) / (2 * _a);
	float x2 = (-_b - sqrt(pow(_b, 2) - (4 * _a*_c))) / (2 * _a);
	glm::vec2 solution = { x1,x2 };
	return solution;
}

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::force(unit & _unit)
{
	_unit.v.y -= g;//gravity (y axis negative force)
	_unit.v *= 0.999;//drag
	
	float A;
	float cDrag = 1.0f;//coef of drag for a cone
	float drag;//need to calculate coef drag and frontal area for each axis of velocity
	
	/*
	A = (_unit.hitbox.max.z - _unit.hitbox.min.z)*(_unit.hitbox.max.y - _unit.hitbox.min.y);//side
	drag = 0.5*densityAir*pow(_unit.v.x, 2)*A*cDrag;
	drag /= 144.0f;
	if (_unit.v.x > 0) {	
		_unit.v.x -= drag / _unit.m;
	}
	else if(_unit.v.x < 0) {
		_unit.v.x += drag / _unit.m;
	}

	A = (_unit.hitbox.max.x - _unit.hitbox.min.x)*(_unit.hitbox.max.z - _unit.hitbox.min.z);//top
	drag = 0.5*densityAir*pow(_unit.v.y, 2)*A*cDrag;
	drag /= 144.0f;
	if (_unit.v.y > 0) {
		_unit.v.y -= drag / _unit.m;
	}
	else if (_unit.v.y < 0) {
		_unit.v.y += drag / _unit.m;
	}

	A = (_unit.hitbox.max.x - _unit.hitbox.min.x)*(_unit.hitbox.max.y - _unit.hitbox.min.y);//front
	drag = 0.5*densityAir*pow(_unit.v.z, 2)*A*cDrag;
	drag /= 144.0f;
	if (_unit.v.z > 0) {
		_unit.v.z -= drag / _unit.m;
	}
	else if (_unit.v.z < 0) {
		_unit.v.z += drag / _unit.m;
	}
	*/
	
}

void Physics::collision(unit & _unit, unit & _unit2,bool groundCollision)
{
	/*
	glm::vec3 unitMax, unitMin, unit2Max, unit2Min;
	unitMax = _unit.pos + glm::vec3(_unit.radius, _unit.radius, _unit.radius);
	unitMin = _unit.floor;
	unit2Max = _unit2.pos + glm::vec3(_unit2.radius, _unit2.radius, _unit2.radius);
	unit2Min = _unit2.floor;

	if (unitMax.z>unit2Min.z & unitMin.z < unit2Max.z){ 	
		if (unitMax.x > unit2Min.x & unitMin.x < unit2Max.x) {
			if (unitMax.y > unit2Min.y & unitMin.y < unit2Max.y) {
				//potential collision between 2 units
				//impulse = ft = m delta(v) where t is the time to decelerate to 0m/s during the collision
				//
				float m1, v1i, m2, v2i, p1i, p2i, v1f, v2f;
				m1 = _unit.m;
				m2 = _unit2.m;
				v1i = _unit.v.y;
				v2i = _unit2.v.y;
				p1i = momentum(m1, v1i);
				//p2i = momentum(m2, v2i);//p1i=p2i if momentum is conserved
				//assuming kinetic energy (E1i + E1f = E2i + E2F) / linear momentum mv simplifies to v1i+v1f=v2i+v2f
				v2f = ((m1*v1i) - (m1*v2i) + p1i) / (m1 + m2);
				v1f = v2i + v2f - v1i;
				//std::cout << v2f << "," << v1f << std::endl;
				float elasticity = 0.5f;
				//_unit.v.y = v1f*elasticity;
				//_unit2.v.y = -v2f*elasticity;
			}
		}
	}
	*/

	if (_unit.hitbox.hitboxCollision(_unit.hitbox, _unit2.hitbox)) {//_unit.hitbox.hitboxCollision(_unit.hitbox, _unit2.hitbox)//_unit.hitbox.min.y<=_unit2.hitbox.max.y
		//if collision between both unit axis aligned hitboxes 
		//potential collision between 2 units																
		//impulse = ft = m delta(v) where t is the time to decelerate to 0m/s during the collision	
		float elasticity = 0.333f;//fraction of energy conserved during collision (e.g. bouncy ball high elasticity
		glm::vec3 penetration;
		
		float m1, m2, p1i, p2i;
		glm::vec3 v1i, v2i, v1f, v2f;
		m1 = _unit.m;
		m2 = _unit2.m;

		v1i = _unit.v;
		v2i = _unit2.v;
		p1i = m1 * _unit.magnitude(_unit.v);
		v2f = ((m1 * v1i) - (m1 * v2i) + p1i) / (m1 + m2);
		v1f = v2i + v2f - v1i;

		//v1f *= -1;
		


		if (_unit.hitbox.min.y-_unit.v.y > _unit2.hitbox.max.y) {//if the bottom of this unit is above top of other unit do not bounce back
			v1f.x *= -1;
			v1f.z *= -1;		
		}
		if(_unit.hitbox.min.x-_unit.v.x > _unit2.hitbox.max.x)  {
			//v1f.x *= -1;
		}
		if (_unit.hitbox.max.z - _unit.v.z > _unit2.hitbox.min.z) {
			//v1f.z *= -1;
		}

		
		_unit.v = v1f * elasticity;

		////y
		//v1i.y = _unit.v.y;
		//v2i.y = _unit2.v.y;
		//p1i = momentum(m1, v1i.y);		
		//v2f.y = ((m1*v1i.y) - (m1*v2i.y) + p1i) / (m1 + m2);
		//v1f.y = v2i.y + v2f.y - v1i.y;	
		//_unit.v.y = v1f.y*elasticity;
		////z
		//v1i.z = _unit.v.z;
		//v2i.z = _unit2.v.z;
		//p1i = momentum(m1, v1i.z);
		//v2f.z = ((m1*v1i.z) - (m1*v2i.z) + p1i) / (m1 + m2);
		//v1f.z = v2i.z + v2f.z - v1i.z;
		//
		//if (_unit.hitbox.min.y > _unit2.hitbox.max.y) {//deflection (1 above 2)
		//	//_unit.v.z = -v1f.z * elasticity;
		//}
		//else if(_unit.hitbox.min.x - (_unit.v.x / 144.0f) > _unit2.hitbox.max.x) {//deflection
		//	//_unit.v.z = -v1f.z * elasticity;
		//}
		//else {//collision
		//	//_unit.v.z = -v1f.z * elasticity;
		//}
		//
		//_unit.v.z = -v1f.z * elasticity;
		////_unit.v.z = v1f.z *elasticity;//if moving -z, bounces back +z, but when +z bounces -z, needs to be fixed TODO
		////x
		//v1i.x = _unit.v.x;
		//v2i.x = _unit2.v.x;
		//p1i = momentum(m1, v1i.x);
		//v2f.x = ((m1*v1i.x) - (m1*v2i.x) + p1i) / (m1 + m2);
		//v1f.x = v2i.x + v2f.x - v1i.x;
		//_unit.v.x = -v1f.x*elasticity;//needs to be fixed TODO


		//penetration.x = _unit2.hitbox.max.x - _unit.hitbox.min.x;
		//penetration.y = _unit2.hitbox.max.y - _unit.hitbox.min.y;
		//penetration.z = _unit2.hitbox.max.z - _unit.hitbox.min.z;

		penetration = _unit2.hitbox.max - _unit.hitbox.min;


		glm::vec3 penetrationMag;
		penetrationMag.x = abs(penetration.x);
		penetrationMag.y = abs(penetration.y);
		penetrationMag.z = abs(penetration.z);
		float minPenetration;
		minPenetration = std::min(penetrationMag.x, penetrationMag.y);
		minPenetration = std::min(minPenetration, penetrationMag.z);
		if (minPenetration == penetrationMag.x) {
			_unit.move(glm::vec3(penetration.x, 0, 0));//seperate object from other
		}
		else if (minPenetration == penetrationMag.y) {
			_unit.move(glm::vec3(0, penetration.y, 0));//seperate object from other
		}
		else if (minPenetration == penetrationMag.z) {
			_unit.move(glm::vec3(0, 0, penetration.z));//seperate object from other
		}//need to base penetration correction on unit velocity , e.g. move back from same velocity vector e.g. z
	}
}

