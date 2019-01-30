#include "EntityManager.h"

EntityManager* EntityManager::_instance = nullptr;
Entity* EntityManager::_mainCamera = nullptr;
TransformComponent* EntityManager::_playerTransform = nullptr;

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

	_entitiesWithComps[type].push_back(entity);
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
	//// Remove the entity.
	//for (unsigned int i = 0; i < _entities.size(); i++)
	//{
	//	if (_entities[i] == entity)
	//	{
	//		delete _entities[i];
	//		_entities[i] = nullptr;
	//		_entities.erase(_entities.begin() + i);
	//		break;
	//	}
	//}


	//// Remove all of the entity's components.
	//map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt;

	//for (compTypeIt = _components.begin(); compTypeIt != _components.end(); compTypeIt++)
	//{
	//	map<unsigned int, Component*>::iterator compIt = compTypeIt->second.find(entity->getEid());

	//	// Delete and clean up all of this entity's components. Then remove this entity's component map.
	//	if (compIt != compTypeIt->second.end())
	//	{
	//		delete compIt->second;
	//		compIt->second = nullptr;
	//		//compTypeIt->second.erase(compIt);
	//		_components.erase(compTypeIt);
	//	}
	//}
}

void EntityManager::deleteEntities(const vector<Entity*>& entities)
{
}

vector<Entity*> EntityManager::getAllEntitiesWithComponent(ComponentType compType)
{
	//vector<Entity*> entities;
	//
	//// Find all components of the specified type.
	//map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt = _components.find(compType);

	//// Find all entities, which possess the specified component type.
	//if (compTypeIt != _components.end())
	//{
	//	map<unsigned int, Component*>::iterator compIt;

	//	for (compIt = compTypeIt->second.begin(); compIt != compTypeIt->second.end(); compIt++)
	//	{
	//		entities.push_back(getEntity(compIt->first));
	//	}
	//}

	//return entities;


	//##### TESTING #####
	return _entitiesWithComps[compType];
}

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

TransformComponent * EntityManager::getPlayerTransform()
{
	return _playerTransform;
}

void EntityManager::setPlayerTransform(TransformComponent * transform)
{
	_playerTransform = transform;
}
