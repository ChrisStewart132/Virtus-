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

void Physics::collision(unit & _unit, unit & _unit2)
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
		//if collision between both unit axis aligned hitboxes (calculated only on initial orientation)
		//potential collision between 2 units																
		//impulse = ft = m delta(v) where t is the time to decelerate to 0m/s during the collision															
		float m1, v1i, m2, v2i, p1i, p2i, v1f, v2f;
		m1 = _unit.m;
		m2 = _unit2.m;
		v1i = _unit.v.y;
		v2i = _unit2.v.y;
		p1i = momentum(m1, v1i);		
		v2f = ((m1*v1i) - (m1*v2i) + p1i) / (m1 + m2);
		v1f = v2i + v2f - v1i;	
		float elasticity = 0.333f;//fraction of energy conserved during collision (e.g. bouncy ball high elasticity, lead balls energy is less maleable and converted to damage)
		float penetration = _unit2.hitbox.max.y - _unit.hitbox.min.y;
		_unit.move(glm::vec3(0, penetration, 0));//seperate object from ground to prevent sinking
		_unit.v.y = v1f*elasticity;
		//z
		v1i = _unit.v.z;
		v2i = _unit2.v.z;
		p1i = momentum(m1, v1i);
		v2f = ((m1*v1i) - (m1*v2i) + p1i) / (m1 + m2);
		v1f = v2i + v2f - v1i;
		_unit.v.z = -v1f*elasticity;
		//x
		v1i = _unit.v.x;
		v2i = _unit2.v.x;
		p1i = momentum(m1, v1i);
		v2f = ((m1*v1i) - (m1*v2i) + p1i) / (m1 + m2);
		v1f = v2i + v2f - v1i;
		_unit.v.x = -v1f*elasticity;
	}
}

