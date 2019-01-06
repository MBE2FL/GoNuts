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



	vector<Entity*> entities = _entityManager->getAllEntitiesWithComponent(ComponentType::PhysicsBody);//gets entities with physics bodies

	vector<Entity*>::iterator it;

	for (it = entities.begin(); it != entities.end(); it++)
	{
		// Get the physics body component for the current entity.
		//Component* component = _entityManager->getComponent(ComponentType::PhysicsBody, *it);
		PhysicsBodyComponent* physicsBodyOne = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, *it);
		if (!physicsBodyOne)
			return;

		// Check if there is another physics body.
		// Check current entity's physics body with every other entity's physics body, behind it in the vector.
		vector<Entity*>::iterator nextIt = it;
		nextIt++;
		while (nextIt != entities.end())
		{
			// Get the physics body component for the other entity.
			//component = _entityManager->getComponent(ComponentType::PhysicsBody, *nextIt);
			PhysicsBodyComponent* physicsBodyTwo = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, *nextIt);
			if (!physicsBodyTwo)
				return;

			// Check for collisions between physics bodies.
			if (checkCollision(physicsBodyOne, physicsBodyTwo))
			{
				// Register the collision with the physics system.
				Collision* collision = new Collision();
				collision->bodyOne = physicsBodyOne;
				collision->bodyTwo = physicsBodyTwo;
				collision->entityOne = *it;
				collision->entityTwo = *nextIt;
				addCollision(collision);
			}

			nextIt++;
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
	ColliderBounds bounds;

	bounds = physicsBodyOne->getCollisionBounds();
	Vector3 positionOne = bounds.getCentre();
	Vector3 minOne = positionOne - bounds.getExtends();
	Vector3 maxOne = positionOne + bounds.getExtends();

	bounds = physicsBodyTwo->getCollisionBounds();
	Vector3 positionTwo = bounds.getCentre();
	Vector3 minTwo = positionTwo - bounds.getExtends();
	Vector3 maxTwo = positionTwo + bounds.getExtends();


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
