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

Quaternion::Quaternion(const float angle, const vec3 & axis)
{
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
	//_w = sqrt(1.0f + rot.data[0] + rot.data[5] + rot.data[10]) * 0.5f;
	//_x = (rot.data[6] - rot.data[9]) / (4.0f * _w);
	//_y = (rot.data[8] - rot.data[2]) / (4.0f * _w);
	//_z = (rot.data[1] - rot.data[4]) / (4.0f * _w);


	//float m01 = rot.data[4];
	//float m02 = rot.data[8];

	//float m10 = rot.data[1];
	//float m12 = rot.data[9];

	//float m20 = rot.data[2];
	//float m21 = rot.data[6];

	//float m00 = rot.data[0];
	//float m11 = rot.data[5];
	//float m22 = rot.data[10];

	//float trace = m00 + m11 + m22;

	//if ((1.0f + trace) > 0)
	//{
	//	float s = sqrt(1.0f + trace) * 2.0f;
	//	_w = 0.25f * s;
	//	_x = (m21 - m12) / s;
	//	_y = (m02 - m20) / s;
	//	_z = (m10 - m01) / s;
	//}
	//else if ((m00 > m11) && (m00 > m22))
	//{
	//	float s = sqrt(1.0f + m00 - m11 - m22) * 2.0f;
	//	_w = (m21 - m12) / s;
	//	_x = 0.25f * s;
	//	_y = (m01 + m10) / s;
	//	_z = (m02 + m20) / s;
	//}
	//else if (m11 > m22)
	//{
	//	float s = sqrt(1.0f + m11 - m00 - m22) * 2.0f;
	//	_w = (m02 - m20) / s;
	//	_x = (m01 + m10) / s;
	//	_y = 0.25f * s;
	//	_z = (m12 + m21) / s;
	//}
	//else
	//{
	//	float s = sqrt(1.0f + m22 - m00 - m11) * 2.0f;
	//	_w = (m10 - m01) / s;
	//	_x = (m02 + m20) / s;
	//	_y = (m12 + m21) / s;
	//	_z = 0.25f * s;
	//}



	// #################### Post Hogue
	float trace = 1.0f + rot.data[0] + rot.data[5] + rot.data[10];

	// Trace > 0. Perform instant calc
	if (trace > 0.00000001f)
	{
		float s = sqrt(trace) * 2.0f;
		_x = (rot.data[9] - rot.data[6]) / s;
		_y = (rot.data[2] - rot.data[8]) / s;
		_z = (rot.data[4] - rot.data[1]) / s;
		_w = 0.25f * s;
	}
	else
	{
		// Column 0
		if ((rot.data[0] > rot.data[5]) && (rot.data[0] > rot.data[10]))
		{
			float s = sqrt(1.0f + rot.data[0] - rot.data[5] - rot.data[10]) * 2.0f;
			_x = 0.25f * s;
			_y = (rot.data[4] + rot.data[1]) / s;
			_z = (rot.data[2] + rot.data[8]) / s;
			_w = (rot.data[9] - rot.data[6]) / s;
		}
		// Column 1
		else if (rot.data[5] > rot.data[10])
		{
			float s = sqrt(1.0f + rot.data[5] - rot.data[0] - rot.data[10]) * 2.0f;
			_x = (rot.data[4] + rot.data[1]) / s;
			_y = 0.25f * s;
			_z = (rot.data[9] + rot.data[6]) / s;
			_w = (rot.data[2] - rot.data[8]) / s;
		}
		// Column 2
		else
		{
			float s = sqrt(1.0f + rot.data[10] - rot.data[0] - rot.data[5]) * 2.0f;
			_x = (rot.data[2] + rot.data[8]) / s;
			_y = (rot.data[9] + rot.data[6]) / s;
			_z = 0.25f * s;
			_w = (rot.data[4] - rot.data[1]) / s;
		}
	}

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

void Quaternion::rotate(const float angle, const vec3 & axis)
{
	Quaternion q = Quaternion(angle, axis);
	Quaternion qc = q.getConjugate();


	Quaternion qpqc = q * *this * qc;
}

mat4 Quaternion::getRotationMatrix()
{
	mat4 rot;
	normalize();
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

	rot.data[0] = 1.0f - (2.0f * (yy + zz));
	rot.data[1] = 2.0f * (xy - zw);
	rot.data[2] = 2.0f * (xz + yw);
	rot.data[3] = 0.0f;

	rot.data[4] = 2.0f * (xy + zw);
	rot.data[5] = 1.0f - (2.0f * (xx + zz));
	rot.data[6] = 2.0f * (yz - xy);
	rot.data[7] = 0.0f;

	rot.data[8] = 2.0f * (xz - yw);
	rot.data[9] = 2.0f * (yz + xw);
	rot.data[10] = 1.0f - (2.0f * (xx + yy));
	rot.data[11] = 0.0f;

	rot.data[12] = 0.0f;
	rot.data[13] = 0.0f;
	rot.data[14] = 0.0f;
	rot.data[15] = 1.0f;

	/*rot.data[0] = 1.0f - (2.0f * ((_y * _y) + (_z * _z)));
	rot.data[1] = 2.0f * ((_x * _y) - (_z * _w));
	rot.data[2] = 2.0f * ((_x * _z) + (_y * _w));
	rot.data[3] = 0.0f;

	rot.data[4] = 2.0f * ((_x * _y) + (_z * _w));
	rot.data[5] = 1.0f - (2.0f * ((_x * _x) + (_z * _z)));
	rot.data[6] = 2.0f * ((_y * _z) - (_x * _w));
	rot.data[7] = 0.0f;

	rot.data[8] = 2.0f * ((_x * _z) - (_y * _w));
	rot.data[9] = 2.0f * ((_y * _z) + (_x * _w));
	rot.data[10] = 1.0f - (2.0f * ((_x * _x) + (_y * _y)));
	rot.data[11] = 0.0f;

	rot.data[12] = 0.0f;
	rot.data[13] = 0.0f;
	rot.data[14] = 0.0f;
	rot.data[15] = 1.0f;*/




	//float sqrW = pow(_w, 2.0f);
	//float sqrX = pow(_x, 2.0f);
	//float sqrY = pow(_y, 2.0f);
	//float sqrZ = pow(_z, 2.0f);

	//// quat not normalized.
	//if ((sqrW + sqrX + sqrY + sqrZ) != 1.0f)
	//{


	//	float inverseSqrLen = 1 / (sqrW + sqrX + sqrY + sqrZ);


	//	rot.data[0] = (sqrX - sqrY - sqrZ + sqrW) * inverseSqrLen;
	//	rot.data[1] = ((2.0f * _x * _y) + (2.0f * _z * _w)) * inverseSqrLen;
	//	rot.data[2] = ((2.0f * _x * _z) - (2.0f * _y * _w)) * inverseSqrLen;
	//	rot.data[3] = 0.0f;

	//	rot.data[4] = ((2.0f * _x * _y) - (2.0f * _z * _w)) * inverseSqrLen;
	//	rot.data[5] = (-sqrX + sqrY - sqrZ + sqrW) * inverseSqrLen;
	//	rot.data[6] = ((2.0f * _y * _z) + (2.0f * _x * _w)) * inverseSqrLen;
	//	rot.data[7] = 0.0f;

	//	rot.data[8] = ((2.0f * _x * _z) + (2.0f * _y * _w)) * inverseSqrLen;
	//	rot.data[9] = ((2.0f * _y * _z) - (2.0f * _x * _w)) * inverseSqrLen;
	//	rot.data[10] = (-sqrX - sqrY + sqrZ + sqrW) * inverseSqrLen;
	//	rot.data[11] = 0.0f;
	//}
	//else
	//{
	//	rot.data[0] = 1.0f - (2.0f * pow(_y, 2.0f)) - (2.0f * pow(_z, 2.0f));
	//	rot.data[1] = (2.0f * _x * _y) + (2.0f * _z * _w);
	//	rot.data[2] = (2.0f * _x * _z) - (2.0f * _y * _w);
	//	rot.data[3] = 0.0f;
	//	
	//	rot.data[4] = (2.0f * _x * _y) - (2.0f * _z * _w);
	//	rot.data[5] = 1.0f - (2.0f * pow(_x, 2.0f)) - (2.0f * pow(_z, 2.0f));
	//	rot.data[6] = (2.0f * _y * _z) + (2.0f * _x * _w);
	//	rot.data[7] = 0.0f;
	//	
	//	rot.data[8] = (2.0f * _x * _z) + (2.0f * _y * _w);
	//	rot.data[9] = (2.0f * _y * _z) - (2.0f * _x * _w);
	//	rot.data[10] = 1.0f - (2.0f * pow(_x, 2.0f)) - (2.0f * pow(_y, 2.0f));
	//	rot.data[11] = 0.0f;
	//}


	//rot.data[12] = 0.0f;
	//rot.data[13] = 0.0f;
	//rot.data[14] = 0.0f;
	//rot.data[15] = 1.0f;

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

	Quaternion quat = Quaternion();

	// Calculate the angle between the begin and end quaternions.
	float cosHalfTheta = (begin._w * end._w) + (begin._x * end._x) + (begin._y * end._y) + (begin._z * end._z);

	// If begin = end or begin = -end then theta = 0 and we can return begin.
	if (fabs(cosHalfTheta) >= 1.0f)
	{
		quat._w = begin._w;
		quat._x = begin._x;
		quat._y = begin._y;
		quat._z = begin._z;
		return quat;
	}

	float halfTheta = acosf(cosHalfTheta);
	float sinHalfTheta = sqrtf(1.0f - (cosHalfTheta * cosHalfTheta));

	// If theta = 180 degrees the result is not fully defined.
	// We could rotate around any axis normal to begin or end.
	if (fabs(sinHalfTheta) < 0.001f)
	{
		quat._w = (begin._w * 0.5f) + (end._w * 0.5f);
		quat._x = (begin._x * 0.5f) + (end._x * 0.5f);
		quat._y = (begin._y * 0.5f) + (end._y * 0.5f);
		quat._z = (begin._z * 0.5f) + (end._z * 0.5f);

		return quat;
	}

	float ratioBegin = sinf((1.0f - interValue) * halfTheta) / sinHalfTheta;
	float ratioEnd = sinf(interValue * halfTheta) / sinHalfTheta;

	// Calculate quaternion
	quat._w = (begin._w * ratioBegin) + (end._w * ratioEnd);
	quat._x = (begin._x * ratioBegin) + (end._x * ratioEnd);
	quat._y = (begin._y * ratioBegin) + (end._y * ratioEnd);
	quat._z = (begin._z * ratioBegin) + (end._z * ratioEnd);

	return quat;
}
