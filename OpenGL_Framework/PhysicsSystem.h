#pragma once

#include "System.h"
#include "BoxCollider.h"

using std::find;


struct Collision
{
	PhysicsBodyComponent* bodyOne = nullptr;
	PhysicsBodyComponent* bodyTwo = nullptr;
	bool stillColliding = true;
	Entity* entityOne;
	Entity* entityTwo;
	Collider* colOne;
	Collider* colTwo;

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

	void addCollision(Collision* collision, float deltaTime);
	bool checkAABBCollision(Collider* colliderOne, Collider* colliderTwo);
	bool checkSATCollision(Collider* colliderOne, Collider* colliderTwo);
	void verifyCollisions();

	void calculateFaceNormals(vec3* faceNormals, const vec3* vertices);
	void calculateEdgeNormals(vec3* edgeNormals, vec3* faceNormalsOne, vec3* faceNormalsTwo);
};