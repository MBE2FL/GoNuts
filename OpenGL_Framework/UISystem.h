#pragma once

#include "UICanvas.h"
#include <string>
#include "CameraComponent.h"
#include "EntityManager.h"
#include "Font.h"

using std::string;

class UISystem
{
public:
	UISystem(EntityManager* entityManager);
	~UISystem();

	void update(float deltaTime);
	void draw();
	void addCanvas(UICanvas* canvas);
	void deleteCanvas(const string& name);
	Entity* getCamera() const;
	void checkClick(int x, int y);
	unordered_map<string, UICanvas*> getCanvases() const;
	UICanvas* getCanvas(const string& name) const;

private:
	unordered_map<string, UICanvas*> _canvases;
	Entity* _camera;
	TransformComponent* _camTrans;
	CameraComponent* _camComp;
	FontManager _fontManager;

	void init(EntityManager* entityManager);
};