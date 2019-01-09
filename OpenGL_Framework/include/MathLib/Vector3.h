#pragma once
#include <ostream>
#include <math.h>
#include "MathLib/Vector2.h"
#define PI 3.141592635f
class Vector3
{
public:

	//Actual Data
	float x, y, z;

	Vector3();
	Vector3(Vector2 vec2, float Z);
	Vector3(float num) : x(num), y(num), z(num) {}
	//Vector3 vec1 = {x,y,z};
	Vector3(float X, float Y, float Z) :x(X), y(Y), z(Z) {}

	~Vector3();
	//vec1.set(4,5,6)
	void set(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	friend Vector3 operator*(const float scalar, Vector3 vec3)
	{
		///vec1 * 5
		return Vector3((vec3.x*scalar), (vec3.y*scalar), (vec3.z*scalar));
	}

	Vector3 operator*(const float scalar) const
	{
		///vec1 * 5
		return Vector3((x*scalar), (y*scalar), (z*scalar));
	}
	Vector3 operator*(const Vector3 scalar) const
	{
		///vec1 * 5
		return Vector3((x*scalar.x), (y*scalar.y), (z*scalar.z));
	}
	Vector3 operator/(const float divisor) const
	{
		///vec1 / 5
		return Vector3((x/divisor), (y/divisor), (z/divisor));
	}
	Vector3 operator+(const Vector3 rhs) const
	{
		//vec1 + vec2
		return Vector3((x + rhs.x), (y + rhs.y), (z + rhs.z));
	}
	Vector3 operator-(const Vector3 rhs) const
	{
		//vec1 - vec2
		return Vector3((x - rhs.x), (y - rhs.y), (z - rhs.z));
	}
	Vector3 operator-() const
	{
		//-vec
		return Vector3((-x), (-y), (-z));
	}
	bool operator==(Vector3 vector) const
	{
		//-vec
		if (x == vector.x && y == vector.y && z == vector.z)
			return true;
		else
			return false;
	}
	friend std::ostream& operator<< (std::ostream& out, const Vector3 source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (vec1.x,vec1.y,vec1.z)
		return out << "(" << source.x << "," << source.y << "," << source.z << ")";
	}
	void operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}
	//3D Functions
	float Length();
	float sqrLength();
	Vector3 Normalize();
	float DotProduct(Vector3 v);
	static float dot(const Vector3& lhs, const Vector3& rhs);
	float ProjLength(Vector3 v);
	Vector3 ProjPoint(Vector3 v);
	float ProjAngle(Vector3 v);
	float DistanceBetweenObj(Vector3 v);
	static float distance(const Vector3& lhs, const Vector3& rhs);
	static float sqrDistance(const Vector3& lhs, const Vector3& rhs);
	Vector3 CrossProduct(Vector3 v);
	static Vector3 Zero;
	static Vector3 One;

};