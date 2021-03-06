#include "SceneManager.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager * SceneManager::getInstance()
{
	if (!_instance)
	{
		_instance = new SceneManager();
	}

	return _instance;
}

SceneManager::~SceneManager()
{
}

void SceneManager::addScene(Scene * scene)
{
	if (scene)
		_scenes.push_back(scene);
	else
	{
		cerr << "Could not add scene!" << endl;
	}
}

void SceneManager::loadSceneFromFile(const string & path, const string & name, bool inGameUi)
{
	Scene* scene = new Scene(name, inGameUi);
	//EntityManager::setInstance(scene->getEntityManager());
	//EntityFactory::getInstance()->setEntityManager();
	scene->loadSceneFromFile(path);
	_scenes.push_back(scene);
}

void SceneManager::loadSceneFromFile(const string & path, const string & name, bool ScoreboardUi, int forscore)
{
	Scene* scene = new Scene(name, ScoreboardUi, forscore);
	scene->loadSceneFromFile(path);
	_scenes.push_back(scene);
}

void SceneManager::loadSceneFromFile(const string & path, const string & name, bool ScoreboardUi, string fordialogue)
{
	Scene* scene = new Scene(name, ScoreboardUi, fordialogue);
	scene->loadSceneFromFile(path);
	_scenes.push_back(scene);
}

void SceneManager::loadScene(const string & name)
{
	for (Scene* scene : _scenes)
	{
		if (scene->getName() == name)
		{
			_currentScene = scene;
			scene->loadScene();
			return;
		}
	}
}

void SceneManager::loadOldFaithful(const string & name)
{
	for (Scene* scene : _scenes)
	{
		if (scene->getName() == name)
		{
			_currentScene = scene;
			scene->loadOldFaithful();
			return;
		}
	}
}

void SceneManager::loadMainMenu(const string & name)
{
	for (Scene* scene : _scenes)
	{
		if (scene->getName() == name)
		{
			_currentScene = scene;
			scene->loadMainMenu();
			return;
		}
	}
}

void SceneManager::saveScene()
{
	_currentScene->saveScene();
}

void SceneManager::saveSceneAs(const string & name)
{
	Scene* newScene = _currentScene->saveSceneAs(name);
	addScene(newScene);
}

Scene * SceneManager::getCurrentScene() const
{
	return _currentScene;
}

vector<Scene*> SceneManager::getScenes() const
{
	return _scenes;
}

SceneManager::SceneManager()
{
}
