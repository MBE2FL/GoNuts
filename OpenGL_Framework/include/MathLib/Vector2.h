#pragma once
#include <ostream>
#include <math.h>
#define PI 3.141592635
class Vector2
{
public:

	//Actual Data
	float x, y;

	Vector2();
	//Vector2 vec1 = {x,y};
	Vector2(float X = 0.0f, float Y = 0.0f) :x(X), y(Y) {}

	~Vector2();
	//vec1.set(x,y)
	void set(float X, float Y) { x = X; y = Y; }

	Vector2 operator*(const float scalar) const
	{
		//vec1 * 5
		return Vector2((x*scalar), (y*scalar));
	}
	Vector2 operator+(const Vector2 rhs) const
	{
		//vec1 + vec2
		return Vector2((x + rhs.x), (y + rhs.y));
	}
	Vector2 operator-(const Vector2 rhs) const
	{
		//vec1 - vec2
		return Vector2((x - rhs.x), (y - rhs.y));
	}
	friend std::ostream& operator<< (std::ostream& out ,const Vector2 source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (x,y)
		return out << "(" << source.x << "," << source.y << ")";
	}

	//2D Functions
	float Length();
	Vector2 Normalize();
	float DotProduct(Vector2 v);
	float ProjLength(Vector2 v);
	Vector2 ProjPoint(Vector2 v);
	float ProjAngle(Vector2 v);
	float DistanceBetweenObj(Vector2 v);
	Vector2 CrossProduct(Vector2 v);
};