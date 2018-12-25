#include "System.h"

System::System(EntityManager * entityManager)
{
	_entityManager = entityManager;
}

System::~System()
{
}

void System::update(float deltaTime)
{
}
