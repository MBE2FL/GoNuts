#pragma once
#include <ostream>
#include <math.h>
#define PI 3.141592635
class Vector3
{
public:

	//Actual Data
	float x, y, z;

	Vector3();
	//Vector3 vec1 = {x,y,z};
	Vector3(float X, float Y, float Z) :x(X), y(Y), z(Z) {}

	~Vector3();
	//vec1.set(4,5,6)
	void set(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	Vector3 operator*(const float scalar) const
	{
		///vec1 * 5
		return Vector3((x*scalar), (y*scalar), (z*scalar));
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
	friend std::ostream& operator<< (std::ostream& out, const Vector3 source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (vec1.x,vec1.y,vec1.z)
		return out << "(" << source.x << "," << source.y << "," << source.z << ")";
	}
	//3D Functions
	float Length(Vector3 v1);
	Vector3 Normalize(Vector3 v1);
	float DotProduct(Vector3 v1, Vector3 v2);
	float ProjLength(Vector3 v1, Vector3 v2);
	Vector3 ProjPoint(Vector3 v1, Vector3 v2);
	float ProjAngle(Vector3 v1, Vector3 v2);
	float DistanceBetweenObj(Vector3 v1, Vector3 v2);
	Vector3 CrossProduct(Vector3 v1, Vector3 v2);















};