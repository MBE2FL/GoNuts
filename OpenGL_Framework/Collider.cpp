#include "Collider.h"
#include "EntityFactory.h"

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

void Collider::onCollisionEnter(Entity * self, Entity * other)
{
	// Assign collision functions.
	switch (_tag)
	{
		//### ACORN ###
	case TTag::Acorn:
	{
		std::cout << "Acorn Collision Entered!" << std::endl;
		EntityFactory* entityFactory = EntityFactory::getInstance();
		entityFactory->plusAcorn();

		EntityManager* entityManager = EntityManager::getInstance();
		Collider* otherCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, other);
		if (otherCollider->getTag() == TTag::Player)
			_physicsBody->setUseGravity(true);

		break;
	}
		//### COIN ###
	case TTag::Coin:
	{
		std::cout << "Coin Collision Entered!" << std::endl;
		EntityFactory* entityFactory = EntityFactory::getInstance();
		entityFactory->plusCoin();

		EntityManager* entityManager = EntityManager::getInstance();
		Collider* otherCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, other);
		if (otherCollider->getTag() == TTag::Player)
			_physicsBody->setUseGravity(true);

		break;
	}
		//### ENEMY ###
	case TTag::Enemy:
	{
		break;
	}
		//### PLATFORM ###
	case TTag::Platform:
	{
		std::cout << "Platform Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		//TransformComponent* otherTransform = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);
		//TransformComponent* thisTransform = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, self);
		Collider* thisCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, self);
		Collider* otherCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, other);
		//float thisHeight = (abs(thisCollider->_max.y) + abs(thisCollider->_min.y)) / 2.0f;
		//float otherHeight = (abs(otherCollider->_max.y) + abs(otherCollider->_min.y)) / 2.0f;
		if (otherCollider->_min.y < thisCollider->_max.y - 0.2f
			&& otherCollider->getTag() == TTag::Player)
		{
			float xSpeed = otherBody->getVelocity().x;
			otherBody->addImpluseForce(vec3(-xSpeed*1.2f, 0.0f, 0.0f));
		}

		otherBody->setUseGravity(false);
		//otherBody->setVelocity(vec3(0.0f));

		if (otherBody->getVelocity().y < 0.0f && otherCollider->getTag() == TTag::Player)
		{
			float ySpeed = otherBody->getVelocity().y;
			otherBody->addImpluseForce(vec3(0.0f, -ySpeed, 0.0f));

		}

		otherBody->setCanJump(true);

		break;
	}
		//### PLAYER ###
	case TTag::Player:
	{
		break;
	}
		//### SPIKE ###
	case TTag::Spike:
	{
		std::cout << "Spike Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player && !otherCol->beastMode)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(vec3(-3.0f, 8, -5.0f));
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
		break;
	}
		//### TOP PLATFORM ###
	case TTag::TopPlatform:
	{
		std::cout << "topPlatform Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();

		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(vec3(-3.0f, 8.0f, -5.0f));
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
		break;
	}
	default:
		break;
	}
}

void Collider::onCollisionStay(Entity * self, Entity * other)
{
	switch (_tag)
	{
	case TTag::Player:
	{
		break;
	}
	case TTag::Platform:
	{
		//std::cout << "Platform Collision Stayed!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManger->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player)
		{
			//std::cout << "Platform Collision Stayed!" << std::endl;
			otherBody->addForce(vec3(2.2f, 0.0f, 0.0f));
		}

		break;
	}
	case TTag::Enemy:
		break;
	case TTag::Coin:
		break;
	case TTag::Acorn:
		break;
	case TTag::Spike:
		break;
	case TTag::TopPlatform:
		break;
	default:
		break;
	}
}

void Collider::onCollisionExit(Entity * self, Entity * other)
{
	switch (_tag)
	{
	case TTag::Player:
		break;
	case TTag::Platform:
	{
		std::cout << "Platform Collision Exited!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManger->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player)
		{
			otherBody->setUseGravity(true);
			otherBody->setCanJump(false);
		}

		break;
	}
	case TTag::Enemy:
		break;
	case TTag::Coin:
		break;
	case TTag::Acorn:
		break;
	case TTag::Spike:
		break;
	case TTag::TopPlatform:
		break;
	default:
		break;
	}
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

vec3 Collider::getOffset() const
{
	return _contactOffset;
}

void Collider::setOffset(const vec3 & offset)
{
	_contactOffset = offset;
}

bool Collider::getEnabled() const
{
	return _enabled;
}

void Collider::setEnabled(const bool enabled)
{
	_enabled = enabled;
}

TTag Collider::getTag() const
{
	return _tag;
}

void Collider::setTag(const TTag tag)
{
	_tag = tag;
}
