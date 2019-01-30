#pragma once

#include "TransformComponent.h"
////#include "MathLib/MathLibCore.h"
#include <iostream>
#include "Entity.h"

//#define GRAVITY -16.2f
#define EGRAVITY -9.81f

//struct Collision
//{
//	PhysicsBodyComponent* bodyOne;
//	PhysicsBodyComponent* bodyTwo;
//	bool stillColliding = true;
//	Entity* entityOne;
//	Entity* entityTwo;
//
//	bool operator==(const Collision& otherCol);
//};

enum class TTag
{
	Player,
	Platform,
	Enemy,
	Coin,
	Acorn,
	Spike,
	TopPlatform
};

class PhysicsBodyComponent : public Component
{
public:
	PhysicsBodyComponent(vec3 force = vec3(0.0f), vec3 velocity = vec3(0.0f), vec3 acceleration = vec3(0.0f),
		float mass = 1.0f);
	~PhysicsBodyComponent();

	void update(float deltaTime, TransformComponent* transform);
	void addForce(const vec3& force);
	void addImpluseForce(const vec3& force);
	//void onCollisionEnter(PhysicsBodyComponent* other);
	//void onCollisionExit(PhysicsBodyComponent* other);

	vec3 getForce()const;
	vec3 getVelocity()const;
	vec3 getMaxVelocity()const;
	vec3 getAcceleration()const;
	float getMass()const;
	bool getUseGravity()const;
	TTag getTag() const;

	void setForce(const vec3& force);
	void setVelocity(const vec3& velocity);
	void setMaxVelocity(const vec3& velocity);
	void setAcceleration(const vec3& acceleration);
	void setMass(const float mass);
	void setUseGravity(const bool useGravity);
	void setTag(const TTag tag);

	bool getCanJump();
	void setCanJump(bool jump);
	bool getActive() const;
	void setActive(const bool active);


	// Testing function pointers
	typedef void(*onCollision)(Entity* self, Entity* other);
	onCollision onCollisionEnter;
	onCollision onCollisionStay;
	onCollision onCollisionExit;

private:
	vec3 _force, _velocity, _maxVelocity = vec3(5.0f, 8.0f, 0), _acceleration, _impluseForce, _impluseAcceleration;
	float _mass;
	bool _useGravity = false;
	bool _canJump = false;
	TTag _tag;
	bool _active = false;
};