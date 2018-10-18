#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(Vector2 f, Vector2 v, Vector2 a, Vector2 p, float m, float w, float h)
{
	force = f;
	velocity = v;
	acceleration = a;
	position = p;
	mass = m;
	width = w;
	height = h;
}

PhysicsBody::~PhysicsBody()
{
}

bool PhysicsBody::collision(Vector2 p, float w, float h)
{
	if (position.x - width / 2.0f < p.x + w / 2.0f &&
		position.x + width / 2.0f > p.x - w / 2.0f &&
		position.y - height / 2.0f < p.y + h / 2.0f &&
		position.y + height / 2.0f > p.y - h / 2.0f)
	{
		return true;
	}
	else
		return false;
}

bool PhysicsBody::collision(PhysicsBody physicsBody)
{
	if (position.x - width / 2.0f < physicsBody.position.x + physicsBody.width / 2.0f &&
		position.x + width / 2.0f > physicsBody.position.x - physicsBody.width / 2.0f &&
		position.y - height / 2.0f < physicsBody.position.y + physicsBody.height / 2.0f &&
		position.y + height / 2.0f > physicsBody.position.y - physicsBody.height / 2.0f)
	{
		return true;
	}
	else
		return false;
}

void PhysicsBody::updatePhysicsBody()
{
	Vector2 gravity(0, 0.981f);
	force = force + gravity;

	acceleration = force * 1.0f / mass;
	velocity = velocity + acceleration * dt;
	position = position + velocity * dt;

	force = Vector2(0, 0);
}

void PhysicsBody::addForce(Vector2 f)
{
	force = force + f;
}

Vector2 PhysicsBody::getForce() const
{
	return force;
}

Vector2 PhysicsBody::getVelocity() const
{
	return velocity;
}

Vector2 PhysicsBody::getAcceleration() const
{
	return acceleration;
}

Vector2 PhysicsBody::getPosition() const
{
	return position;
}

float PhysicsBody::getMass() const
{
	return mass;
}

void PhysicsBody::setForce(Vector2 f)
{
	force = f;
}

void PhysicsBody::setVelocity(Vector2 v)
{
	velocity = v;
}

void PhysicsBody::setAcceleration(Vector2 a)
{
	acceleration = a;
}

void PhysicsBody::setPosition(Vector2 p)
{
	position = p;
}

void PhysicsBody::setMass(float m)
{
	mass = m;
}
