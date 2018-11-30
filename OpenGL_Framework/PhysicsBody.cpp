#include "PhysicsBody.h"

PhysicsBody::PhysicsBody()
{
}

PhysicsBody::PhysicsBody(MeshBounds meshBounds, Vector2 f, Vector2 v, Vector2 a, float m)
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

bool PhysicsBody::collision(Vector2 p, float w, float h)
{
	Vector2 position = Vector2(_collisionBounds.getCentre().x, _collisionBounds.getCentre().y);
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

bool PhysicsBody::collision(PhysicsBody* physicsBody)
{
	Vector2 position = Vector2(_collisionBounds.getCentre().x, _collisionBounds.getCentre().y);
	float width = _collisionBounds.getExtends().x;
	float height = _collisionBounds.getExtends().y;

	ColliderBounds otherBounds = physicsBody->getCollisionBounds();
	Vector2 otherPosition = Vector2(otherBounds.getCentre().x, otherBounds.getCentre().y);
	float otherWidth = otherBounds.getExtends().x;
	float otherHeight = otherBounds.getExtends().y;

	if (position.x - width < otherPosition.x + otherWidth &&
		position.x + width > otherPosition.x - otherWidth &&
		position.y - height < otherPosition.y + otherHeight &&
		position.y + height > otherPosition.y - otherHeight)
	{
		setUseGravity(false);

		if (position.y - (height * 0.8f) >= otherPosition.y + otherHeight &&
			position.y - height <= otherPosition.y + otherHeight)
		{
			if (getVelocity().y <= -0.01f)
			{
				float ySpeed = getVelocity().y;
				addForce(Vector2(0.0f, -ySpeed / dt));
				
			}
		}
		else if (position.x - width < otherPosition.x + otherWidth &&
			position.x + width > otherPosition.x + otherWidth &&
			getVelocity().x < -0.1f)
		{
			float xSpeed = getVelocity().x;
			addForce(Vector2(-xSpeed / (dt * 0.6f), 0.0f));	
		}
		else if (position.x + width > otherPosition.x - otherWidth &&
			position.x - width < otherPosition.x + otherWidth &&
			getVelocity().x > 0.1f)
		{
			float xSpeed = getVelocity().x;
			addForce(Vector2(-xSpeed / (dt * 0.6f), 0.0f));
		}
		return true;
	}
	else
	{
		setUseGravity(true);
		return false;
	}
}

bool PhysicsBody::spikeCollision(PhysicsBody * physicsBody)
{
	Vector2 position = Vector2(_collisionBounds.getCentre().x, _collisionBounds.getCentre().y);
	float width = _collisionBounds.getExtends().x;
	float height = _collisionBounds.getExtends().y;

	ColliderBounds otherBounds = physicsBody->getCollisionBounds();
	Vector2 otherPosition = Vector2(otherBounds.getCentre().x, otherBounds.getCentre().y);
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
	{
		return false;
	}
}

bool PhysicsBody::coinCollision(PhysicsBody * physicsBody)
{
	Vector2 position = Vector2(_collisionBounds.getCentre().x, _collisionBounds.getCentre().y);
	float width = _collisionBounds.getExtends().x;
	float height = _collisionBounds.getExtends().y;

	ColliderBounds otherBounds = physicsBody->getCollisionBounds();
	Vector2 otherPosition = Vector2(otherBounds.getCentre().x, otherBounds.getCentre().y);
	float otherWidth = otherBounds.getExtends().x;
	float otherHeight = otherBounds.getExtends().y;

	if (position.x - width < otherPosition.x + otherWidth &&
		position.x + width > otherPosition.x - otherWidth &&
		position.y - height < otherPosition.y + otherHeight &&
		position.y + height > otherPosition.y - otherHeight)
	{
		physicsBody->setUseGravity(true);
		return true;
	}
	else
	{
		
		return false;
	}
}

void PhysicsBody::updatePhysicsBody(Transform* transform, float deltaTime)
{
	dt = deltaTime;
	Vector2 gravity(0, GRAVITY);

	if (useGravity && velocity.y > -6.0f)
		force = force + gravity;

	if (velocity.x > 0)
		addForce(Vector2(1.0f*GRAVITY, 0.0f));
	else if (velocity.x < 0)
		addForce(Vector2(1.0f*-GRAVITY, 0.0f));


	acceleration = force * 1.0f / mass;
	velocity = velocity + acceleration * dt;
	//_collisionBounds.setCentre(_collisionBounds.getCentre() + Vector3(velocity, 0.0f) * dt);
	transform->setWorldPosition(transform->getWorldPosition() + Vector3(velocity, 0.0f) * dt);
	_collisionBounds.update(transform);


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

Vector3 PhysicsBody::getPosition() const
{
	return _collisionBounds.getCentre();
}

float PhysicsBody::getMass() const
{
	return mass;
}

bool PhysicsBody::getUseGravity() const
{
	return useGravity;
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

void PhysicsBody::setPosition(Vector3 p)
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

void PhysicsBody::onCollisionEnter(PhysicsBody * other)
{
	// Check if the collision is just occuring for the first time.
	vector<PhysicsBody*>::iterator it;
	for (it = _colliding.begin(); it != _colliding.end(); it++)
	{
		// Physics body is already in the vector
		if (other == (*it))
			return;
	}

	// The first time these two physics bodies are colliding.
	_colliding.push_back(other);

	// Perform certain behaviour, depending on the type of objects colliding.
	onCollisionHelper(other->getTag(), true);
}

void PhysicsBody::onCollisionExit(PhysicsBody * other)
{
	// Check if the collision is just occuring for the first time.
	vector<PhysicsBody*>::iterator it;
	for (it = _colliding.begin(); it != _colliding.end(); it++)
	{
		// Remove physics body from vector
		if (other == (*it))
		{
			_colliding.erase(it);
			break;
		}
	}

	// Perform certain behaviour, depending on the type of objects colliding.
	onCollisionHelper(other->getTag(), false);
}

Tags PhysicsBody::getTag() const
{
	return _tag;
}

void PhysicsBody::onCollisionHelper(Tags tag, bool enter)
{
	switch (tag)
	{
	// Player colliding with platform
	case Tags::Platform:
	{
		if (enter)
		{
			useGravity = false;
			_canJump = true;
		}
		else
		{
			useGravity = true;
			_canJump = false;
		}
		break;
	}
	}
}
