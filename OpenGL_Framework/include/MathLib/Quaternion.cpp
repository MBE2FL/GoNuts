#include "Quaternion.h"
#include "MathLibCore.h"

using MathUtils::toRadians;

Quaternion Quaternion::Identity = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion()
{
	_w = 1.0f;
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Quaternion::Quaternion(const float w, const float x, const float y, const float z)
	: _w(w), _x(x), _y(y), _z(z)
{
	normalize();
}

Quaternion::Quaternion(const float angle, vec3 axis)
{
	axis.normalize();

	float radians = toRadians(angle);

	_w = cosf(radians * 0.5f);

	vec3 sinHalfTheta = sinf(radians * 0.5f) * axis;

	_x = sinHalfTheta.x;
	_y = sinHalfTheta.y;
	_z = sinHalfTheta.z;

	normalize();
}

Quaternion::Quaternion(const Quaternion & other)
{
	_w = other._w;
	_x = other._x;
	_y = other._y;
	_z = other._z;

	normalize();
}

Quaternion::Quaternion(const mat4 & rot)
{
	// #################### Post Hogue
	float trace = 1.0f + rot.data[0] + rot.data[5] + rot.data[10];

	// Trace > 0. Perform instant calc
	if (trace > 0.00000001f)
	{
		float s = sqrt(trace) * 2.0f;
		_x = (rot.data[6] - rot.data[9]) / s;
		_y = (rot.data[8] - rot.data[2]) / s;
		_z = (rot.data[1] - rot.data[4]) / s;
		_w = 0.25f * s;
	}
	else
	{
		// Column 0
		if ((rot.data[0] > rot.data[5]) && (rot.data[0] > rot.data[10]))
		{
			float s = sqrt(1.0f + rot.data[0] - rot.data[5] - rot.data[10]) * 2.0f;
			_x = 0.25f * s;
			_y = (rot.data[1] + rot.data[4]) / s;
			_z = (rot.data[8] + rot.data[2]) / s;
			_w = (rot.data[6] - rot.data[9]) / s;
		}
		// Column 1
		else if (rot.data[5] > rot.data[10])
		{
			float s = sqrt(1.0f + rot.data[5] - rot.data[0] - rot.data[10]) * 2.0f;
			_x = (rot.data[1] + rot.data[4]) / s;
			_y = 0.25f * s;
			_z = (rot.data[6] + rot.data[9]) / s;
			_w = (rot.data[8] - rot.data[2]) / s;
		}
		// Column 2
		else
		{
			float s = sqrt(1.0f + rot.data[10] - rot.data[0] - rot.data[5]) * 2.0f;
			_x = (rot.data[8] + rot.data[2]) / s;
			_y = (rot.data[6] + rot.data[9]) / s;
			_z = 0.25f * s;
			_w = (rot.data[1] - rot.data[4]) / s;
		}
	}

	normalize();
}

Quaternion::Quaternion(const float eulerX, const float eulerY, const float eulerZ)
{
	//vec3 xAxis = vec3(1.0f, 0.0f, 0.0f);
	//vec3 yAxis = vec3(0.0f, 1.0f, 0.0f);
	//vec3 zAxis = vec3(0.0f, 0.0f, 1.0f);

	//Quaternion xQuat = Quaternion(eulerX, xAxis);
	//Quaternion yQuat = Quaternion(eulerY, yAxis);
	//Quaternion zQuat = Quaternion(eulerZ, zAxis);

	//*this = Quaternion((xQuat * yQuat) * zQuat);

	const float sinPitch = sin(eulerX * 0.5f);
	const float cosPitch = cos(eulerX * 0.5f);

	const float sinYaw = sin(eulerY * 0.5f);
	const float cosYaw = cos(eulerY * 0.5f);

	const float sinRoll = sin(eulerZ * 0.5f);
	const float cosRoll = cos(eulerZ * 0.5f);

	const float cosPitchCosYaw = cosPitch * cosYaw;
	const float sinPitchSinYaw = sinPitch * sinYaw;

	_x = (sinRoll * cosPitchCosYaw) - (cosRoll * sinPitchSinYaw);
	_y = (cosRoll * sinPitch * cosYaw) + (sinRoll * cosPitch * sinYaw);
	_z = (cosRoll * cosPitch * sinYaw) - (sinRoll * sinPitch * cosYaw);
	_w = (cosRoll * cosPitchCosYaw) + (sinRoll * sinPitchSinYaw);

	normalize();
}

Quaternion::~Quaternion()
{
}

void Quaternion::conjugate()
{
	_x *= -1.0f;
	_y *= -1.0f;
	_z *= -1.0f;
}

Quaternion Quaternion::getConjugate()
{
	Quaternion conj = Quaternion(*this);
	conj.conjugate();

	return conj;
}

void Quaternion::rotate(const float angle, vec3 axis)
{
	Quaternion q = Quaternion(angle, axis);
	Quaternion qc = q.getConjugate();


	Quaternion qpqc = q * *this * qc;
}

mat4 Quaternion::getRotationMatrix()
{
	mat4 rot;
	//normalize();
	//float sqrW = pow(_w, 2.0f);
	//float sqrX = pow(_x, 2.0f);
	//float sqrY = pow(_y, 2.0f);
	//float sqrZ = pow(_z, 2.0f);

	//// quat not normalized.
	//if (abs(1.0f - (sqrW + sqrX + sqrY + sqrZ)) > 0.01f)
	//{
	//	float mag = sqrt(sqrX + sqrY + sqrZ + sqrW);
	//	_x /= mag;
	//	_y /= mag;
	//	_z /= mag;
	//	_w /= mag;
	//}
	float xx = _x * _x;
	float xy = _x * _y;
	float xz = _x * _z;
	float xw = _x * _w;
	float yy = _y * _y;
	float yz = _y * _z;
	float yw = _y * _w;
	float zz = _z * _z;
	float zw = _z * _w;

	// Row #1
	rot.data[0] = 1.0f - (2.0f * (yy + zz));
	rot.data[4] = 2.0f * (xy - zw);
	rot.data[8] = 2.0f * (xz + yw);
	rot.data[12] = 0.0f;

	// Row #2
	rot.data[1] = 2.0f * (xy + zw);
	rot.data[5] = 1.0f - (2.0f * (xx + zz));
	rot.data[9] = 2.0f * (yz - xw);
	rot.data[13] = 0.0f;

	// Row #3
	rot.data[2] = 2.0f * (xz - yw);
	rot.data[6] = 2.0f * (yz + xw);
	rot.data[10] = 1.0f - (2.0f * (xx + yy));
	rot.data[14] = 0.0f;


	// Row #4
	rot.data[3] = 0.0f;
	rot.data[7] = 0.0f;
	rot.data[11] = 0.0f;
	rot.data[15] = 1.0f;


	return rot;
}

void Quaternion::normalize()
{
	float sqrW = pow(_w, 2.0f);
	float sqrX = pow(_x, 2.0f);
	float sqrY = pow(_y, 2.0f);
	float sqrZ = pow(_z, 2.0f);

	float mag = sqrt(sqrX + sqrY + sqrZ + sqrW);
	_x /= mag;
	_y /= mag;
	_z /= mag;
	_w /= mag;
}

Quaternion Quaternion::getNormalized() const
{
	Quaternion normQuat = *this;
	normQuat.normalize();

	return normQuat;
}

void Quaternion::setW(const float w)
{
	_w = w;
}

void Quaternion::setX(const float x)
{
	_x = x;
}

void Quaternion::setY(const float y)
{
	_y = y;
}

void Quaternion::setZ(const float z)
{
	_z = z;
}

Quaternion Quaternion::operator*(const Quaternion & otherQuat) const
{
	float q1_w = _w;
	float q1_x = _x;
	float q1_y = _y;
	float q1_z = _z;

	float q2_w = otherQuat._w;
	float q2_x = otherQuat._x;
	float q2_y = otherQuat._y;
	float q2_z = otherQuat._z;

	float w = (q1_w * q2_w) - (q1_x * q2_x) - (q1_y * q2_y) - (q1_z * q2_z);
	float x = (q1_w * q2_x) + (q1_x * q2_w) + (q1_y * q2_z) - (q1_z * q2_y);
	float y = (q1_w * q2_y) - (q1_x * q2_z) + (q1_y * q2_w) + (q1_z * q2_x);
	float z = (q1_w * q2_z) + (q1_x * q2_y) - (q1_y * q2_x) + (q1_z * q2_w);

	Quaternion q1q2 = Quaternion(w, x, y, z);

	return q1q2;
}

Quaternion Quaternion::slerp(const Quaternion & begin, const Quaternion & end, float interValue)
{
	if (interValue < 0)
		interValue = 0;
	if (interValue > 1)
		interValue = 1;

	// Normalize inputs
	Quaternion nBegin = begin.getNormalized();
	Quaternion nEnd = end.getNormalized();

	Quaternion quat = Quaternion();

	// Calculate the angle between the begin and end quaternions.
	float cosHalfTheta = (nBegin._w * nEnd._w) + (nBegin._x * nEnd._x) + (nBegin._y * nEnd._y) + (nBegin._z * nEnd._z);

	// If begin = end or begin = -end then theta = 0 and we can return begin.
	if (fabs(cosHalfTheta) >= 1.0f)
	{
		quat._w = nBegin._w;
		quat._x = nBegin._x;
		quat._y = nBegin._y;
		quat._z = nBegin._z;
		return quat;
	}

	float halfTheta = acosf(cosHalfTheta);
	float sinHalfTheta = sqrtf(1.0f - (cosHalfTheta * cosHalfTheta));

	// If theta = 180 degrees the result is not fully defined.
	// We could rotate around any axis normal to begin or end.
	if (fabs(sinHalfTheta) < 0.001f)
	{
		quat._w = (nBegin._w * 0.5f) + (nEnd._w * 0.5f);
		quat._x = (nBegin._x * 0.5f) + (nEnd._x * 0.5f);
		quat._y = (nBegin._y * 0.5f) + (nEnd._y * 0.5f);
		quat._z = (nBegin._z * 0.5f) + (nEnd._z * 0.5f);

		return quat;
	}

	float ratioBegin = sinf((1.0f - interValue) * halfTheta) / sinHalfTheta;
	float ratioEnd = sinf(interValue * halfTheta) / sinHalfTheta;

	// Calculate quaternion
	quat._w = (nBegin._w * ratioBegin) + (nEnd._w * ratioEnd);
	quat._x = (nBegin._x * ratioBegin) + (nEnd._x * ratioEnd);
	quat._y = (nBegin._y * ratioBegin) + (nEnd._y * ratioEnd);
	quat._z = (nBegin._z * ratioBegin) + (nEnd._z * ratioEnd);

	return quat;
}

Quaternion Quaternion::nslerp(const Quaternion & begin, const Quaternion & end, float interValue)
{
	Quaternion result = Quaternion();

	float dot = begin._w * end._w + begin._x * end._x + begin._y * end._y + begin._z * end._z;
	float blendI = 1.0f - interValue;
	if (dot < 0) {
		result._w = blendI * begin._w + interValue * -end._w;
		result._x = blendI * begin._x + interValue * -end._x;
		result._y = blendI * begin._y + interValue * -end._y;
		result._z = blendI * begin._z + interValue * -end._z;
	}
	else {
		result._w = blendI * begin._w + interValue * end._w;
		result._x = blendI * begin._x + interValue * end._x;
		result._y = blendI * begin._y + interValue * end._y;
		result._z = blendI * begin._z + interValue * end._z;
	}
	result.normalize();
	return result;
}
