#pragma once
#include "mathlib.h"

class PhysicsBody
{
public:
	PhysicsBody(Vector2 f, Vector2 v, Vector2 a, Vector2 p,
		float m, float w, float h);
	~PhysicsBody();

	bool collision(Vector2 position, float w, float h);
	bool collision(PhysicsBody physicsBody);

	void updatePhysicsBody();

	void addForce(Vector2 f);

	Vector2 getForce()const;
	Vector2 getVelocity()const;
	Vector2 getAcceleration()const;
	Vector2 getPosition()const;
	float getMass()const;

	void setForce(Vector2 f);
	void setVelocity(Vector2 v);
	void setAcceleration(Vector2 a);
	void setPosition(Vector2 p);
	void setMass(float m);
	

private:
	Vector2 force, velocity, acceleration, position;
	float mass, width, height;
	float dt = 0;
};

