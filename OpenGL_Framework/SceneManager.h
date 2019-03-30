#pragma once
#include "Scene.h"

class SceneManager
{
public:
	static SceneManager* getInstance();
	~SceneManager();

	void addScene(Scene* scene);
	void loadSceneFromFile(const string& path, const string& name, bool inGameUi);
	void loadSceneFromFile(const string& path, const string& name, bool ScoreboardUi, int forscore);
	void loadScene(const string& name);
	void loadOldFaithful(const string& name);
	void loadMainMenu(const string& name);
	void saveScene();
	void saveSceneAs(const string& name);
	Scene* getCurrentScene() const;
	vector<Scene*> getScenes() const;

private:
	SceneManager();
	static SceneManager* _instance;

	Scene* _currentScene;
	vector<Scene*> _scenes;
};