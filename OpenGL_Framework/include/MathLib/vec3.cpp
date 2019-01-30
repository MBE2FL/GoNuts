#include "vec3.h"
#include "vec4.h"

vec3 vec3::Zero = vec3(0.0f);
vec3 vec3::One = vec3(1.0f);

vec3::vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3::vec3(vec2 vec2, float Z)
{
	x = vec2.x;
	y = vec2.y;
	z = Z;
}

vec3::vec3(const vec3 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

vec3::vec3(const vec4 & other)
	: x(other.x), y(other.y), z(other.z)
{
}

vec3::~vec3()
{
}

float vec3::length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

float vec3::sqrLength() const
{
	return ((x * x) + (y * y) + (z * z));
}

void vec3::normalize()
{
	float _length = length();

	x /= _length;
	y /= _length;
	z /= _length;
}

vec3 vec3::getNormalized() const
{
	vec3 result(*this);
	result.normalize();

	return result;
}

float vec3::dot(const vec3 & a, const vec3 & b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

//float vec3::ProjLength(const vec3 & v)
//{
//	return (DotProduct(v) / Length());
//}
//
//vec3 vec3::ProjPoint(const vec3 & v)
//{
//	return vec3(((DotProduct(v) / (Length() * Length())) * x)
//				 , ((DotProduct(v) / (Length() * Length())) * y)
//				 , ((DotProduct(v) / (Length() * Length())) * z));
//}
//
//float vec3::ProjAngle(const vec3 & v)
//{
//	return ((acosf(DotProduct(v) / (Length() * v.Length()))) *(180 / PI));
//
//}

float vec3::scalarProj(const vec3 & a, const vec3 & b)
{
	return dot(a, b) / a.length();
}

vec3 vec3::vectorProj(const vec3 & a, const vec3 & b)
{
	return dot(a, b) / a.sqrLength() * a;
}

float vec3::distance(const vec3 & a, const vec3 & b)
{
	return (b - a).length();
}

float vec3::sqrDistance(const vec3 & a, const vec3 & b)
{
	return (b - a).sqrLength();
}

vec3 vec3::cross(const vec3 & a, const vec3 & b)
{
	return vec3(((a.y * b.z) - (a.z * b.y)), ((a.z * b.x) - (a.x * b.z)), ((a.x * b.y) - (a.y * b.x)));
}

string vec3::toString() const
{
	string output = "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
	return output;
}
