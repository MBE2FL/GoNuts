#include "MathLibCore.h"

float MathUtils::toRadians(const float degrees)
{
	return (degrees * PI / 180.0f);
}

float MathUtils::toDegrees(const float radians)
{
	return (radians * 180.0f / PI);
}
