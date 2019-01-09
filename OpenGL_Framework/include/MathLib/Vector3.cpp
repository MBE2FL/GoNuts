#include "Vector3.h"

Vector3 Vector3::Zero = Vector3(0.0f);
Vector3 Vector3::One = Vector3(1.0f);

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(Vector2 vec2, float Z)
{
	x = vec2.x;
	y = vec2.y;
	z = Z;
}

Vector3::~Vector3()
{
}

float Vector3::Length()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

float Vector3::sqrLength()
{
	return ((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::Normalize()
{
	return Vector3((x / Length()), (y / Length()),(z/Length()));
}

float Vector3::DotProduct(Vector3 v)
{
	return ((x * v.x) + (y * v.y)+(z * v.z));
}

float Vector3::dot(const Vector3 & lhs, const Vector3 & rhs)
{
	return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z));
}

float Vector3::ProjLength(Vector3 v)
{
	return (DotProduct(v) / Length());
}

Vector3 Vector3::ProjPoint(Vector3 v)
{
	return Vector3(((DotProduct(v) / (Length() * Length())) * x)
				 , ((DotProduct(v) / (Length() * Length())) * y)
				 , ((DotProduct(v) / (Length() * Length())) * z));
}

float Vector3::ProjAngle(Vector3 v)
{
	return ((acosf(DotProduct(v) / (Length() * v.Length()))) *(180 / PI));

}

float Vector3::DistanceBetweenObj(Vector3 v)
{
	return (v - *this).Length();
}

float Vector3::distance(const Vector3 & lhs, const Vector3 & rhs)
{
	return (rhs - lhs).Length();
}

float Vector3::sqrDistance(const Vector3 & lhs, const Vector3 & rhs)
{
	return (rhs - lhs).sqrLength();
}

Vector3 Vector3::CrossProduct(Vector3 v)
{
	return Vector3(((y * z) - (z*v.y)), ((x*v.z) - (z*v.x)), ((x*v.y) - (y*v.x)));
}