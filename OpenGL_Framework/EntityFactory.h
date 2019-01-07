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

	Entity* createEmpty(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createCoin(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createPlatform(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createSpike(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createCone(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createPlayer(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createAcorn(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);
	Entity* createVent(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 1.0f), Entity* parent = nullptr);

	void createPlatforms(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f));
	void createCones(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f), const float offset = 70.0f);

private:
	EntityManager* _entityManager;
};