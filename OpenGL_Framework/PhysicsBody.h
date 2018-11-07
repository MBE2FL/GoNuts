#pragma once
//#include "mathlib.h"
//#include "MiniMath/Core.h"
#include "ColliderBounds.h"
#include "MeshBounds.h"
#include "Transform.h"
#include <vector>
#include <iostream>

using std::vector;

#define GRAVITY -4.2f


enum class Tags
{
	Player,
	Platform,
	Enemy,
	PowerUp
};


class PhysicsBody
{
public:
	PhysicsBody();
	PhysicsBody(MeshBounds meshBounds, Vector2 f = Vector2::Zero, Vector2 v = Vector2::Zero, Vector2 a = Vector2::Zero,
		float m = 1.0f);
	~PhysicsBody();

	bool collision(Vector2 position, float w, float h);
	bool collision(PhysicsBody* physicsBody);

	void updatePhysicsBody(Transform* transform, float deltaTime);

	void addForce(Vector2 f);

	Vector2 getForce()const;
	Vector2 getVelocity()const;
	Vector2 getAcceleration()const;
	Vector3 getPosition()const;
	float getMass()const;
	bool getUseGravity()const;

	void setForce(Vector2 f);
	void setVelocity(Vector2 v);
	void setAcceleration(Vector2 a);
	void setPosition(Vector3 p);
	void setMass(float m);
	void setUseGravity(const bool _useGravity);
	
	ColliderBounds getCollisionBounds() const;

	void onCollisionEnter(PhysicsBody* other);
	void onCollisionExit(PhysicsBody* other);

	Tags getTag() const;

private:
	Vector2 force, velocity, acceleration;
	float mass;
	float dt = 0;
	bool useGravity = false;
	bool _canJump = false;
	Tags _tag;



	ColliderBounds _collisionBounds;
	vector<PhysicsBody*> _colliding;


	void onCollisionHelper(Tags tag, bool enter);
};

