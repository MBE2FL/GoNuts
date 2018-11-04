#pragma once

//#include "MiniMath/Core.h"
#include "MathLib/MathLibCore.h"

class Bounds
{
public:
	Bounds();
	Bounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ);
	~Bounds();

	Vector3 getCentre() const;
	void setCentre(const Vector3& centre);
	Vector3 getExtends() const;
	Vector3 getMax() const;
	void setMax(const Vector3& max);
	Vector3 getMin() const;
	void setMin(const Vector3& min);
	Vector3 getSize() const;
	void setSize(const Vector3& size);

private:
	Vector3 _centre;
	Vector3 _extends;
	Vector3 _max;
	Vector3 _min;
	Vector3 _size;
};