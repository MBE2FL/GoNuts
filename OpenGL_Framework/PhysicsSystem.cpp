#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(EntityManager * entityManager)
	: System(entityManager)
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update(float deltaTime)
{
	// Reset all collisions' 'stillColliding' variable. To check which collisions are still colliding, or
	// have exited, since the last frame.
	for (Collision* collision : _collisions)
	{
		collision->stillColliding = false;
	}


	vector<PhysicsBodyComponent*> physicsBodies = _entityManager->getAllPhysicsBodyComponents();

	vector<PhysicsBodyComponent*>::iterator it;

	for (it = physicsBodies.begin(); it != physicsBodies.end(); ++it)
	{
		// Get the physics body component for the current entity.
		PhysicsBodyComponent* physicsBodyOne = *it;
		if (!physicsBodyOne || !physicsBodyOne->getActive())
			continue;

		// Check if there is another physics body.
		// Check current entity's physics body with every other entity's physics body, behind it in the vector.
		vector<PhysicsBodyComponent*>::iterator nextIt = it;
		++nextIt;
		while (nextIt != physicsBodies.end())
		{
			// Get the physics body component for the other entity.
			//component = _entityManager->getComponent(ComponentType::PhysicsBody, *nextIt);
			PhysicsBodyComponent* physicsBodyTwo = *nextIt;
			if (!physicsBodyTwo || !physicsBodyTwo->getActive())
			{
				++nextIt;
				continue;
			}

			// Check for collisions between physics bodies.
			if (checkCollision(physicsBodyOne, physicsBodyTwo))
			{
				// Register the collision with the physics system.
				Collision* collision = new Collision();
				collision->bodyOne = physicsBodyOne;
				collision->bodyTwo = physicsBodyTwo;
				collision->entityOne = physicsBodyOne->getEntity();
				collision->entityTwo = physicsBodyTwo->getEntity();
				addCollision(collision);
			}

			++nextIt;
		}
	}


	// Check for any collisions which have ended since the last frame.
	verifyCollisions();
}

void PhysicsSystem::addCollision(Collision * collision)
{
	// Check if the collision is already registered with the system
	for (Collision* currCol : _collisions)
	{
		if (*currCol == *collision)
		{
			currCol->stillColliding = true;
			// TO-DO call onCollisionStay()
			if (collision->bodyOne->onCollisionStay)
				collision->bodyOne->onCollisionStay(collision->entityOne, collision->entityTwo);
			if (collision->bodyTwo->onCollisionStay)
				collision->bodyTwo->onCollisionStay(collision->entityTwo, collision->entityOne);
			return;
		}
	}

	// Register new collision with the system
	_collisions.push_back(collision);
	// TO-DO onCollisionEnter()
	if (collision->bodyOne->onCollisionEnter)
		collision->bodyOne->onCollisionEnter(collision->entityOne, collision->entityTwo);
	if (collision->bodyTwo->onCollisionEnter)
		collision->bodyTwo->onCollisionEnter(collision->entityTwo, collision->entityOne);
}

bool PhysicsSystem::checkCollision(PhysicsBodyComponent * physicsBodyOne, PhysicsBodyComponent * physicsBodyTwo)
{
	// Get the bounds of both physics bodies collider bounds.
	ColliderBounds* bounds = nullptr;

	bounds = physicsBodyOne->getCollisionBounds();
	static Vector3 positionOne;
	static Vector3 minOne;
	static Vector3 maxOne;

	positionOne = bounds->getCentre();
	minOne = positionOne - bounds->getExtends();
	maxOne = positionOne + bounds->getExtends();

	bounds = physicsBodyTwo->getCollisionBounds();
	static Vector3 positionTwo;
	static Vector3 minTwo;
	static Vector3 maxTwo;

	positionTwo = bounds->getCentre();
	minTwo = positionTwo - bounds->getExtends();
	maxTwo = positionTwo + bounds->getExtends();


	// Check if the collider bounds of two physics bodies are colliding.
	return ((minOne.x < maxTwo.x) && (maxOne.x > minTwo.x) &&
		(minOne.y < maxTwo.y) && (maxOne.y > minTwo.y));
}

void PhysicsSystem::verifyCollisions()
{
	// Remove any collisions which have ended since the last frame.
	vector<Collision*>::iterator it;

	for (it = _collisions.begin(); it != _collisions.end(); )
	{
		if (!(*it)->stillColliding)
		{
			// TO-DO call onCollisionExit()
			if ((*it)->bodyOne->onCollisionExit)
				(*it)->bodyOne->onCollisionExit((*it)->entityOne, (*it)->entityTwo);
			if ((*it)->bodyTwo->onCollisionExit)
				(*it)->bodyTwo->onCollisionExit((*it)->entityTwo, (*it)->entityOne);
			delete *it;
			*it = nullptr;
			it = _collisions.erase(it);
		}
		else
			++it;
	}
}

bool Collision::operator==(const Collision & otherCol)
{
	return (((bodyOne == otherCol.bodyOne) || (bodyOne == otherCol.bodyTwo)) &&
		((bodyTwo == otherCol.bodyOne) || (bodyTwo == otherCol.bodyTwo)));
}
