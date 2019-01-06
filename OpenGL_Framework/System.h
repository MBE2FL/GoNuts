#pragma once

#include "EntityManager.h"

class System
{
public:
	System(EntityManager* entityManager);
	virtual ~System();

	virtual void update(float deltaTime);

protected:
	EntityManager* _entityManager;
};