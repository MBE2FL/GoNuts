#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <sqlite3.h>
#include "EntityFactory.h"
#include "TransformSystem.h"
#include "MeshRendererSystem.h"
#include "PhysicsSystem.h"
#include "GUIHelper.h"


#define WINDOW_WIDTH			1900
#define WINDOW_HEIGHT			1000
#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;
#define FIXED_DELTA_TIME		0.01667f

using std::stof;


struct EntityLoad
{
	string name = "";
	int transformID = -1;
	int meshRendererID = -1;
	int physicsBodyID = -1;
};

struct TransformLoad
{
	TransformComponent* transform = nullptr;
	string parentName = "";
	vector<string> childrenNames;
};


class Scene
{
public:
	Scene(const string& name);
	~Scene();

	void update(float deltaTime);
	void draw();
	void imguiDraw();
	string getName() const;
	void setName(const string& name);
	string getFilename() const;
	void setFilename(const string& filename);
	vector<Entity*> getEntities() const;
	void saveScene();
	void loadScene();
	void loadSceneFromFile(const string& path);
	EntityManager* getEntityManager() const;

	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

private:
	string _name;
	string _filename;
	EntityManager* _entityManager;
	EntityFactory* _entityFactory;
	TransformSystem* _transformSystem;
	MeshRendererSystem* _meshRendererSystem;
	PhysicsSystem* _physicsSystem;

	TransformComponent* _playerTransform;
	PhysicsBodyComponent* _playerPhysicsBody;
	TransformComponent* _mainCameraTransform;

	Light* light;
	Light* spotLight;

	GUIHelper* _guiHelper = nullptr;

	bool sliding = false;

	void errorCheck(sqlite3* db, char* errMsg);
	void saveTransforms(sqlite3* db, char* errMsg);
	void saveMeshRenderers(sqlite3* db, char* errMsg);
	void savePhysicsBodies(sqlite3* db, char* errMsg);
	void saveEntities(sqlite3* db, char* errMsg);

	static int storeEntityCallback(void* data, int numRows, char** rowFields, char** colNames);
	static int storeTransformCallback(void* data, int numRows, char** rowFields, char** colNames);
	//static int loadTransformCallback
	void loadEntities(sqlite3* db, char* errMsg);

	vector<EntityLoad> _entityLoads;
};