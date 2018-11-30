#include "PhysicsManager.h"

vector<Collision*> PhysicsManager::_collisions = vector<Collision*>();

void PhysicsManager::addCollision(Collision * collision)
{
	// Check if the collision is already registered with the system
	for (Collision* currCol : _collisions)
	{
		if (currCol == collision)
			return;
	}

	// Register new collision with the system
	_collisions.push_back(collision);
	collision->bodyOne->onCollisionEnter(collision->bodyTwo);
	collision->bodyTwo->onCollisionEnter(collision->bodyOne);
}

bool operator==(const Collision & colOne, const Collision & colTwo)
{
	return (((colOne.bodyOne == colTwo.bodyOne) || (colOne.bodyOne == colTwo.bodyTwo)) &&
		((colOne.bodyTwo == colTwo.bodyOne) || (colOne.bodyTwo == colTwo.bodyTwo)));
}
