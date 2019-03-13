#include "vec2.h"

vec2 vec2::Zero = vec2(0.0f, 0.0f);
vec2 vec2::One = vec2(1.0f, 1.0f);

vec2::~vec2()
{
	x = 0.0f;
	y = 0.0f;
}

float vec2::Length()
{
	return sqrt((x * x) + (y * y));
}

vec2 vec2::Normalize()
{
	return vec2((x / Length()), (y / Length()));
}

float vec2::DotProduct(vec2& v)
{
	return ((x * v.x) + (y * v.y));
}

float vec2::ProjLength(vec2& v)
{
	return (DotProduct(v) / Length());
}

vec2 vec2::ProjPoint(vec2& v)
{
	return vec2(((DotProduct(v) / (Length() * Length())) * x), ((DotProduct(v) / (Length() * Length())) * y));
}

float vec2::ProjAngle(vec2& v)
{
	return (acosf((DotProduct(v) / (Length() * v.Length()))) *(180 / PI));
}

float vec2::DistanceBetweenObj(vec2& v)
{
	return (v - *this).Length();
}

vec2 vec2::CrossProduct(vec2& v)
{
	return vec2((x * v.y), (y * v.x));
}
