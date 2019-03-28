#pragma once

#include "EntityManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsBodyComponent.h"
#include "ObjectLoader.h"
#include "BoxCollider.h"

class EntityFactory
{
public:
	//EntityFactory(EntityManager* entityManager);
	static EntityFactory* getInstance();
	~EntityFactory();

	Entity* createEmpty(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Empty");
	Entity* createPerspectiveCamera(const vec3& position = vec3(0.0f), const float fovY = 60.0f, 
		const float aspect = 1.8f, const float zNear = 1.0f, const float zFar = 1000.0f, const string& name = "Camera");
	Entity* createOrthographicCamera(const vec3& position, const float left,
		const float right, const float bottom, const float top, const float zNear = 1.0f, const float zFar = 1000.0f, const string& name = "Ortho Camera");
	Entity* createCoin(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Coin");
	Entity* createPlatform(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Platform");
	Entity* createTopPlatform(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Top Platform");
	Entity* createSpike(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Spike");
	Entity* createCone(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Cone");
	Entity* createPlayer(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Player");
	Entity* createAcorn(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Acorn");
	Entity* createVent(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Vent");
	Entity* createBackground(const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), Entity* parent = nullptr, const string& name = "Background");

	void createPlatforms(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f, 1.0f, 0.5f));
	void createTopPlatforms(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), const float offset = 70.0f);
	void createCones(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), const float offset = 70.0f);
	void createVents(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), const float offset = 70.0f);
	void createSpikes(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), const float offset = 70.0f);
	void createCoins(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), const float offset = 70.0f);
	void createAcorns(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f), const float offset = 70.0f);
	void createBackgrounds(const unsigned int amount, const vec3& position = vec3(0.0f), const vec3& scale = vec3(1.0f, 1.0f, 0.5f));

	void setEntityManager();

private:
	EntityManager* _entityManager;
	static EntityFactory* _instance;

	EntityFactory();
};