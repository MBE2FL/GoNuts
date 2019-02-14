#pragma once

#include "Entity.h"
#include "Component.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsBodyComponent.h"
#include "Collider.h"

#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using std::map;
using std::out_of_range;

using std::unordered_set;
using std::unordered_map;

using std::remove;
using std::cerr;


//enum class ComponentType
//{
//	Transform,
//	Renderer
//};

class EntityManager
{
public:
	EntityManager();
	~EntityManager();
	static EntityManager* getInstance();
	static void setInstance(EntityManager* entityManager);

	Entity* createEntity();
	void addComponent(Component* component, Entity* entity);
	//Component* getComponent(ComponentType compType, const Entity* entity);
	template<typename T>
	T getComponent(ComponentType compType, Entity* entity);
	void deleteEntity(Entity* entity);
	void deleteEntities(const vector<Entity*>& entities);

	// #### Get all component functions ####
	//vector<Entity*> getAllEntitiesWithComponent(ComponentType compType);
	//template<typename T>
	//vector<T> getAllComponentsOfType(ComponentType type);
	vector<PhysicsBodyComponent*> getAllPhysicsBodyComponents() const;
	vector<TransformComponent*> getAllTransforms() const;
	vector<MeshRendererComponent*> getAllMeshRenderers() const;
	vector<Collider*> getAllColliders() const;
	vector<CameraComponent*> getAllCameras() const;
	// #### Get all component functions ####

	Entity* getEntity(const unsigned int eid);
	vector<Entity*> getEntities() const;


	// #### Useful specific funtions ####
	static void setMainCamera(Entity* camera);
	static Entity* getMainCamera();
	static TransformComponent* getPlayerTransform();
	static void setPlayerTransform(TransformComponent* transform);
	// #### Useful specific funtions ####

private:
	vector<Entity*> _entities;
	//map<ComponentType, map<unsigned int, Component*>> _components;

	//unordered_set<Entity*> _entities;
	//unordered_map<ComponentType, vector<Entity*>> _entitiesWithComps;

	unordered_map<Entity*, TransformComponent*> _transformComps;
	unordered_map<Entity*, MeshRendererComponent*> _meshRendComps;
	unordered_map<Entity*, PhysicsBodyComponent*> _physicsBodyComps;
	unordered_map<Entity*, Collider*> _colliderComps;
	unordered_map<Entity*, CameraComponent*> _cameraComps;

	vector<TransformComponent*> _allTransforms;
	vector<MeshRendererComponent*> _allMeshRenderers;
	vector<PhysicsBodyComponent*> _allPhysicsBodies;
	vector<CameraComponent*> _allCameras;
	vector<Collider*> _allColliders;

	static EntityManager* _instance;
	static Entity* _mainCamera;
	static TransformComponent* _playerTransform;
};

//template<typename T>
//inline void EntityManager::addComponent(T * component, Entity * entity)
//{
//	ComponentType type = component->getType();
//
//	switch (type)
//	{
//	case ComponentType::Transform:
//		_transformComps[entity] = component;
//		break;
//	case ComponentType::MeshRenderer:
//		_meshRendComps[entity] = component;
//		break;
//	case ComponentType::PhysicsBody:
//		_physicsBodyComps[entity] = component;
//		break;
//	case ComponentType::Camera:
//		_cameraComps[entity] = component;
//		break;
//	case ComponentType::Light:
//		break;
//	default:
//		break;
//	}
//
//	_entitiesWithComps[type].push_back(entity);
//}

template<typename T>
inline T EntityManager::getComponent(ComponentType compType, Entity * entity)
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


	//Component* component;
	//map<ComponentType, map<unsigned int, Component*>>::iterator typeIT = _components.find(compType);
	//if (typeIT != _components.end())
	//{
	//	map<unsigned int, Component*>::iterator compIT = typeIT->second.find(entity->getEid());
	//	if (compIT != typeIT->second.end())
	//	{
	//		component = compIT->second;
	//		return dynamic_cast<T>(component);
	//	}
	//}

	//return nullptr;

	//Component* component;
	//unordered_map<ComponentType, unordered_map<Entity*, Component*>>::iterator typeIT = _components.find(compType);
	//if (typeIT != _components.end())
	//{
	//	unordered_map<Entity*, Component*>::iterator compIT = typeIT->second.find(entity);
	//	if (compIT != typeIT->second.end())
	//	{
	//		component = compIT->second;
	//		return dynamic_cast<T>(component);
	//	}
	//}

	//return nullptr;




	switch (compType)
	{
	case ComponentType::Transform:
		return dynamic_cast<T>(_transformComps[entity]);
		break;
	case ComponentType::MeshRenderer:
		return dynamic_cast<T>(_meshRendComps[entity]);
		break;
	case ComponentType::PhysicsBody:
		return dynamic_cast<T>(_physicsBodyComps[entity]);
		break;
	case ComponentType::Camera:
		return dynamic_cast<T>(_cameraComps[entity]);
		break;
	case ComponentType::Light:
		return nullptr;
		break;
	case ComponentType::Collider:
		return dynamic_cast<T>(_colliderComps[entity]);;
		break;
	default:
		return nullptr;
		break;
	}

}

//template<typename T>
//inline vector<T> EntityManager::getAllComponentsOfType(ComponentType type)
//{
//	switch (type)
//	{
//	case ComponentType::Transform:
//		return reinterpret_cast<vector<T>>(_allTransforms);
//		break;
//	case ComponentType::MeshRenderer:
//		return reinterpret_cast<vector<T>>(_allMeshRenderers);
//		break;
//	case ComponentType::PhysicsBody:
//		return reinterpret_cast<vector<T>>(_allPhysicsBodies);
//		break;
//	case ComponentType::Camera:
//		return reinterpret_cast<vector<T>>(_allCameras);
//		break;
//	case ComponentType::Light:
//		return vector<T>();
//		break;
//	default:
//		return vector<T>();
//		break;
//	}
//	return vector<T>();
//}
