#include "ivec4.h"

ivec4 ivec4::Zero = ivec4(0);
ivec4 ivec4::One = ivec4(1);

ivec4::ivec4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

ivec4::ivec4(const vec3& vec3, int Q)
{
	x = static_cast<int>(vec3.x);
	y = static_cast<int>(vec3.y);
	z = static_cast<int>(vec3.z);
	w = Q;
}

ivec4::ivec4(const ivec4 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

ivec4::~ivec4()
{
}

int ivec4::length()
{
	return static_cast<int>(sqrt((x * x) + (y * y) + (z * z) + (w * w)));
}

int ivec4::sqrLength()
{
	return ((x * x) + (y * y) + (z * z) + (w * w));
}

int ivec4::dot(const ivec4 & lhs, const ivec4 & rhs)
{
	return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w));
}

void ivec4::normalize()
{
	int _length = length();

	x /= _length;
	y /= _length;
	z /= _length;;
}

ivec4 ivec4::getNormalized() const
{
	ivec4 result(*this);
	result.normalize();

	return result;
}
