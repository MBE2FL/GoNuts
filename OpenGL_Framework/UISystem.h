#pragma once

#include "UICanvas.h"
#include <string>
#include "CameraComponent.h"
#include "EntityManager.h"

using std::string;

class UISystem
{
public:
	UISystem(EntityManager* entityManager);
	~UISystem();

	void update(float deltaTime);
	void addCanvas(const string& name, UICanvas* canvas);
	void deleteCanvas(const string& name);
	Entity* getCamera() const;

private:
	unordered_map<string, UICanvas*> canvases;
	Entity* _camera;
	TransformComponent* _camTrans;
	CameraComponent* _camComp;

	void init(EntityManager* entityManager);
};