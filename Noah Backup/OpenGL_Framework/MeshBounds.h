#pragma once

#include "Bounds.h"

class MeshBounds : public Bounds
{
public:
	MeshBounds() : Bounds()
	{
	};

	MeshBounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ) :
		Bounds(minX, maxX, minY, maxY, minZ, maxZ)
	{

	};

	~MeshBounds() {};
};