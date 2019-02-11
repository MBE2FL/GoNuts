#include "PhysicsBodyComponent.h"

PhysicsBodyComponent::PhysicsBodyComponent(vec3 force, vec3 velocity, vec3 acceleration, float mass)
	: _force(force), _velocity(velocity), _acceleration(acceleration), _mass(mass)
{
	_type = ComponentType::PhysicsBody;
}

PhysicsBodyComponent::~PhysicsBodyComponent()
{
}

void PhysicsBodyComponent::update(float deltaTime, TransformComponent * transform)
{
	vec3 gravity(0.0f, EGRAVITY, 0.0f);

	if(_useGravity)
		_force += gravity;

	//if (_velocity.x > 0.0f)
	//	addForce(vec3(1.0f*EGRAVITY, 0.0f, 0.0f));
	//else if (_velocity.x < 0.0f)
	//	addForce(vec3(1.0f*-EGRAVITY, 0.0f, 0.0f));


	_acceleration = _force * 1.0f / _mass;
	_impluseAcceleration = _impluseForce * 1.0f / _mass;
	_velocity += _acceleration * deltaTime + _impluseAcceleration;
	_velocity.x = std::fminf(_velocity.x, _maxVelocity.x);
	_velocity.y = std::fminf(_velocity.y, _maxVelocity.y);
	_velocity.z = std::fminf(_velocity.z, _maxVelocity.z);


	_force = vec3(0.0f);
	_impluseForce = vec3(0.0f);
}

void PhysicsBodyComponent::addForce(const vec3 & force)
{
	_force += force;
}

void PhysicsBodyComponent::addImpluseForce(const vec3 & force)
{
	_impluseForce += force;
}

vec3 PhysicsBodyComponent::getForce() const
{
	return _force;
}

vec3 PhysicsBodyComponent::getVelocity() const
{
	return _velocity;
}

vec3 PhysicsBodyComponent::getMaxVelocity() const
{
	return _maxVelocity;
}

vec3 PhysicsBodyComponent::getAcceleration() const
{
	return _acceleration;
}

float PhysicsBodyComponent::getMass() const
{
	return _mass;
}

bool PhysicsBodyComponent::getUseGravity() const
{
	return _useGravity;
}

//Tags PhysicsBodyComponent::getTag() const
//{
//	return _tag;
//}

void PhysicsBodyComponent::setForce(const vec3 & force)
{
	_force = force;
}

void PhysicsBodyComponent::setVelocity(const vec3 & velocity)
{
	_velocity = velocity;
}

void PhysicsBodyComponent::setMaxVelocity(const vec3 & velocity)
{
	_maxVelocity = velocity;
}

void PhysicsBodyComponent::setAcceleration(const vec3 & acceleration)
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

bool PhysicsBodyComponent::getCanJump()
{
	return _canJump;
}

void PhysicsBodyComponent::setCanJump(bool jump)
{
	_canJump = jump;
}

bool PhysicsBodyComponent::getActive() const
{
	return _active;
}

void PhysicsBodyComponent::setActive(const bool active)
{
	_active = active;
}
