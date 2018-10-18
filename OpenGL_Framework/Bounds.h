#pragma once

#include "MiniMath/Core.h"

class Bounds
{
public:
	Bounds();
	Bounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ);
	~Bounds();

	vec3 getCentre() const;
	void setCentre(const vec3& centre);
	vec3 getExtends() const;
	vec3 getMax() const;
	void setMax(const vec3& max);
	vec3 getMin() const;
	void setMin(const vec3& min);
	vec3 getSize() const;
	void setSize(const vec3& size);

private:
	vec3 _centre;
	vec3 _extends;
	vec3 _max;
	vec3 _min;
	vec3 _size;
};