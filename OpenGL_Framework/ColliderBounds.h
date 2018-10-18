#pragma once

#include "Bounds.h"
#include "Transform.h"

class ColliderBounds : public Bounds
{
public:
	ColliderBounds();
	ColliderBounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ);
	~ColliderBounds() {};

	void update(const Transform& transform);
	void updatePoints(const Transform& transform);
	void updatePoint(const Transform& transform);

private:
	vec3 points[8];
};