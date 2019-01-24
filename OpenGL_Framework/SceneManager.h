#pragma once
#include "Scene.h"

class SceneManager
{
public:
	static SceneManager* getInstance();
	~SceneManager();

	void addScene(Scene* scene);
	void loadSceneFromFile(const string& path, const string& name);
	void loadScene(const string& name);
	void saveScene();
	Scene* getCurrentScene() const;

private:
	SceneManager();
	static SceneManager* _instance;

	Scene* _currentScene;
	vector<Scene*> _scenes;
};