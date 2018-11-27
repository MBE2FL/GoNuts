#pragma once
#include "PhysicsBody.h"


struct Collision
{
	PhysicsBody* bodyOne;
	PhysicsBody* bodyTwo;
};


class PhysicsManager
{
public:
	static void addCollision(Collision* collision);
	static void update(float deltaTime);

	friend bool operator==(const Collision& colOne, const Collision& colTwo);

private:
	static vector<Collision*> _collisions;
};