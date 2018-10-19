#pragma once
//#include "mathlib.h"
#include "MiniMath/Core.h"
#include "ColliderBounds.h"
#include "MeshBounds.h"
#include "Transform.h"

class PhysicsBody
{
public:
	PhysicsBody();
	PhysicsBody(MeshBounds meshBounds, vec2 f = vec2::Zero, vec2 v = vec2::Zero, vec2 a = vec2::Zero,
		float m = 1.0f);
	~PhysicsBody();

	bool collision(vec2 position, float w, float h);
	bool collision(PhysicsBody physicsBody);

	void updatePhysicsBody(Transform& transform, float deltaTime);

	void addForce(vec2 f);

	vec2 getForce()const;
	vec2 getVelocity()const;
	vec2 getAcceleration()const;
	vec3 getPosition()const;
	float getMass()const;

	void setForce(vec2 f);
	void setVelocity(vec2 v);
	void setAcceleration(vec2 a);
	void setPosition(vec3 p);
	void setMass(float m);
	void setUseGravity(const bool _useGravity);
	
	ColliderBounds getCollisionBounds() const;

private:
	vec2 force, velocity, acceleration;
	float mass;
	float dt = 0;
	bool useGravity = false;

	ColliderBounds _collisionBounds;
};

