#pragma once

#include "Entity.h"
#include "Component.h"

#include <map>

using std::map;
using std::out_of_range;


//enum class ComponentType
//{
//	Transform,
//	Renderer
//};

class EntityManager
{
public:
	~EntityManager();
	static EntityManager* getInstance();

	Entity* createEntity();
	void addComponent(Component* component, Entity* entity);
	//Component* getComponent(ComponentType compType, const Entity* entity);
	template<typename T>
	T getComponent(ComponentType compType, const Entity* entity);
	void removeEntity(Entity* entity);
	vector<Entity*> getAllEntitiesWithComponent(ComponentType compType);
	Entity* getEntity(const unsigned int eid);

private:
	EntityManager();
	vector<Entity*> _entities;
	map<ComponentType, map<unsigned int, Component*>> _components;

	static EntityManager* _instance;
};

template<typename T>
inline T EntityManager::getComponent(ComponentType compType, const Entity * entity)
{
	// Return the component belonging to the entity, if it exists.
	// Use .at instead of [], to prevent the creation of new elements in _components.
	//try
	//{
	//	Component* component;
	//	component = _components.at(compType).at(entity->getEid());
	//	return dynamic_cast<T>(component);
	//}
	//catch (const out_of_range)
	//{
	//	return nullptr;
	//}


	Component* component;
	map<ComponentType, map<unsigned int, Component*>>::iterator typeIT = _components.find(compType);
	if (typeIT != _components.end())
	{
		map<unsigned int, Component*>::iterator compIT = typeIT->second.find(entity->getEid());
		if (compIT != typeIT->second.end())
		{
			component = compIT->second;
			return dynamic_cast<T>(component);
		}
	}

	return nullptr;



	//Component* component;
	//if (_components.count(compType) > 0)
	//{
	//	if (_components[compType].count(entity->getEid()) > 0)
	//	{
	//		component = _components[compType][entity->getEid()];
	//		return dynamic_cast<T>(component);
	//	}
	//}

	//return nullptr;
}
