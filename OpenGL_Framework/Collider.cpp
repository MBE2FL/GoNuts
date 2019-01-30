#include "Collider.h"

Collider::Collider()
{
	_type = ComponentType::Collider;
	_bounds = nullptr;
	_contactOffset = vec3(0.0f);
	_enabled = true;
}

Collider::~Collider()
{
}

void Collider::onCollisionEnter()
{
}

void Collider::onCollisionStay()
{
}

void Collider::onCollisionExit()
{
}

Bounds * Collider::getBounds() const
{
	return _bounds;
}

void Collider::setBounds(const Bounds & newBounds)
{
	_bounds = new Bounds(newBounds);
}

PhysicsBodyComponent * Collider::getPhysicsBody() const
{
	return _physicsBody;
}

void Collider::setPhysicsBody(PhysicsBodyComponent * physicsBody)
{
	_physicsBody = physicsBody;
}
