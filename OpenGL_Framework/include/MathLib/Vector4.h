#pragma once
#include <ostream>
#include <math.h>
#include "MathLib/Vector3.h"
#include "Matrix44.h"
#define PI 3.141592635f
class Vector4
{
public:

	//Actual Data
	float x, y, z, q;

	Vector4();
	//Vector4 vec1 = {x,y,z};
	Vector4(Vector3 vec3, float Q);
	Vector4(float X, float Y, float Z, float Q) :x(X), y(Y), z(Z), q(Q) {}
	Vector4(float num) :x(num), y(num), z(num), q(num) {}

	~Vector4();
	//vec1.set(4,5,6)
	void set(float X, float Y, float Z, float Q) { x = X; y = Y; z = Z, q = Q; }

	friend Vector4 operator*(const float scalar, Vector4 vec4)
	{
		///vec1 * 5
		return Vector4((vec4.x*scalar), (vec4.y*scalar), (vec4.z*scalar), (vec4.q*scalar));
	}
	Vector4 operator*(const float scalar) const
	{
		///vec1 * 5
		return Vector4((x*scalar), (y*scalar), (z*scalar), (q*scalar));
	}
	Vector4 operator/(const float divisor) const
	{
		///vec1 * 5
		return Vector4((x/divisor), (y/divisor), (z/divisor), (q/divisor));
	}
	Vector4 operator+(const Vector4 rhs) const
	{
		//vec1 + vec2
		return Vector4((x + rhs.x), (y + rhs.y), (z + rhs.z), (q + rhs.q));
	}
	Vector4 operator-(const Vector4 rhs) const
	{
		//vec1 - vec2
		return Vector4((x - rhs.x), (y - rhs.y), (z - rhs.z), (q - rhs.q));
	}
	Vector4 operator-() const
	{
		//-vec
		return Vector4((-x), (-y), (-z), (-q));
	}
	friend std::ostream& operator<< (std::ostream& out, const Vector4 source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (vec1.x,vec1.y,vec1.z)
		return out << "(" << source.x << "," << source.y << "," << source.z << "," << source.q << ")";
	}
	//3D Functions
	float Length();
	Vector4 Normalize();
	float DotProduct(Vector4 v);
	float ProjLength(Vector4 v);
	Vector4 ProjPoint(Vector4 v);
	float ProjAngle(Vector4 v);
	float DistanceBetweenObj(Vector4 v);
	Vector4 CrossProduct(Vector4 v);
	static Vector4 Zero;

	friend Vector4 operator*(const Matrix44 m, const Vector4 v);
};