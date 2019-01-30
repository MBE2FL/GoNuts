#include "EntityFactory.h"

//EntityFactory::EntityFactory(EntityManager * entityManager)
//	: _entityManager(entityManager)
//{
//}

EntityFactory* EntityFactory::_instance = nullptr;

EntityFactory * EntityFactory::getInstance()
{
	if (!_instance)
		_instance = new EntityFactory();

	return _instance;
}

EntityFactory::~EntityFactory()
{
}

Entity * EntityFactory::createEmpty(const vec3 & position, const vec3& scale, Entity * parent, const string& name)
{
	// Create a new entity.
	Entity* entity = _entityManager->createEntity();

	// Create transform component.
	TransformComponent* transform = new TransformComponent();
	transform->setLocalPosition(vec3(position));
	transform->setLocalScale(scale);
	transform->setName(name + " " + std::to_string(entity->getEid()));

	// If this entity is given a parent, then add this entity as a child of the parent.
	if (parent)
	{
		TransformComponent* parentTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, parent);
		parentTransform->addChild(transform);
		//parentTransform->setName("P " + name + " " + std::to_string(entity->getEid()));
	}

	// Add the transform component to the entity.
	_entityManager->addComponent(transform, entity);

	return entity;
}

Entity * EntityFactory::createPerspectiveCamera(const vec3 & position, const float fovY, const float aspect, 
	const float zNear, const float zFar, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, vec3(1.0f), nullptr, name);

	// Create camera component.
	CameraComponent* camera = new CameraComponent();
	camera->setPerspective(fovY, aspect, zNear, zFar);
	camera->setCullingActive(true);

	// Add the camera component to the entity.
	_entityManager->addComponent(camera, entity);
	EntityManager::setMainCamera(entity);

	return entity;
}

Entity * EntityFactory::createCoin(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Coin"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Coin"),
		ObjectLoader::getShaderProgram("Normal"), textures);
	meshRenderer->setIsTransparent(false);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Coin);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Coin Collision Entered!" << std::endl; 
		EntityManager* entityManger = EntityManager::getInstance();
		EntityFactory* entityFactory = EntityFactory::getInstance();
		entityFactory->plusCoin();

		PhysicsBodyComponent* selfBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, self);

		selfBody->setUseGravity(true);
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createPlatform(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Building"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Building"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Platform);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Platform Collision Entered!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);

		otherBody->setUseGravity(false);
		//otherBody->setVelocity(vec3(0.0f));

		if (otherBody->getVelocity().y < 0.0f)
		{
			float ySpeed = otherBody->getVelocity().y;
			otherBody->addImpluseForce(vec3(0.0f, -ySpeed, 0.0f));

		}

		otherBody->setCanJump(true);
	};
	physicsBody->onCollisionStay = [](Entity* self, Entity* other)
	{
		//std::cout << "Platform Collision Stayed!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);

		if (otherBody->getTag() == TTag::Player)
		{
			//std::cout << "Platform Collision Stayed!" << std::endl;
			//otherBody->addForce(vec3(1.2f, 0.0f, 0.0f));
		}
	};
	physicsBody->onCollisionExit = [](Entity* self, Entity* other)
	{
		std::cout << "Platform Collision Exited!" << std::endl;

		EntityManager* entityManger = EntityManager::getInstance();
		PhysicsBodyComponent* otherBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);

		if (otherBody->getTag() == TTag::Player)
		{
			otherBody->setUseGravity(true);
			otherBody->setCanJump(false);
		}
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());

	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createTopPlatform(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Default"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Cube"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::TopPlatform);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "topPlatform Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();

		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(vec3(-3.0f, 8.0f, -5.0f));
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());

	// Add the transform component to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createSpike(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Spikes"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Spikes"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Spike Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();

		
		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(vec3(-3.0f, 4, -5.0f));
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createCone(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Cone"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Cone"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Cone Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(vec3(-3.0f, 8.0f, -5.0f));
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createPlayer(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);


	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("FatBoi"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("TestBoi1"),
		ObjectLoader::getShaderProgram("Normal"), textures);
	Animation* anim = new Animation;
	meshRenderer->setAnimation(anim);
	meshRenderer->setIsAnimated(true);
	meshRenderer->setMesh(ObjectLoader::getMesh("TestBoi1"));
	meshRenderer->addMesh("TestBoi", 20);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Player);
	physicsBody->setUseGravity(true);

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());

	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createAcorn(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Acorn"));
	textures.push_back(ObjectLoader::getTexture("Toon"));
	textures.push_back(ObjectLoader::getTexture("Normal"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Acorn"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Coin);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Coin Collision Entered!" << std::endl;
		EntityManager* entityManger = EntityManager::getInstance();
		EntityFactory* entityFactory = EntityFactory::getInstance();
		entityFactory->plusAcorn();

		PhysicsBodyComponent* selfBody = entityManger->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, self);

		selfBody->setUseGravity(true);
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());

	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);

	return entity;
}

Entity * EntityFactory::createVent(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Vent"));
	textures.push_back(ObjectLoader::getTexture("Toon"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Vent"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent();
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Entity* self, Entity* other)
	{
		std::cout << "Vent Collision Entered!" << std::endl;

		EntityManager* entityManager = EntityManager::getInstance();


		PhysicsBodyComponent* otherBody = entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, other);
		if (otherBody->getTag() == TTag::Player)
		{
			TransformComponent* otherTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, other);

			otherTrans->setWorldPosition(vec3(-3.0f, 4, -5.0f));
			otherBody->setVelocity(vec3(0.0f));
			otherTrans->setLocalScale(vec3(0.2f));
		}
	};

	// Collider
	BoxCollider* collider = new BoxCollider(meshRenderer->getMesh()->getMeshBounds());

	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);
	_entityManager->addComponent(collider, entity);


	return entity;
}

Entity * EntityFactory::createBackground(const vec3 & position, const vec3 & scale, Entity * parent, const string& name)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, scale, parent, name);

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

void EntityFactory::createPlatforms(const unsigned int amount, const vec3 & position, const vec3 & scale)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x, position.y, position.z);

		entity = createPlatform(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createTopPlatforms(const unsigned int amount, const vec3 & position, const vec3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x + offset, position.y, position.z);

		entity = createTopPlatform(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createCones(const unsigned int amount, const vec3 & position, const vec3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x + offset, position.y, position.z);

		entity = createCone(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createVents(const unsigned int amount, const vec3 & position, const vec3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x + offset, position.y, position.z);

		entity = createVent(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createSpikes(const unsigned int amount, const vec3 & position, const vec3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x + offset, position.y, position.z);

		entity = createSpike(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createCoins(const unsigned int amount, const vec3 & position, const vec3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x + offset, position.y, position.z);

		entity = createCoin(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createAcorns(const unsigned int amount, const vec3 & position, const vec3 & scale, const float offset)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x + offset, position.y, position.z);

		entity = createAcorn(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::createBackgrounds(const unsigned int amount, const vec3 & position, const vec3 & scale)
{
	Entity* entity;

	for (unsigned int i = 0; i < amount; i++)
	{
		vec3 newPos = vec3(i * position.x, position.y, position.z);

		entity = createBackground(newPos, scale);//, createEmpty(newPos));
	}
}

void EntityFactory::setEntityManager()
{
	_entityManager = EntityManager::getInstance();
}

EntityFactory::EntityFactory()
{
	_entityManager = EntityManager::getInstance();
}
