#include "Quaternion.h"

Quaternion::Quaternion(const float angle, const vec3 & axis)
{
	_w = cosf(angle * 0.5f);
	_xyz = sinf(angle * 0.5f) * axis;
}

Quaternion::Quaternion(const Quaternion & other)
{
	_w = other._w;
	_xyz = other._xyz;
}

Quaternion::~Quaternion()
{
}

void Quaternion::conjugate()
{
	_xyz *= -1.0f;
}

Quaternion Quaternion::getConjugate()
{
	Quaternion conj = Quaternion(*this);
	conj.conjugate();

	return conj;
}

void Quaternion::rotate(const float angle, vec3 & vector)
{
	Quaternion pure = Quaternion(0.0f, vector);

	Quaternion qpqc = *this * pure * this->getConjugate();

	vector = qpqc._xyz;
}

Quaternion Quaternion::operator*(const Quaternion & otherQuat) const
{
	vec4 q1 = vec4(_xyz, _w);
	vec4 q2 = vec4(otherQuat._xyz, otherQuat._w);

	float w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
	float x = (q1.w * q2.x) - (q1.x * q2.w) - (q1.y * q2.z) - (q1.z * q2.y);
	float y = (q1.w * q2.y) - (q1.x * q2.z) - (q1.y * q2.w) - (q1.z * q2.x);
	float z = (q1.w * q2.z) - (q1.x * q2.y) - (q1.y * q2.x) - (q1.z * q2.w);
	vec3 xyz = vec3(x, y, z);

	Quaternion q1q2 = Quaternion(w, xyz);

	return q1q2;
}
