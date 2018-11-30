#pragma once


#include "MeshBounds.h"
#include "Transform.h"

class ColliderBounds : public Bounds
{
public:
	ColliderBounds();
	ColliderBounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ);
	ColliderBounds(MeshBounds meshBounds);
	~ColliderBounds() {};

	void update(Transform* transform);
	void updatePoint(Transform* transform);

private:
	Vector3 points[8];
};