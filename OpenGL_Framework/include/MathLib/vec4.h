#pragma once
#include <ostream>
#include <math.h>
#include "MathLib/vec3.h"
#include "mat4.h"
#define PI 3.141592635f
class vec4
{
public:

	//Actual Data
	float x, y, z, w;

	vec4();
	//vec4 vec1 = {x,y,z};
	vec4(const vec3& vec3, float Q);
	vec4(float X, float Y, float Z, float Q) :x(X), y(Y), z(Z), w(Q) {}
	vec4(float num) :x(num), y(num), z(num), w(num) {}
	vec4(const vec4& other);

	~vec4();
	//vec1.set(4,5,6)
	void set(float X, float Y, float Z, float Q) { x = X; y = Y; z = Z, w = Q; }

	friend vec4 operator*(const float scalar, const vec4& vec)
	{
		///vec1 * 5
		return vec4((vec.x * scalar), (vec.y * scalar), (vec.z * scalar), (vec.w * scalar));
	}
	vec4 operator*(const float scalar) const
	{
		///vec1 * 5
		return vec4((x * scalar), (y * scalar), (z * scalar), (w * scalar));
	}
	vec4 operator*(const vec4 vector) const
	{
		///vec1 * vec2
		return vec4((x * vector.x), (y * vector.y), (z * vector.z), (w * vector.w));
	}
	vec4 operator+=(const vec4 vector) const
	{
		///vec1 * vec2
		return vec4((x + vector.x), (y + vector.y), (z + vector.z), (w + vector.w));
	}
	vec4 operator*=(const vec4 vector) const
	{
		///vec1 * vec2
		return vec4((x * vector.x), (y * vector.y), (z * vector.z), (w * vector.w));
	}
	vec4 operator/(const float divisor) const
	{
		///vec1 * 5
		return vec4((x / divisor), (y / divisor), (z / divisor), (w / divisor));
	}
	vec4 operator+(const vec4& rhs) const
	{
		//vec1 + vec2
		return vec4((x + rhs.x), (y + rhs.y), (z + rhs.z), (w + rhs.w));
	}
	vec4 operator-(const vec4& rhs) const
	{
		//vec1 - vec2
		return vec4((x - rhs.x), (y - rhs.y), (z - rhs.z), (w - rhs.w));
	}
	vec4 operator-() const
	{
		//-vec
		return vec4((-x), (-y), (-z), (-w));
	}
	friend std::ostream& operator<< (std::ostream& out, const vec4& source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (vec1.x,vec1.y,vec1.z)
		return out << "(" << source.x << "," << source.y << "," << source.z << "," << source.w << ")";
	}
	//3D Functions
	float length();
	float sqrLength();
	void normalize();
	vec4 getNormalized() const;
	static float dot(const vec4& lhs, const vec4& rhs);
	//float ProjLength(vec4 v);
	//vec4 ProjPoint(vec4 v);
	//float ProjAngle(vec4 v);

	static vec4 Zero;
	static vec4 One;

	friend vec4 operator*(const mat4& m, const vec4& v);
};