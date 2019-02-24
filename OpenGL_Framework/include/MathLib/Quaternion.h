#pragma once

#include "vec3.h"
#include "mat4.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(const float w, const float x, const float y, const float z);
	Quaternion(const float angle, const vec3& axis);
	Quaternion(const Quaternion& other);
	Quaternion(const mat4& rot);
	~Quaternion();

	void conjugate();
	Quaternion getConjugate();
	void rotate(const float angle, const vec3& axis);
	mat4 getRotationMatrix() const;

	Quaternion operator*(const Quaternion& otherQuat) const;

	static Quaternion slerp(const Quaternion& begin, const Quaternion& end, float interValue);

	static Quaternion Identity;

private:
	float _w, _x, _y, _z;

};