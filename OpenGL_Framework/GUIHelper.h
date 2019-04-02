#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_freeglut.h"
#include "imgui/imgui_impl_opengl3.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "PhysicsBodyComponent.h"
#include "CameraComponent.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "ObjectLoader.h"
#include <vector>
#include <algorithm>
#include "SceneManager.h"

using std::vector;
using std::partition;

class GUIHelper
{
public:
	static GUIHelper* getInstance();
	~GUIHelper();

	void init(float width, float height);
	void draw();

	void update();

	bool getPhysicsDebugEnabled() const;
	static char* projToChar(ProjectionType type);
	static ProjectionType charToProj(char* type);

	static char* tagToChar(TTag tag);
	static TTag charToTag(char* tag);

private:
	static GUIHelper* _instance;

	bool _showSceneEditor = false;
	bool _showPropertyEditor = false;
	bool _showShaderProgCreator = false;
	bool _showSpawnEntity = false;
	bool _enablePhysicsDebug = false;
	bool _showSceneSelector = false;
	bool _showSceneSaveModal = false;
	bool _showLightsMenu = false;
	bool _showUIEditor = false;
	bool _showSpawnUIElement = false;
	bool _showSpawnUICanvas = false;
	bool _showUIAnimationEditor = false;
	bool _showParticleManagerEditor = false;
	bool _showParticleSystemEditor = false;

	TransformComponent* _currentTransform = nullptr;
	UIImage* _currentUIImage = nullptr;
	UICanvas* _currentUICanvas = nullptr;
	EntityManager* _entityManager = nullptr;
	EntityFactory* _entityFactory = nullptr;
	SceneManager* _sceneManager = nullptr;
	UISystem* _uiSystem = nullptr;
	ParticleManager* _particleManager = nullptr;
	ParticleEffect* _currentParticleEffect = nullptr;
	ParticleSystem* _currentParticleSystem = nullptr;

	GUIHelper();
	void drawMenuBar();
	void drawHierarchy();
	void drawHierarchyHelper(TransformComponent* transform);
	void propertyEditor(TransformComponent* transform, bool* open);
	void drawMeshRenderer(MeshRendererComponent* meshRenderer, Collider* collider);
	void drawPhysicsBody(PhysicsBodyComponent* physicsBody);
	void drawCamera(CameraComponent* camera);
	void drawCollider(Collider* collider);
	void SpawnEntity();
	void drawScenes();
	void drawSceneEditor();
	void drawTransforms();
	void drawLights();
	void drawUIEditor();
	void SpawnUIElement();
	void SpawnUICanvas();
	void drawCanvases();
	void drawUIHierarchy(UICanvas* canvas);
	void drawUIHierarchyHelper(UIImage* image, bool* showImageProperty);
	void propertyUIEditor(const string& canvasName, UIImage* image, bool* open);
	void drawUIAnimator(UIAnimator* animator);
	void drawUIAnimationEditor();
	void propertyUIAnimationEditor(UIAnimation* anim, bool* open);
	void drawParticleManagerEditor();
	void drawParticleEffects();
	void drawParticleEffectHierarchy(ParticleEffect* particleEffect);
	void drawParticleSystemEditor(bool* open);
	void drawParticles();

	unordered_map<UICanvas*, bool> _canvasShowImageProperty;
};