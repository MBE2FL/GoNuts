#pragma once

#include "System.h"
#include "TransformComponent.h"
#include "PhysicsBodyComponent.h"
#include <algorithm>

using std::sort;
using std::partition;

#define CULL_DISTANCE 1400.0f

class TransformSystem : public System
{
public:
	TransformSystem(EntityManager* entityManager);
	~TransformSystem();

	void update(float deltaTime);

private:
};