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

	template<typename T>
	static T invLerp(const T &v, const T &v0, const T &v1);

	template<typename T>
	static T catmull(const T p0, const T p1, const T p2, const T p3, const float interValue);
};

template<typename T>
inline T MathLibCore::lerp(const T & begin, const T & end, float interValue)
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
inline T MathLibCore::invLerp(const T &v, const T &v0, const T &v1)
{
	return ((v - v0) / (v1 - v0));
}
template<typename T>
inline T MathLibCore::catmull(const T p0, const T p1, const T p2, const T p3, float t)
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
