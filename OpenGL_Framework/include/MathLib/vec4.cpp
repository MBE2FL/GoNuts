#include "vec4.h"

vec4 vec4::Zero = vec4(0.0f);
vec4 vec4::One = vec4(1.0f);

vec4::vec4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

vec4::vec4(const vec3& vec3, float Q)
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
	w = Q;
}

vec4::vec4(const vec4 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

vec4::~vec4()
{
}

float vec4::length()
{
	return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

float vec4::sqrLength()
{
	return ((x * x) + (y * y) + (z * z) + (w * w));
}

float vec4::dot(const vec4 & lhs, const vec4 & rhs)
{
	return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w));
}

void vec4::normalize()
{
	float _length = length();

	x /= _length;
	y /= _length;
	z /= _length;;
}

vec4 vec4::getNormalized() const
{
	vec4 result(*this);
	result.normalize();

	return result;
}

vec4 operator*(const mat4& m, const vec4& v)
{
	vec4 result;

	result.x = m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w;
	result.y = m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w;
	result.z = m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w;
	result.w = m.data[4] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15] * v.w;

	return result;
}
