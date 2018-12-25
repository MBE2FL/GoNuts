#include "EntityFactory.h"

EntityFactory::EntityFactory(EntityManager * entityManager)
	: _entityManager(entityManager)
{
}

EntityFactory::~EntityFactory()
{
}

Entity * EntityFactory::createEmpty(const Vector3 & position, Entity * parent)
{
	// Create a new entity.
	Entity* entity = _entityManager->createEntity();

	// Create transform component.
	TransformComponent* transform = new TransformComponent();
	transform->setLocalPosition(Vector3(position));

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

Entity * EntityFactory::createCoin(const Vector3 & position, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Coin"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Coin"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Coin);
	physicsBody->onCollisionEnter = [](Collision* collision)
	{
		std::cout << "Coin Collision Entered!" << std::endl; 
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createPlatform(const Vector3 & position, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Building"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Building"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Platform);
	physicsBody->onCollisionEnter = [](Collision* collision)
	{
		std::cout << "Platform Collision Entered!" << std::endl;
	};
	physicsBody->onCollisionStay = [](Collision* collision)
	{
		std::cout << "Platform Collision Stayed!" << std::endl;
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createSpike(const Vector3 & position, Entity * parent)
{
	// Create a new empty entity.
	Entity* entity = createEmpty(position, parent);

	// Create all the necessary components.
	// Mesh Renderer
	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Spikes"));

	MeshRendererComponent* meshRenderer = new MeshRendererComponent(ObjectLoader::getMesh("Spikes"),
		ObjectLoader::getShaderProgram("Normal"), textures);

	// Physics Body
	PhysicsBodyComponent* physicsBody = new PhysicsBodyComponent(meshRenderer->getMesh()->getMeshBounds());
	physicsBody->setTag(TTag::Spike);
	physicsBody->onCollisionEnter = [](Collision* collision)
	{
		std::cout << "Spike Collision Entered!" << std::endl;
	};


	// Add all the components to the entity.
	_entityManager->addComponent(meshRenderer, entity);
	_entityManager->addComponent(physicsBody, entity);


	return entity;
}

Entity * EntityFactory::createCone(const Vector3 & position, Entity * parent)
{
	return nullptr;
}

Entity * EntityFactory::createPlayer(const Vector3 & position, Entity * parent)
{
	return nullptr;
}

Entity * EntityFactory::createAcorn(const Vector3 & position, Entity * parent)
{
	return nullptr;
}

Entity * EntityFactory::createVent(const Vector3 & position, Entity * parent)
{
	return nullptr;
}
