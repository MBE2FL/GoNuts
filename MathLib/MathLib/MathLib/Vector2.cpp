#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::~Vector2()
{
}

float Vector2::Length(Vector2 v1)
{
	return sqrt((v1.x * v1.x) + (v1.y * v1.y));
}

Vector2 Vector2::Normalize(Vector2 v1)
{
	return Vector2((v1.x / Length(v1)), (v1.y / Length(v1)));
}

float Vector2::DotProduct(Vector2 v1, Vector2 v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y));
}

float Vector2::ProjLength(Vector2 v1, Vector2 v2)
{
	return (DotProduct(v1, v2) / Length(v1));
}

Vector2 Vector2::ProjPoint(Vector2 v1, Vector2 v2)
{
	return Vector2(((DotProduct(v1, v2) / (Length(v1) * Length(v1))) * v1.x), ((DotProduct(v1, v2) / (Length(v1) * Length(v1))) * v1.y));
}

float Vector2::ProjAngle(Vector2 v1, Vector2 v2)
{
	return (acos((DotProduct(v1, v2) / (Length(v1) * Length(v2)))) *(180 / PI));
}

float Vector2::DistanceBetweenObj(Vector2 v1, Vector2 v2)
{
	return Length(v2 - v1);
}

Vector2 Vector2::CrossProduct(Vector2 v1, Vector2 v2)
{
	return Vector2((v1.x * v2.y), (v1.y * v2.x));
}