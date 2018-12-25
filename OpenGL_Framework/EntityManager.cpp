#include "EntityManager.h"

EntityManager* EntityManager::_instance = nullptr;

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

	// Clean up all components.
	map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt;

	for (compTypeIt = _components.begin(); compTypeIt != _components.end(); compTypeIt++)
	{
		map<unsigned int, Component*>::iterator compIt;

		for (compIt = compTypeIt->second.begin(); compIt != compTypeIt->second.end(); compIt++)
		{
			delete compIt->second;
			compIt->second = nullptr;
		}
	}


	// Since all entities have been deleted, we can reset entity ids back to 0.
	Entity::resetEids();
}

EntityManager * EntityManager::getInstance()
{
	if (!_instance)
	{
		_instance = new EntityManager();
	}

	return _instance;
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
	_components[component->getType()][entity->getEid()] = component;
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

void EntityManager::removeEntity(Entity * entity)
{
	// Remove the entity.
	for (unsigned int i = 0; i < _entities.size(); i++)
	{
		if (_entities[i] == entity)
		{
			delete _entities[i];
			_entities[i] = nullptr;
			_entities.erase(_entities.begin() + i);
			break;
		}
	}


	// Remove all of the entity's components.
	map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt;

	for (compTypeIt = _components.begin(); compTypeIt != _components.end(); compTypeIt++)
	{
		map<unsigned int, Component*>::iterator compIt = compTypeIt->second.find(entity->getEid());

		// Delete and clean up all of this entity's components. Then remove this entity's component map.
		if (compIt != compTypeIt->second.end())
		{
			delete compIt->second;
			compIt->second = nullptr;
			//compTypeIt->second.erase(compIt);
			_components.erase(compTypeIt);
		}
	}
}

vector<Entity*> EntityManager::getAllEntitiesWithComponent(ComponentType compType)
{
	vector<Entity*> entities;
	
	// Find all components of the specified type.
	map<ComponentType, map<unsigned int, Component*>>::iterator compTypeIt = _components.find(compType);

	// Find all entities, which possess the specified component type.
	if (compTypeIt != _components.end())
	{
		map<unsigned int, Component*>::iterator compIt;

		for (compIt = compTypeIt->second.begin(); compIt != compTypeIt->second.end(); compIt++)
		{
			entities.push_back(getEntity(compIt->first));
		}
	}


	return entities;
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
