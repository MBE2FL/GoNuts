#pragma once
#include <ostream>
#include <math.h>
#include "MathLib/vec2.h"
#include <string>

#define PI 3.141592635f

using std::string;
using std::to_string;

class vec4;

class vec3
{
public:

	//Actual Data
	float x, y, z;

	vec3();
	vec3(vec2 vec2, float Z);
	vec3(float num) : x(num), y(num), z(num) {}
	//vec3 vec1 = {x,y,z};
	vec3(float X, float Y, float Z) :x(X), y(Y), z(Z) {}
	vec3(const vec3& other);
	vec3(const vec4& other);

	~vec3();
	//vec1.set(4,5,6)
	void set(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	friend vec3 operator*(const float scalar, const vec3& rhs)
	{
		///vec1 * 5
		return vec3((rhs.x*scalar), (rhs.y*scalar), (rhs.z*scalar));
	}

	vec3 operator*(const float scalar) const
	{
		///vec1 * 5
		return vec3((x*scalar), (y*scalar), (z*scalar));
	}
	vec3 operator*(const vec3& scalar) const
	{
		///vec1 * 5
		return vec3((x*scalar.x), (y*scalar.y), (z*scalar.z));
	}
	vec3 operator/(const float divisor) const
	{
		///vec1 / 5
		return vec3((x/divisor), (y/divisor), (z/divisor));
	}
	vec3 operator+(const vec3& rhs) const
	{
		//vec1 + vec2
		return vec3((x + rhs.x), (y + rhs.y), (z + rhs.z));
	}
	void operator+=(const vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	vec3 operator-(const vec3& rhs) const
	{
		//vec1 - vec2
		return vec3((x - rhs.x), (y - rhs.y), (z - rhs.z));
	}
	vec3 operator-() const
	{
		//-vec
		return vec3((-x), (-y), (-z));
	}
	bool operator==(const vec3& vector) const
	{
		//-vec
		if (x == vector.x && y == vector.y && z == vector.z)
			return true;
		else
			return false;
	}
	friend std::ostream& operator<< (std::ostream& out, const vec3& source)
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
	float length() const;
	float sqrLength() const;
	void normalize();
	vec3 getNormalized() const;

	static float dot(const vec3& a, const vec3& b);

	/**
		Calculates the scalar projection of vector b onto vector a.
		(a.b)/|a|

		@param a The vector recieving the projection.
		@param b The vector casting the projection.
		@return The scalar projection of b onto a.
	*/
	static float scalarProj(const vec3& a, const vec3& b);

	/**
		Calculates the vector projection of vector b onto vector a.
		(a.b)/|a| * a/|a| => (a.b)/|a|^2 * a

		@param a The vector recieving the projection.
		@param b The vector casting the projection.
		@return The vector projection of b onto a.
	*/
	static vec3 vectorProj(const vec3& a, const vec3& b);


	static float distance(const vec3& a, const vec3& b);
	static float sqrDistance(const vec3& a, const vec3& b);
	static vec3 cross(const vec3& a, const vec3& b);
	string toString() const;

	static vec3 Zero;
	static vec3 One;
};