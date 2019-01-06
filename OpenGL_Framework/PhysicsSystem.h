#pragma once

#include "System.h"
#include "PhysicsBodyComponent.h"


struct Collision
{
	PhysicsBodyComponent* bodyOne;
	PhysicsBodyComponent* bodyTwo;
	bool stillColliding = true;
	Entity* entityOne;
	Entity* entityTwo;

	bool operator==(const Collision& otherCol);
};

class PhysicsSystem : public System
{
public:
	PhysicsSystem(EntityManager* entityManager);
	~PhysicsSystem();

	void update(float deltaTime);

private:
	vector<Collision*> _collisions;

	void addCollision(Collision* collision);
	bool checkCollision(PhysicsBodyComponent* physicsBodyOne, PhysicsBodyComponent* physicsBodyTwo);
	void verifyCollisions();
};