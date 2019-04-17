#include "Collider.h"
#include "EntityFactory.h"
#include "SceneManager.h"

Collider::Collider()
{
	_type = ComponentType::Collider;
	_bounds = nullptr;
	_contactOffset = vec3(0.0f);
	_enabled = true;
	_sound = SoundComponent::getInstance();
	sceneManager = SceneManager::getInstance();
	_score = new ScoreCounter;
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
		//std::cout << "Acorn Collision Entered!" << std::endl;
		

		EntityManager* entityManager = EntityManager::getInstance();
		Collider* otherCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, other);
		if (otherCollider->getTag() == TTag::Player)
		{
			_sound->playSound("acorn", false);
			_physicsBody->setUseGravity(true);
			_score->plusAcorn();
		}

		break;
	}
		//### COIN ###
	case TTag::Coin:
	{
		//std::cout << "Coin Collision Entered!" << std::endl;
		

		EntityManager* entityManager = EntityManager::getInstance();
		Collider* otherCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, other);
		if (otherCollider->getTag() == TTag::Player)
		{
			_sound->playSound("coin", false);
			_physicsBody->setUseGravity(true);
			_score->plusCoin();
			_score->totalGameTime -= 1.0f;
		}

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
		//std::cout << "Platform Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* thisCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, self);
		Collider* otherCollider = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCollider->_min.y < thisCollider->_max.y - 0.2f
			&& otherCollider->getTag() == TTag::Player)
		{
			float xSpeed = otherBody->getVelocity().x;
			otherBody->addImpluseForce(vec3(-xSpeed*1.2f, 0.0f, 0.0f));
			_sound->playSound("landingGrunt", false, 0.5f);
		}
		if (otherCollider->_min.y < thisCollider->_max.y - 0.2f
			&& otherCollider->_min.z < thisCollider->_max.y
			&& otherCollider->_max.x - (otherCollider->_max.x - otherCollider->_min.x) * 0.5 > thisCollider->_min.x
			&& otherCollider->getTag() == TTag::Player)
		{
			otherCollider->front = true;
			_sound->playSound("landingGrunt", false, 0.5f);
		}

		if (otherCollider->getTag() == TTag::Player && otherCollider->beastMode)
		{
			otherCollider->screenShake = true;
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
		//std::cout << "Spike Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player && !otherCol->beastMode)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(otherTrans->getPlayerSpawnPosition());
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
		else if (otherCol->getTag() == TTag::Player && otherCol->beastMode)
		{
			TransformComponent* trans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, self);
			trans->setLocalScale(0);
			otherCol->screenShake = true;
		}
		break;
	}
		//### CHECKPOINT ###
	case TTag::Checkpoint:
	{
		//std::cout << "Checkpoint Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		Collider* otherCol = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setPlayerSpawnPosition(vec3(otherTrans->getWorldPosition().x, 
				otherTrans->getWorldPosition().y + 2, otherTrans->getWorldPosition().z));
		}
		break;
	}
		//### VictoryPoint
	case TTag::VictoryPoint:
	{
		EntityManager* entityManager = EntityManager::getInstance();
		//add things later on when more ui stuff is done to change to the scoreboard scene or which ever scene is next
		Collider* otherCol = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		_currentScene = SceneManager::getInstance()->getCurrentScene();
 		if (otherCol->getTag() == TTag::Player)
		{
			otherCol->victor = true;
		}
		break;
	}
		//### Generic ###
	case TTag::Generic:
	{
		//std::cout << "Generic Collision Entered!" << std::endl;
		break;
	}
		//### TOP PLATFORM ###
	case TTag::TopPlatform:
	{
		//std::cout << "topPlatform Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();

		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManager->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(otherTrans->getPlayerSpawnPosition());
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
		break;
	}
	default:
		break;
	}
}

void Collider::onCollisionStay(Entity * self, Entity * other, float deltaTime)
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

		if (otherBody != nullptr && otherCol != nullptr)
		{
			if (otherCol->getTag() == TTag::Player)
			{
				//std::cout << "Platform Collision Stayed!" << std::endl;
				otherBody->addForce(vec3(3.2f, 0.0f, 0.0f));

				if (otherCol->beastMode && otherCol->screenShake)
				{
					otherCol->shakeTimer += deltaTime;
				}

				if (otherCol->beastMode && otherCol->shakeTimer > 0.15f)
				{
					otherCol->screenShake = false;
					otherCol->shakeTimer = 0.0f;

				}

				if (otherCol->_max.y < _max.y)
				{
					otherBody->setUseGravity(true);
					otherBody->setCanJump(false);
				}

			}
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
		//std::cout << "Platform Collision Exited!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		Collider* otherCol = entityManger->getComponent<Collider*>(ComponentType::Collider, other);

		if (otherCol->getTag() == TTag::Player)
		{
			otherBody->setUseGravity(true);
			otherBody->setCanJump(false);
			otherCol->screenShake = false;
			otherCol->shakeTimer = 0.0f;
		}

		break;
	}
	case TTag::Enemy:
		break;
	case TTag::Coin:
		break;
	case TTag::Acorn:
		std::cout << "Acorn Collision Exited!" << std::endl;

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
