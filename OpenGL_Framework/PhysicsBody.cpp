#include "PhysicsBody.h"

PhysicsBody::PhysicsBody()
{
}

PhysicsBody::PhysicsBody(MeshBounds meshBounds, vec2 f, vec2 v, vec2 a, float m)
{
	force = f;
	velocity = v;
	acceleration = a;
	mass = m;

	_collisionBounds = ColliderBounds(meshBounds);
}

PhysicsBody::~PhysicsBody()
{
}

bool PhysicsBody::collision(vec2 p, float w, float h)
{
	vec2 position = vec2(_collisionBounds.getCentre().x, _collisionBounds.getCentre().y);
	float width = _collisionBounds.getExtends().x;
	float height = _collisionBounds.getExtends().y;

	if (position.x - width < p.x + w / 2.0f &&
		position.x + width > p.x - w / 2.0f &&
		position.y - height < p.y + h / 2.0f &&
		position.y + height > p.y - h / 2.0f)
	{
		return true;
	}
	else
		return false;
}

bool PhysicsBody::collision(PhysicsBody physicsBody)
{
	vec2 position = vec2(_collisionBounds.getCentre().x, _collisionBounds.getCentre().y);
	float width = _collisionBounds.getExtends().x;
	float height = _collisionBounds.getExtends().y;

	ColliderBounds otherBounds = physicsBody.getCollisionBounds();
	vec2 otherPosition = vec2(otherBounds.getCentre().x, otherBounds.getCentre().y);
	float otherWidth = otherBounds.getExtends().x;
	float otherHeight = otherBounds.getExtends().y;

	if (position.x - width < otherPosition.x + otherWidth &&
		position.x + width > otherPosition.x - otherWidth &&
		position.y - height < otherPosition.y + otherHeight &&
		position.y + height > otherPosition.y - otherHeight)
	{
		return true;
	}
	else
		return false;
}

void PhysicsBody::updatePhysicsBody(Transform& transform, float deltaTime)
{
	dt = deltaTime;
	vec2 gravity(0, GRAVITY);

	if (useGravity && velocity.y > -4.0f)
		force = force + gravity;

	if (velocity.x > 0)
		addForce(vec2(2.2f*GRAVITY, 0.0f));
	else if (velocity.x < 0)
		addForce(vec2(2.2f*-GRAVITY, 0.0f));


	acceleration = force * 1.0f / mass;
	velocity = velocity + acceleration * dt;
	//_collisionBounds.setCentre(_collisionBounds.getCentre() + vec3(velocity, 0.0f) * dt);
	transform.setPosition(transform.getPosition() + vec3(velocity, 0.0f) * dt);
	_collisionBounds.update(transform);


	force = vec2(0, 0);
}

void PhysicsBody::addForce(vec2 f)
{
	force = force + f;
}

vec2 PhysicsBody::getForce() const
{
	return force;
}

vec2 PhysicsBody::getVelocity() const
{
	return velocity;
}

vec2 PhysicsBody::getAcceleration() const
{
	return acceleration;
}

vec3 PhysicsBody::getPosition() const
{
	return _collisionBounds.getCentre();
}

float PhysicsBody::getMass() const
{
	return mass;
}

void PhysicsBody::setForce(vec2 f)
{
	force = f;
}

void PhysicsBody::setVelocity(vec2 v)
{
	velocity = v;
}

void PhysicsBody::setAcceleration(vec2 a)
{
	acceleration = a;
}

void PhysicsBody::setPosition(vec3 p)
{
	_collisionBounds.setCentre(p);
}

void PhysicsBody::setMass(float m)
{
	mass = m;
}

void PhysicsBody::setUseGravity(const bool _useGravity)
{
	useGravity = _useGravity;
}

ColliderBounds PhysicsBody::getCollisionBounds() const
{
	return _collisionBounds;
}
