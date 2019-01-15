#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl3.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsBodyComponent.h"
#include "EntityManager.h"
#include "ObjectLoader.h"
#include <vector>
#include <algorithm>

using std::vector;
using std::partition;

class GUIHelper
{
public:
	static GUIHelper* getInstance();
	~GUIHelper();

	void init(float width, float height);
	void draw();

private:
	static GUIHelper* _instance;
	bool _showSceneEditor = false;
	bool _showPropertyEditor = false;
	bool _showShaderProgCreator = false;
	TransformComponent* _currentTransform = nullptr;
	EntityManager* _entityManager = nullptr;

	GUIHelper();
	void drawHierarchy();
	void drawHierarchyHelper(TransformComponent* transform);
	void propertyEditor(TransformComponent* transform, bool* open);
	void drawMeshRenderer(MeshRendererComponent* meshRenderer);
	void drawPhysicsBody(PhysicsBodyComponent* physicsBody);
};