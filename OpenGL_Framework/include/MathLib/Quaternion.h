#pragma once

#include "vec3.h"
#include "mat4.h"

class Quaternion
{
public:
	Quaternion();
	Quaternion(const float w, const float x, const float y, const float z);
	Quaternion(const float angle, vec3 axis);
	Quaternion(const Quaternion& other);
	Quaternion(const mat4& rot);
	Quaternion(const float eulerX, const float eulerY, const float eulerZ);
	~Quaternion();

	void conjugate();
	Quaternion getConjugate();
	void rotate(const float angle, vec3 axis);
	mat4 getRotationMatrix();
	void normalize();
	Quaternion getNormalized() const;

	float getW() const;
	float getX() const;
	float getY() const;
	float getZ() const;

	void setW(const float w);
	void setX(const float x);
	void setY(const float y);
	void setZ(const float z);

	Quaternion operator*(const Quaternion& otherQuat) const;

	static Quaternion slerp(const Quaternion& begin, const Quaternion& end, float interValue);
	static Quaternion nslerp(const Quaternion& begin, const Quaternion& end, float interValue);

	static Quaternion Identity;

private:
	float _w, _x, _y, _z;

};