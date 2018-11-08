#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix33.h"
#include "Matrix44.h"

class MathLibCore
{
public:
	template<typename T>
	static T lerp(const T& begin, const T& end, float interValue);
};

template<typename T>
inline T MathLibCore::lerp(const T & begin, const T & end, float interValue)
{
	T result;

	result = ((1.0f - interValue) * begin) + (interValue * end);

	return result;
}
