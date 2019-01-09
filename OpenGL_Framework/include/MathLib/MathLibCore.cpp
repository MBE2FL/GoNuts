#include "MathLibCore.h"

float MathLibCore::toRadians(const float degrees)
{
	return (degrees * PI / 180.0f);
}

float MathLibCore::toDegrees(const float radians)
{
	return (radians * 180.0f / PI);
}
