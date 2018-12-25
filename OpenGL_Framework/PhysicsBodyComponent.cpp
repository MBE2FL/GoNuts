#include "PhysicsBodyComponent.h"

PhysicsBodyComponent::PhysicsBodyComponent(MeshBounds meshBounds, Vector3 force, Vector3 velocity, Vector3 acceleration, float mass)
	: _force(force), _velocity(velocity), _acceleration(acceleration), _mass(mass)
{
	_collisionBounds = ColliderBounds(meshBounds);
	_type = ComponentType::PhysicsBody;
}

PhysicsBodyComponent::~PhysicsBodyComponent()
{
}

void PhysicsBodyComponent::update(float deltaTime, TransformComponent * transform)
{
	Vector3 gravity(0.0f, EGRAVITY, 0.0f);

	if (_useGravity && _velocity.y > -6.0f)
		_force = _force + gravity;

	if (_velocity.x > 0.0f)
		addForce(Vector3(1.0f*EGRAVITY, 0.0f, 0.0f));
	else if (_velocity.x < 0.0f)
		addForce(Vector3(1.0f*-EGRAVITY, 0.0f, 0.0f));


	_acceleration = _force * 1.0f / _mass;
	_velocity = _velocity + _acceleration * deltaTime;
	//_collisionBounds.setCentre(_collisionBounds.getCentre() + Vector3(velocity, 0.0f) * dt);
	// ####
	//transform->setWorldPosition(transform->getWorldPosition() + Vector3(velocity, 0.0f) * dt);
	_collisionBounds.update(transform);
	// ####

	_force = Vector3(0.0f);
}

void PhysicsBodyComponent::addForce(const Vector3 & force)
{
	_force = _force + force;
}

Vector3 PhysicsBodyComponent::getForce() const
{
	return _force;
}

Vector3 PhysicsBodyComponent::getVelocity() const
{
	return _velocity;
}

Vector3 PhysicsBodyComponent::getAcceleration() const
{
	return _acceleration;
}

Vector3 PhysicsBodyComponent::getPosition() const
{
	return _collisionBounds.getCentre();
}

float PhysicsBodyComponent::getMass() const
{
	return _mass;
}

bool PhysicsBodyComponent::getUseGravity() const
{
	return _useGravity;
}

ColliderBounds PhysicsBodyComponent::getCollisionBounds() const
{
	return _collisionBounds;
}

TTag PhysicsBodyComponent::getTag() const
{
	return _tag;
}

//Tags PhysicsBodyComponent::getTag() const
//{
//	return _tag;
//}

void PhysicsBodyComponent::setForce(const Vector3 & force)
{
	_force = force;
}

void PhysicsBodyComponent::setVelocity(const Vector3 & velocity)
{
	_velocity = velocity;
}

void PhysicsBodyComponent::setAcceleration(const Vector3 & acceleration)
{
	_acceleration = acceleration;
}

void PhysicsBodyComponent::setMass(const float mass)
{
	_mass = mass;
}

void PhysicsBodyComponent::setUseGravity(const bool useGravity)
{
	_useGravity = useGravity;
}

void PhysicsBodyComponent::setTag(const TTag tag)
{
	_tag = tag;
}

//void PhysicsBodyComponent::onCollisionHelper(Tags tag, bool enter)
//{
//	switch (tag)
//	{
//		// Player colliding with platform
//	case Tags::Platform:
//	{
//		if (enter)
//		{
//			_useGravity = false;
//			_canJump = true;
//		}
//		else
//		{
//			_useGravity = true;
//			_canJump = false;
//		}
//		break;
//	}
//	}
//}
