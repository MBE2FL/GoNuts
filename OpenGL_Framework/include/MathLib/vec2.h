#pragma once
#include <ostream>
#include <math.h>
#define PI 3.141592635f
class vec2
{
public:

	//Actual Data
	float x, y;

	//vec2 vec1 = {x,y};
	vec2(float num) : x(num), y(num) {}
	vec2(float X = 0.0f, float Y = 0.0f) :x(X), y(Y) {}

	~vec2();
	//vec1.set(x,y)
	void set(float X, float Y) { x = X; y = Y; }

	friend vec2 operator*(const float scalar, vec2& rhs)
	{
		//vec1 * 5
		return vec2((rhs.x*scalar), (rhs.y*scalar));
	}

	vec2 operator*(const float scalar) const
	{
		//vec1 * 5
		return vec2((x*scalar), (y*scalar));
	}
	vec2 operator/(const float divisor) const
	{
		//vec1 / 5
		return vec2((x/divisor), (y/divisor));
	}
	vec2 operator+(const vec2& rhs) const
	{
		//vec1 + vec2
		return vec2((x + rhs.x), (y + rhs.y));
	}
	vec2 operator-(const vec2& rhs) const
	{
		//vec1 - vec2
		return vec2((x - rhs.x), (y - rhs.y));
	}
	vec2 operator-() const
	{
		//vec1 - vec2
		return vec2((-x), (-y));
	}
	friend std::ostream& operator<< (std::ostream& out ,const vec2 source)
	{
		//Example: std::cout << vec1 << std::endl
		//output in cmd: (x,y)
		return out << "(" << source.x << "," << source.y << ")";
	}

	//2D Functions
	float Length();
	vec2 Normalize();
	float DotProduct(vec2& v);
	float ProjLength(vec2& v);
	vec2 ProjPoint(vec2& v);
	float ProjAngle(vec2& v);
	float DistanceBetweenObj(vec2& v);
	vec2 CrossProduct(vec2& v);
	static vec2 Zero;
	static vec2 One;
};