#include "Vector2.h"

Vector2 Vector2::Zero = Vector2(0, 0);

Vector2::~Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

float Vector2::Length()
{
	return sqrt((x * x) + (y * y));
}

Vector2 Vector2::Normalize()
{
	return Vector2((x / Length()), (y / Length()));
}

float Vector2::DotProduct(Vector2 v)
{
	return ((x * v.x) + (y * v.y));
}

float Vector2::ProjLength(Vector2 v)
{
	return (DotProduct(v) / Length());
}

Vector2 Vector2::ProjPoint(Vector2 v)
{
	return Vector2(((DotProduct(v) / (Length() * Length())) * x), ((DotProduct(v) / (Length() * Length())) * y));
}

float Vector2::ProjAngle(Vector2 v)
{
	return (acosf((DotProduct(v) / (Length() * v.Length()))) *(180 / PI));
}

float Vector2::DistanceBetweenObj(Vector2 v)
{
	return (v - *this).Length();
}

Vector2 Vector2::CrossProduct(Vector2 v)
{
	return Vector2((x * v.y), (y * v.x));
}
