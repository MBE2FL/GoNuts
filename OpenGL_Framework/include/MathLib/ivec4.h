#pragma once
#include <ostream>
#include <math.h>
#include "MathLib/vec3.h"
#include "mat4.h"
#define PI 3.141592635f

class ivec4
{
public:

	//Actual Data
	int x, y, z, w;

	ivec4();
	//vec4 vec1 = {x,y,z};
	ivec4(const vec3& vec3, int Q);
	ivec4(int X, int Y, int Z, int Q) :x(X), y(Y), z(Z), w(Q) {}
	ivec4(int num) :x(num), y(num), z(num), w(num) {}
	ivec4(const ivec4& other);

	~ivec4();
	//vec1.set(4,5,6)
	void set(int X, int Y, int Z, int Q) { x = X; y = Y; z = Z, w = Q; }

	friend ivec4 operator*(const int scalar, const ivec4& vec)
	{
		///vec1 * 5
		return ivec4((vec.x * scalar), (vec.y * scalar), (vec.z * scalar), (vec.w * scalar));
	}
	ivec4 operator*(const int scalar) const
	{
		///vec1 * 5
		return ivec4((x * scalar), (y * scalar), (z * scalar), (w * scalar));
	}
	ivec4 operator/(const int divisor) const
	{
		///vec1 * 5
		return ivec4((x / divisor), (y / divisor), (z / divisor), (w / divisor));
	}
	ivec4 operator+(const ivec4& rhs) const
	{
		//vec1 + vec2
		return ivec4((x + rhs.x), (y + rhs.y), (z + rhs.z), (w + rhs.w));
	}
	ivec4 operator-(const ivec4& rhs) const
	{
		//vec1 - vec2
		return ivec4((x - rhs.x), (y - rhs.y), (z - rhs.z), (w - rhs.w));
	}
	ivec4 operator-() const
	{
		//-vec
		return ivec4((-x), (-y), (-z), (-w));
	}
	friend std::ostream& operator<< (std::ostream& out, const ivec4& source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (vec1.x,vec1.y,vec1.z)
		return out << "(" << source.x << "," << source.y << "," << source.z << "," << source.w << ")";
	}
	//3D Functions
	int length();
	int sqrLength();
	void normalize();
	ivec4 getNormalized() const;
	static int dot(const ivec4& lhs, const ivec4& rhs);
	//int ProjLength(vec4 v);
	//vec4 ProjPoint(vec4 v);
	//int ProjAngle(vec4 v);

	static ivec4 Zero;
	static ivec4 One;
};