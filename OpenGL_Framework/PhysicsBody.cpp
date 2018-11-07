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

	unsigned int left, right, up, down;
	left = 0; right = 0; up = 0; down = 0;

	//if (up == 1||down ==1)
	//	return 1;
	//else if (left == 1)
	//	return 2;
	//else if (right == 1)
	//	return 3;
	//else
	//	return 0;


	if (position.x - width < otherPosition.x + otherWidth &&
		position.x + width > otherPosition.x - otherWidth &&
		position.y - height < otherPosition.y + otherHeight &&
		position.y + height > otherPosition.y - otherHeight)
	{
		//Vector2 toLeftCorner(otherPosition.x - otherWidth, otherPosition.y + otherHeight);
		//toLeftCorner = toLeftCorner - otherPosition;
		//Vector2 toRightCorner(otherPosition.x + otherWidth, otherPosition.y + otherHeight);
		//toRightCorner = toRightCorner - otherPosition;
		////std::cout << toCorner << std::endl;
		//float angleToLeftCorner;
		//angleToLeftCorner = atanh(toLeftCorner.y / toLeftCorner.x) * 180.0f / PI;
		//float angleToRightCorner;
		//angleToRightCorner = atanh(toRightCorner.y / toRightCorner.x) * 180.0f / PI;
		////std::cout << angleToLeftCorner << std::endl;
		//Vector2 toPlayer(position - otherPosition);
		//float angleToPlayer;
		//angleToPlayer = atanh(toPlayer.y / toPlayer.x) * 180.0f / PI;
		//std::cout << std::endl;
		////std::cout << angleToLeftCorner << std::endl;
		//std::cout << angleToPlayer << std::endl;
		//std::cout << std::endl;
		//if (angleToPlayer <= 0.0f)
		//{
		//	if (angleToLeftCorner > angleToPlayer)
		//		return 1;
		//	else
		//		return 2;
		//}
		//else
		//{
		//	if (angleToRightCorner < angleToPlayer)
		//		return 1;
		//	else
		//		return 2;
		//}
		setUseGravity(false);

		

		if (position.y - (height * 0.8f) >= otherPosition.y + otherHeight &&
			position.y - height <= otherPosition.y + otherHeight 
			)
		{
			//if (position.y - height < otherPosition.y + otherHeight)
			//	position.y = position.y + 0.5f;//(otherPosition.y + otherHeight - position.y - height);
			if (getVelocity().y <= 0.01f)
			{
				float ySpeed = getVelocity().y;
				addForce(Vector2(0.0f, -ySpeed / (dt * 0.8f)));
				std::cout << -ySpeed / (dt * 0.8f) << std::endl;
			}
			//if (ySpeed < 0.05f && ySpeed > -0.05)
			//	addForce(Vector2(0.0f, -GRAVITY * 1.002f));
		}
		else if (position.x - width < otherPosition.x + otherWidth &&
			position.x + width > otherPosition.x + otherWidth &&
			getVelocity().x < -0.1f)
		{
			float xSpeed = getVelocity().x;
			addForce(Vector2(-xSpeed / (dt * 0.6f), 0.0f));
			//std::cout << -xSpeed / dt * 1.2f << std::endl;
		}
		else if (position.x + width > otherPosition.x - otherWidth &&
			position.x - width < otherPosition.x + otherWidth &&
			getVelocity().x > 0.1f)
		{
			float xSpeed = getVelocity().x;
			addForce(Vector2(-xSpeed / (dt * 0.6f), 0.0f));
		}


		//else if (position.x - width <= otherPosition.x + otherWidth &&
		//	position.x + width > otherPosition.x + otherWidth)
		//	return 2;
		//else if (position.x + width >= otherPosition.x - otherWidth &&
		//	position.x - width < otherPosition.x + otherWidth)
		//	return 2;
		

		
		return true;
	}
	else
	{
		setUseGravity(true);
		
		return false;
	}
}

void PhysicsBody::updatePhysicsBody(Transform* transform, float deltaTime)
{
	dt = deltaTime;
	Vector2 gravity(0, GRAVITY);

	if (useGravity && velocity.y > -4.0f)
		force = force + gravity;

	if (velocity.x > 0)
		addForce(Vector2(2.2f*GRAVITY, 0.0f));
	else if (velocity.x < 0)
		addForce(Vector2(2.2f*-GRAVITY, 0.0f));


	acceleration = force * 1.0f / mass;
	velocity = velocity + acceleration * dt;
	//_collisionBounds.setCentre(_collisionBounds.getCentre() + Vector3(velocity, 0.0f) * dt);
	transform->setPosition(transform->getPosition() + Vector3(velocity, 0.0f) * dt);
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
