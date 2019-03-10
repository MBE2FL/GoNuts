#include "EntityManager.h"

EntityManager* EntityManager::_instance = nullptr;
Entity* EntityManager::_mainCamera = nullptr;
TransformComponent* EntityManager::_playerTransform = nullptr;
Entity* EntityManager::_shadowCamera = nullptr;

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	// Clean up all entities.
	for (Entity* entity : _entities)
	{
		delete entity;
		entity = nullptr;
	}

	//// Clean up all components.
	//map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt;

	//for (compTypeIt = _components.begin(); compTypeIt != _components.end(); compTypeIt++)
	//{
	//	map<unsigned int, Component*>::iterator compIt;

	//	for (compIt = compTypeIt->second.begin(); compIt != compTypeIt->second.end(); compIt++)
	//	{
	//		delete compIt->second;
	//		compIt->second = nullptr;
	//	}
	//}


	// Since all entities have been deleted, we can reset entity ids back to 0.
	Entity::resetEids();
}

EntityManager * EntityManager::getInstance()
{
	//if (!_instance)
	//{
	//	_instance = new EntityManager();
	//}

	//return _instance;

	return _instance;
}

void EntityManager::setInstance(EntityManager * entityManager)
{
	_instance = entityManager;
}

Entity * EntityManager::createEntity()
{
	// Create a new entity, and add it to the vector of entities.
	Entity* entity = new Entity();

	_entities.push_back(entity);


	// return the newly created entity for convenience.
	return entity;
}

void EntityManager::addComponent(Component * component, Entity * entity)
{
	component->setEntity(entity);
	//_components[component->getType()][entity->getEid()] = component;

	ComponentType type = component->getType();
	switch (type)
	{
	case ComponentType::Transform:
	{
		TransformComponent* transform = dynamic_cast<TransformComponent*>(component);
		_transformComps[entity] = transform;
		_allTransforms.push_back(transform);
		break;
	}
	case ComponentType::MeshRenderer:
	{
		MeshRendererComponent* meshRenderer = dynamic_cast<MeshRendererComponent*>(component);
		_meshRendComps[entity] = meshRenderer;
		_allMeshRenderers.push_back(meshRenderer);
		break;
	}
	case ComponentType::PhysicsBody:
	{
		PhysicsBodyComponent* physicsBody = dynamic_cast<PhysicsBodyComponent*>(component);

		// If this entity has a collider, provide it with the newly added physics body.
		Collider* collider = _colliderComps[entity];
		if (collider)
			collider->setPhysicsBody(physicsBody);

		_physicsBodyComps[entity] = physicsBody;
		_allPhysicsBodies.push_back(physicsBody);
		break;
	}
	case ComponentType::Collider:
	{
		Collider* collider = dynamic_cast<Collider*>(component);

		// If this entity has a physics body, provide it to the newly added collider.
		PhysicsBodyComponent* physicsBody = _physicsBodyComps[entity];
		if (physicsBody)
			collider->setPhysicsBody(physicsBody);

		_colliderComps[entity] = collider;
		_allColliders.push_back(collider);
		break;
	}
	case ComponentType::Camera:
	{
		CameraComponent* camera = dynamic_cast<CameraComponent*>(component);
		_cameraComps[entity] = camera;
		_allCameras.push_back(camera);
		break;
	}
	case ComponentType::Light:
		break;
	default:
		break;
	}

	//_entitiesWithComps[type].push_back(entity);
}

//Component * EntityManager::getComponent(ComponentType compType, const Entity * entity)
//{
//	// Return the component belonging to the entity, if it exists.
//	// Use .at instead of [], to prevent the creation of new elements in _components.
//	try
//	{
//		Component* component;
//		component = _components.at(compType).at(entity->getEid());
//		return component;
//	}
//	catch (const out_of_range)
//	{
//		return nullptr;
//	}
//}

