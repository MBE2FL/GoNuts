#pragma once
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "ivec4.h"
#include "mat3.h"
#include "mat4.h"
#include "Quaternion.h"

namespace MathUtils
{
	template<typename T>
	T lerp(const T& begin, const T& end, float interValue);

	template<typename T>
	T invLerp(const T &v, const T &v0, const T &v1);

	template<typename T>
	T catmull(const T p0, const T p1, const T p2, const T p3, const float interValue);

	float toRadians(const float degrees);
	float toDegrees(const float radians);
}

//class MathLibCore
//{
//public:
//	template<typename T>
//	static T lerp(const T& begin, const T& end, float interValue);
//
//	template<typename T>
//	static T invLerp(const T &v, const T &v0, const T &v1);
//
//	template<typename T>
//	static T catmull(const T p0, const T p1, const T p2, const T p3, const float interValue);
//
//	static float toRadians(const float degrees);
//	static float toDegrees(const float radians);
//};

template<typename T>
inline T MathUtils::lerp(const T & begin, const T & end, float interValue)
{
	if (interValue < 0)
		interValue = 0;
	if (interValue > 1)
		interValue = 1;
	T result;

	result = ((1.0f - interValue) * begin) + (interValue * end);

	return result;
}

template<typename T>
inline T MathUtils::invLerp(const T &v, const T &v0, const T &v1)
{
	return ((v - v0) / (v1 - v0));
}
template<typename T>
inline T MathUtils::catmull(const T p0, const T p1, const T p2, const T p3, float t)
{
	if (t < 0)
		t = 0;
	if (t > 1)
		t = 1;
	return 0.5f *
		(t * t * t * (		-p0 + 3.0f * p1 -3.0f * p2 + p3) +
			 t * t * (2.0f * p0 - 5.0f * p1 + 4.0f * p2- p3) +
				 t * (		-p0 +					 p2	   ) +
					 (			  2.0f * p1				   ));	
}
