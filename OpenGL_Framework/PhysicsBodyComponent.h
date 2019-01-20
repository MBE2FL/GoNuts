#pragma once

#include "Component.h"
#include "MathLib/MathLibCore.h"
#include "MeshBounds.h"
#include "ColliderBounds.h"
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
	PhysicsBodyComponent() {};
	PhysicsBodyComponent(MeshBounds meshBounds, Vector3 force = Vector3::Zero, Vector3 velocity = Vector3::Zero, Vector3 acceleration = Vector3::Zero,
		float mass = 1.0f);
	~PhysicsBodyComponent();

	void update(float deltaTime, TransformComponent* transform);
	void updateBounds(TransformComponent* transform);
	void addForce(const Vector3& force);
	void addImpluseForce(const Vector3& force);
	//void onCollisionEnter(PhysicsBodyComponent* other);
	//void onCollisionExit(PhysicsBodyComponent* other);

	Vector3 getForce()const;
	Vector3 getVelocity()const;
	Vector3 getMaxVelocity()const;
	Vector3 getAcceleration()const;
	Vector3 getPosition()const;
	float getMass()const;
	bool getUseGravity()const;
	ColliderBounds* getCollisionBounds() const;
	TTag getTag() const;

	void setForce(const Vector3& force);
	void setVelocity(const Vector3& velocity);
	void setMaxVelocity(const Vector3& velocity);
	void setAcceleration(const Vector3& acceleration);
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
	Vector3 _force, _velocity, _maxVelocity = Vector3(5.0f, 8.0f, 0), _acceleration, _impluseForce, _impluseAcceleration;
	float _mass;
	bool _useGravity = false;
	bool _canJump = false;
	TTag _tag;
	ColliderBounds* _collisionBounds;
	bool _active = false;
};