void EntityManager::deleteEntity(Entity * entity)
{
	// Remove the entity.
	vector<Entity*>::iterator it;
	for (it = _entities.begin(); it != _entities.end();)
	{
		if (*it == entity)
		{
			//delete entity;
			//entity = nullptr;
			_entities.erase(it);
			break;
		}
		else
			++it;
	}

	unordered_map<Entity*, TransformComponent*>::iterator transIt = _transformComps.find(entity);
	unordered_map<Entity*, MeshRendererComponent*>::iterator meshIt = _meshRendComps.find(entity);
	unordered_map<Entity*, PhysicsBodyComponent*>::iterator physicsIt = _physicsBodyComps.find(entity);
	unordered_map<Entity*, Collider*>::iterator colIt = _colliderComps.find(entity);
	unordered_map<Entity*, CameraComponent*>::iterator camIt = _cameraComps.find(entity);

	// Remove Transform
	if (transIt != _transformComps.end())
	{
		TransformComponent* transform = transIt->second;
		_allTransforms.erase(remove(_allTransforms.begin(), _allTransforms.end(), transform), _allTransforms.end());
		_transformComps.erase(transIt);
		delete transform;
		transform = nullptr;

		cout << entity->getEid() << " Transform removed!" << endl;
	}
	else
		cerr << entity->getEid() << " Transform FAILED to removed!" << endl;
	// Remove Mesh Renderer
	if (meshIt != _meshRendComps.end())
	{
		MeshRendererComponent* meshRenderer = meshIt->second;
		_allMeshRenderers.erase(remove(_allMeshRenderers.begin(), _allMeshRenderers.end(), meshRenderer), _allMeshRenderers.end());
		_meshRendComps.erase(meshIt);
		delete meshRenderer;
		meshRenderer = nullptr;

		cout << entity->getEid() << " Mesh Renderer removed!" << endl;
	}
	else
		cerr << entity->getEid() << " Mesh Renderer FAILED to removed!" << endl;
	// Remove Physics body
	if (physicsIt != _physicsBodyComps.end())
	{
		PhysicsBodyComponent* physicsBody = physicsIt->second;
		_allPhysicsBodies.erase(remove(_allPhysicsBodies.begin(), _allPhysicsBodies.end(), physicsIt->second), _allPhysicsBodies.end());
		_physicsBodyComps.erase(physicsIt);
		delete physicsBody;
		physicsBody = nullptr;

		cout << entity->getEid() << " Physics body removed!" << endl;
	}
	else
		cerr << entity->getEid() << " Physics body FAILED to removed!" << endl;
	// Remove Collider
	if (colIt != _colliderComps.end())
	{
		Collider* collider = colIt->second;
		_allColliders.erase(remove(_allColliders.begin(), _allColliders.end(), collider), _allColliders.end());
		_colliderComps.erase(colIt);
		delete collider;
		collider = nullptr;

		cout << entity->getEid() << " Collider removed!" << endl;
	}
	else
		cerr << entity->getEid() << " Collider FAILED to removed!" << endl;
	// Remove Camera
	if (camIt != _cameraComps.end())
	{
		CameraComponent* camera = camIt->second;
		_allCameras.erase(remove(_allCameras.begin(), _allCameras.end(), camera), _allCameras.end());
		_cameraComps.erase(camIt);
		delete camera;
		camera = nullptr;

		cout << entity->getEid() << " Camera removed!" << endl;
	}
	else
		cerr << entity->getEid() << " Camera FAILED to removed!" << endl;


	// Finally delete the entity.
	delete entity;
	entity = nullptr;
}

void EntityManager::deleteEntities(const vector<Entity*>& entities)
{
}

//vector<Entity*> EntityManager::getAllEntitiesWithComponent(ComponentType compType)
//{
//	//vector<Entity*> entities;
//	//
//	//// Find all components of the specified type.
//	//map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt = _components.find(compType);
//
//	//// Find all entities, which possess the specified component type.
//	//if (compTypeIt != _components.end())
//	//{
//	//	map<unsigned int, Component*>::iterator compIt;
//
//	//	for (compIt = compTypeIt->second.begin(); compIt != compTypeIt->second.end(); compIt++)
//	//	{
//	//		entities.push_back(getEntity(compIt->first));
//	//	}
//	//}
//
//	//return entities;
//
//
//	//##### TESTING #####
//	return _entitiesWithComps[compType];
//}

vector<PhysicsBodyComponent*> EntityManager::getAllPhysicsBodyComponents() const
{
	return _allPhysicsBodies;
}

vector<TransformComponent*> EntityManager::getAllTransforms() const
{
	return _allTransforms;
}

vector<MeshRendererComponent*> EntityManager::getAllMeshRenderers() const
{
	return _allMeshRenderers;
}

vector<Collider*> EntityManager::getAllColliders() const
{
	return _allColliders;
}

vector<CameraComponent*> EntityManager::getAllCameras() const
{
	return _allCameras;
}

Entity * EntityManager::getEntity(const unsigned int eid)
{
	vector<Entity*>::iterator it;
	
	// Find the entity with the given entity id, if it exists.
	for (it = _entities.begin(); it != _entities.end(); it++)
	{
		if (eid == (*it)->getEid())
		{
			return *it;
		}
	}

	return nullptr;
}

vector<Entity*> EntityManager::getEntities() const
{
	return _entities;
}

void EntityManager::setMainCamera(Entity * camera)
{
	_mainCamera = camera;
}

Entity * EntityManager::getMainCamera()
{
	return _mainCamera;
}

void EntityManager::setShadowCamera(Entity * camera)
{
	_shadowCamera = camera;
}

Entity * EntityManager::getShadowCamera()
{
	return _shadowCamera;
}

TransformComponent * EntityManager::getPlayerTransform()
{
	return _playerTransform;
}

void EntityManager::setPlayerTransform(TransformComponent * transform)
{
	_playerTransform = transform;
}
