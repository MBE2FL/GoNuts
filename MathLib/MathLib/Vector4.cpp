#include "Vector4.h"

Vector4::Vector4()
{
}

Vector4::~Vector4()
{
}

float Vector4::Length()
{
	return 0.0f;
}

Vector4 Vector4::Normalize()
{
	return Vector4();
}

float Vector4::DotProduct(Vector4 v)
{
	return 0.0f;
}

float Vector4::ProjLength(Vector4 v)
{
	return 0.0f;
}

Vector4 Vector4::ProjPoint(Vector4 v)
{
	return Vector4();
}

float Vector4::ProjAngle(Vector4 v)
{
	return 0.0f;
}

float Vector4::DistanceBetweenObj(Vector4 v)
{
	return 0.0f;
}

Vector4 Vector4::CrossProduct(Vector4 v)
{
	return Vector4();
}

Vector4 operator*(const Matrix44 m, const Vector4 v)
{
	Vector4 result;

	result.x = m.mV[0] * v.x + m.mV[1] * v.y + m.mV[2] * v.z + m.mV[3] * v.q;
	result.y = m.mV[4] * v.x + m.mV[5] * v.y + m.mV[6] * v.z + m.mV[7] * v.q;
	result.z = m.mV[8] * v.x + m.mV[9] * v.y + m.mV[10] * v.z + m.mV[11] * v.q;
	result.q = m.mV[12] * v.x + m.mV[13] * v.y + m.mV[14] * v.z + m.mV[15] * v.q;

	return result;
}
