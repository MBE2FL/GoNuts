#pragma once
#include <ostream>
#include <math.h>
#include "Matrix44.h"
#define PI 3.141592635
class Vector4
{
public:

	//Actual Data
	float x, y, z, q;

	Vector4();
	//Vector4 vec1 = {x,y,z};
	Vector4(float X, float Y, float Z, float Q) :x(X), y(Y), z(Z), q(Q) {}

	~Vector4();
	//vec1.set(4,5,6)
	void set(float X, float Y, float Z, float Q) { x = X; y = Y; z = Z, q = Q; }

	Vector4 operator*(const float scalar) const
	{
		///vec1 * 5
		return Vector4((x*scalar), (y*scalar), (z*scalar), (q*scalar));
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

	friend Vector4 operator*(const Matrix44 m, const Vector4 v);
};