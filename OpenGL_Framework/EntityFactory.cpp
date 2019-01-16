#include "EntityFactory.h"

EntityFactory::EntityFactory(EntityManager * entityManager)
	: _entityManager(entityManager)
{
}

EntityFactory::~EntityFactory()
{
}

Entity * EntityFactory::createEmpty(const Vector3 & position, const Vector3& scale, Entity * parent)
{
	// Create a new entity.
	Entity* entity = _entityManager->createEntity();

	// Create transform component.
	TransformComponent* transform = new TransformComponent();
	transform->setLocalPosition(Vector3(position));
	transform->setLocalScale(scale);

	// If this entity is given a parent, then add this entity as a child of the parent.
	if (parent)
	{
		TransformComponent* parentTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, parent);
		parentTransform->addChild(transform);
	}

	// Add the transform component to the entity.
	_entityManager->addComponent(transform, entity);

	return entity;
}

Entity * EntityFactory::createPerspectiveCamera(const Vector3 & position, const float fovY, const float aspect, const float zNear, const float zFar)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position);

	// Create camera component.
	CameraComponent* camera = new CameraComponent();
	camera->perspective(fovY, aspect, zNear, zFar);
	camera->setCullingActive(true);

	// Add the camera component to the entity.
	_entityManager->addComponent(camera, entity);
	EntityManager::setMainCamera(entity);

	return entity;
}

Entity * EntityFactory::createCoin(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Water"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Coin"),
		ObjectLoader::getShaderProgram("Normal"), textures);
	meshRenderer->setIsTransparent(true);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Coin);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Coin Collision Entered!" << std::endl; 
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createPlatform(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Building"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Building"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Platform);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Platform Collision Entered!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);

		otherBody->setUseGravity(false);
		otherBody->setVelocity(Vector3::Zero);

		//if (otherBody->getVelocity().y <= -0.01f)
		//{
		//	float ySpeed = otherBody->getVelocity().y;
		//	otherBody->addForce(Vector3(0.0f, -ySpeed, 0.0f));

		//}
	};
	physicsBody->onCollisionStay = [](Entity* self, Entity* other)
	{
		//std::cout << "Platform Collision Stayed!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);

		if (otherBody->getTag() == TTag::Player)
		{
			//std::cout << "Platform Collision Stayed!" << std::endl;
			otherBody->addForce(Vector3(1.2f, 0.0f, 0.0f));
		}
	};
	physicsBody->onCollisionExit = [](Entity* self, Entity* other)
	{
		std::cout << "Platform Collision Exited!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);

		otherBody->setUseGravity(true);
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createSpike(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Spikes"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Spikes"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Spike Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();

		
		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(Vector3(-3.0f, 4, -5.0f));
			otherBody->setVelocity(Vector3::Zero);
			otherTrans->setLocalScale(0.2f);
		}
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createCone(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Cone"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Cone"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Cone Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(Vector3(-3.0f, 8.0f, -5.0f));
			otherTrans->setLocalPosition(Vector3::Zero);
			otherBody->setVelocity(Vector3::Zero);
			otherTrans->setLocalScale(0.2f);
		}
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createPlayer(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);


	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("FatBoi"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("TestBoi1"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Player);
	physicsBody->setUseGravity(true);

	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);

	return entity;
}

Entity * EntityFactory::createAcorn(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Acorn"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Acorn"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Coin);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Acorn Collision Entered!" << std::endl;
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createVent(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Vent"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Vent"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Vent Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(Vector3(-3.0f, 4, -5.0f));
			otherBody->setVelocity(Vector3::Zero);
			otherTrans->setLocalScale(0.2f);
		}
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createBackground(const Vector3 & position, const Vector3 & scale, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Background"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Background"),
		ObjectLoader::getShaderProgram("Normal"), textures);


	// Add the component to the entity.
	_entityManager->addComponent(meshRenderer, entity);


	return entity;
}

void EntityFactory::createPlatforms(const unsigned int amount, const Vector3 & position, const Vector3 & scale)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		Vector3 newPos = Vector3(i * position.x, position.y, position.z);

		entity = createPlatform(Vector3::Zero, scale, createEmpty(newPos));
	}
}

void EntityFactory::createCones(const unsigned int amount, const Vector3 & position, const Vector3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		Vector3 newPos = Vector3(i * position.x + offset, position.y, position.z);

		entity = createCone(Vector3::Zero, scale, createEmpty(newPos));
	}
}

void EntityFactory::createBackgrounds(const unsigned int amount, const Vector3 & position, const Vector3 & scale)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		Vector3 newPos = Vector3(i * position.x, position.y, position.z);

		entity = createBackground(Vector3::Zero, scale, createEmpty(newPos));
	}
}
