#include "Vector3.h"

Vector3::Vector3()
{
}

Vector3::~Vector3()
{
}

float Vector3::Length(Vector3 v1)
{
	return sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z));
}

Vector3 Vector3::Normalize(Vector3 v1)
{
	return Vector3((v1.x / Length(v1)), (v1.y / Length(v1)),(v1.z/Length(v1)));
}

float Vector3::DotProduct(Vector3 v1, Vector3 v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y)+(v1.z * v2.z));
}

float Vector3::ProjLength(Vector3 v1, Vector3 v2)
{
	return (DotProduct(v1, v2) / Length(v1));
}

Vector3 Vector3::ProjPoint(Vector3 v1, Vector3 v2)
{
	return Vector3(((DotProduct(v1, v2) / (Length(v1) * Length(v1))) * v1.x)
				 , ((DotProduct(v1, v2) / (Length(v1) * Length(v1))) * v1.y)
				 , ((DotProduct(v1, v2) / (Length(v1) * Length(v1))) * v1.z));
}

float Vector3::ProjAngle(Vector3 v1, Vector3 v2)
{
	return ((acos(DotProduct(v1, v2) / (Length(v1) * Length(v2)))) *(180 / PI));

}

float Vector3::DistanceBetweenObj(Vector3 v1, Vector3 v2)
{
	return Length(v2 - v1);
}

Vector3 Vector3::CrossProduct(Vector3 v1, Vector3 v2)
{
	return Vector3(((v1.y * v1.z)-(v1.z*v2.y)),((v1.x*v2.z)-(v1.z*v2.x)),((v1.x*v2.y)-(v1.y*v2.x)));
}
