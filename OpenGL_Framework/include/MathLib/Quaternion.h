#pragma once

#include "vec3.h"
#include "vec4.h"

class Quaternion
{
public:
	Quaternion(const float angle = 0.0f, const vec3& axis = vec3::Zero);
	Quaternion(const Quaternion& other);
	~Quaternion();

	void conjugate();
	Quaternion getConjugate();
	void rotate(const float angle, vec3& vector);

	Quaternion operator*(const Quaternion& otherQuat) const;

private:
	float _w;//, _x, _y, _z;
	vec3 _xyz;

};