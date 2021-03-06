#include "Vector3.h"

Vector3::Vector3()
{
}

Vector3::~Vector3()
{
}

float Vector3::Length()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::Normalize()
{
	return Vector3((x / Length()), (y / Length()),(z/Length()));
}

float Vector3::DotProduct(Vector3 v)
{
	return ((x * v.x) + (y * v.y)+(z * v.z));
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
	return ((acos(DotProduct(v) / (Length() * v.Length()))) *(180 / PI));

}

float Vector3::DistanceBetweenObj(Vector3 v)
{
	return (v - *this).Length();
}

Vector3 Vector3::CrossProduct(Vector3 v)
{
	return Vector3(((y * z) - (z*v.y)), ((x*v.z) - (z*v.x)), ((x*v.y) - (y*v.x)));
}
