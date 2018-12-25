#pragma once

#include "System.h"
#include "TransformComponent.h"
#include "PhysicsBodyComponent.h"
#include <algorithm>

using std::sort;
using std::partition;

class TransformSystem : public System
{
public:
	TransformSystem(EntityManager* entityManager);
	~TransformSystem();

	void update(float deltaTime);

private:
	//bool rootSort(Entity* first, Entity* second);
};