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


	vector<Collider*> colliders = _entityManager->getAllColliders();

	vector<Collider*>::iterator it;

	for (it = colliders.begin(); it != colliders.end(); ++it)
	{
		// Get the collider component for the current entity.
		Collider* colliderOne = *it;
		if (!colliderOne || !colliderOne->getEnabled())
			continue;

		// Check if there is another collider.
		// Check current entity's collider with every other entity's collider, behind it in the vector.
		vector<Collider*>::iterator nextIt = it;
		++nextIt;
		while (nextIt != colliders.end())
		{
			// Get the collider component for the other entity.
			Collider* colliderTwo = *nextIt;
			if (!colliderTwo || !colliderTwo->getEnabled())
			{
				++nextIt;
				continue;
			}

			// Check for collisions between physics bodies.
			if (checkSATCollision(colliderOne, colliderTwo))
			{
				// Register the collision with the physics system.
				Collision* collision = new Collision();
				collision->bodyOne = colliderOne->getPhysicsBody();
				collision->bodyTwo = colliderTwo->getPhysicsBody();
				collision->entityOne = colliderOne->getEntity();
				collision->entityTwo = colliderTwo->getEntity();
				collision->colOne = colliderOne;
				collision->colTwo = colliderTwo;
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
			collision->colOne->onCollisionStay(collision->entityOne, collision->entityTwo);
			collision->colTwo->onCollisionStay(collision->entityTwo, collision->entityOne);
			return;
		}
	}

	// Register new collision with the system
	_collisions.push_back(collision);
	// TO-DO onCollisionEnter()
	collision->colOne->onCollisionEnter(collision->entityOne, collision->entityTwo);
	collision->colTwo->onCollisionEnter(collision->entityTwo, collision->entityOne);
}

bool PhysicsSystem::checkAABBCollision(Collider* colliderOne, Collider* colliderTwo)
{
	// Get the bounds of both physics bodies collider bounds.
	static vec3 minOne;
	static vec3 maxOne;

	minOne = colliderOne->_min;
	maxOne = colliderOne->_max;


	static vec3 minTwo;
	static vec3 maxTwo;

	minTwo = colliderTwo->_min;
	maxTwo = colliderTwo->_max;


	// Check if the collider bounds of two physics bodies are colliding.
	return ((minOne.x < maxTwo.x) && (maxOne.x > minTwo.x) &&
		(minOne.y < maxTwo.y) && (maxOne.y > minTwo.y));
}

bool PhysicsSystem::checkSATCollision(Collider * colliderOne, Collider * colliderTwo)
{
	vec3 faceNormalsOne[3];
	const vec3* verticesOne = static_cast<BoxCollider*>(colliderOne)->getVertices();

	vec3 faceNormalsTwo[3];
	const vec3* verticesTwo = static_cast<BoxCollider*>(colliderTwo)->getVertices();

	vec3 allNormals[15];

	// Calculate face, and edge normals.
	calculateFaceNormals(faceNormalsOne, verticesOne);
	calculateFaceNormals(faceNormalsTwo, verticesTwo);
	calculateEdgeNormals(allNormals, faceNormalsOne, faceNormalsTwo);


	float minOne;
	float maxOne;
	float minTwo;
	float maxTwo;

	// Project each vertex onto each normal.
	for (unsigned int i = 0; i < 15; ++i)
	{
		if (allNormals[i] == vec3::Zero)
			continue;

		minOne = vec3::dot(allNormals[i], verticesOne[0]);
		minTwo = vec3::dot(allNormals[i], verticesTwo[0]);

		maxOne = minOne;
		maxTwo = minTwo;

		for (unsigned int j = 1; j < 8; ++j)
		{
			// Don't have to actually use scalar projection of each vertex onto each normal.
			// Since unit vector is not important. ONLY if checking for overlay, not for length of penetration.
			float distOne = vec3::dot(allNormals[i], verticesOne[j]);
			float distTwo = vec3::dot(allNormals[i], verticesTwo[j]);

			// Only care about the max and min of each projection.
			minOne = (distOne < minOne) ? distOne : minOne;
			minTwo = (distTwo < minTwo) ? distTwo : minTwo;

			maxOne = (distOne > maxOne) ? distOne : maxOne;
			maxTwo = (distTwo > maxTwo) ? distTwo : maxTwo;
		}

		//cout << "Max One: " << maxOne << ", Min One: " << minOne << endl;
		//cout << "Max Two: " << maxTwo << ", Min Two: " << minTwo << endl;
		// Check if a pair of projections do NOT overlap. Then no collision occured. No need to check other projections.
		if (!(maxOne >= minTwo && minOne <= maxTwo))
			return false;
	}

	return true;
}

void PhysicsSystem::verifyCollisions()
{
	// Check for any colliders which may have been deleted.
	vector<Collider*> colliders = _entityManager->getAllColliders();
	vector<Collider*>::iterator positionOne;
	vector<Collider*>::iterator positionTwo;

	// Remove any collisions which have ended since the last frame.
	vector<Collision*>::iterator it;

	for (it = _collisions.begin(); it != _collisions.end(); )
	{
		if (!(*it)->stillColliding)
		{
			// Check for any colliders which may have been deleted.
			positionOne = find(colliders.begin(), colliders.end(), (*it)->colOne);
			positionTwo = find(colliders.begin(), colliders.end(), (*it)->colTwo);
			if ((positionOne == colliders.end()) || (positionTwo == colliders.end()))
			{
				delete *it;
				*it = nullptr;
				it = _collisions.erase(it);
				continue;
			}

			(*it)->colOne->onCollisionExit((*it)->entityOne, (*it)->entityTwo);
			(*it)->colTwo->onCollisionExit((*it)->entityTwo, (*it)->entityOne);
			delete *it;
			*it = nullptr;
			it = _collisions.erase(it);
		}
		else
			++it;
	}
}

void PhysicsSystem::calculateFaceNormals(vec3 * faceNormals, const vec3 * vertices)
{
	static vec3 normal;

	// Calculate face normals.

	// front face
	normal = vec3::cross(vertices[0] - vertices[2], vertices[1] - vertices[3]);
	faceNormals[0] = normal;

	// right face
	normal = vec3::cross(vertices[1] - vertices[6], vertices[5] - vertices[2]);
	faceNormals[1] = normal;

	// top face
	normal = vec3::cross(vertices[3] - vertices[6], vertices[2] - vertices[7]);
	faceNormals[2] = normal;
}

void PhysicsSystem::calculateEdgeNormals(vec3 * allNormals, vec3 * faceNormalsOne, vec3 * faceNormalsTwo)
{
	unsigned int index = 6;

	allNormals[0] = faceNormalsOne[0];
	allNormals[1] = faceNormalsOne[1];
	allNormals[2] = faceNormalsOne[2];
	allNormals[3] = faceNormalsTwo[0];
	allNormals[4] = faceNormalsTwo[1];
	allNormals[5] = faceNormalsTwo[2];

	for (unsigned int i = 0; i < 3; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			static vec3 edgeNormal;

			edgeNormal = vec3::cross(faceNormalsOne[i], faceNormalsTwo[j]);

			allNormals[index] = edgeNormal;
			++index;
		}
	}
}

bool Collision::operator==(const Collision & otherCol)
{
	return (((entityOne == otherCol.entityOne) || (entityOne == otherCol.entityTwo)) &&
		((entityTwo == otherCol.entityOne) || (entityTwo == otherCol.entityTwo)));
}
