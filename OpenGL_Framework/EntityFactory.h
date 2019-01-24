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
		const float aspect = 1.8f, const float zNear = 1.0f, const float zFar = 1000.0f, const string& name = "Camera");
	Entity* createCoin(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Coin");
	Entity* createPlatform(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Platform");
	Entity* createTopPlatform(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Top Platform");
	Entity* createSpike(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Spike");
	Entity* createCone(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Cone");
	Entity* createPlayer(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Player");
	Entity* createAcorn(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Acorn");
	Entity* createVent(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Vent");
	Entity* createBackground(const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, Entity* parent = nullptr, const string& name = "Background");

	void createPlatforms(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f));
	void createTopPlatforms(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, const float offset = 70.0f);
	void createCones(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, const float offset = 70.0f);
	void createVents(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, const float offset = 70.0f);
	void createSpikes(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, const float offset = 70.0f);
	void createCoins(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, const float offset = 70.0f);
	void createAcorns(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3::One, const float offset = 70.0f);
	void createBackgrounds(const unsigned int amount, const Vector3& position = Vector3::Zero, const Vector3& scale = Vector3(1.0f, 1.0f, 0.5f));

	int getCoinCount() { return coinCount; }
	void plusCoin() { coinCount++; }

	int getAcornCount() { return acornCount; }
	void plusAcorn() { acornCount++; }

	void setEntityManager();

private:
	EntityManager* _entityManager;
	static EntityFactory* _instance;

	int coinCount = 0;
	int acornCount = 0;

	EntityFactory();
};