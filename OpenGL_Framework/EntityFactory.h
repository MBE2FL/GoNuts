#pragma once

#include "EntityManager.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsBodyComponent.h"
#include "CameraComponent.h"
#include "ObjectLoader.h"

class EntityFactory
{
public:
	//EntityFactory(EntityManager* entityManager);
	static EntityFactory* getInstance();
	~EntityFactory();

	Entity* createEmpty(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Empty");
	Entity* createPerspectiveCamera(const Vector3& position = Vector3::Zero, const float fovY = 60.0f, 
		const float aspect = 1.8f, const float zNear = 1.0f, const float zFar = 1000.0f);
	Entity* createCoin(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createPlatform(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createSpike(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createCone(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createPlayer(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createAcorn(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createVent(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);
	Entity* createBackground(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr);

	void createPlatforms(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f));
	void createCones(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f), const float offset = 70.0f);
	void createBackgrounds(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f));

private:
	EntityManager* _entityManager;
	static EntityFactory* _instance;

	EntityFactory();
};