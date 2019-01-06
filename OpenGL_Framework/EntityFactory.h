#pragma once

#include "EntityManager.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsBodyComponent.h"
#include "ObjectLoader.h"

class EntityFactory
{
public:
	EntityFactory(EntityManager* entityManager);
	~EntityFactory();

	Entity* createEmpty(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createCoin(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createPlatform(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createSpike(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createCone(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createPlayer(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createAcorn(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);
	Entity* createVent(const Vector3& position = Vector3::Zero, Entity* parent = nullptr);

private:
	EntityManager* _entityManager;
};