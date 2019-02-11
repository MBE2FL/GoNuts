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
	_scenes.push_back(scene);
}

void SceneManager::loadSceneFromFile(const string & path, const string & name)
{
	Scene* scene = new Scene(name);
	//EntityManager::setInstance(scene->getEntityManager());
	//EntityFactory::getInstance()->setEntityManager();
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

void SceneManager::saveScene()
{
	_currentScene->saveScene();
}

void SceneManager::saveSceneAs(const string & name)
{
	_currentScene->saveSceneAs(name);
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
	/*sqlite3* db;
	char* msgErr;

	int exit = 0;
	exit = sqlite3_open("Scenes.db", &db);

	if (exit != SQLITE_OK)
	{
		cout << "Could not open scenes database!" << endl;
		return;
	}


	cout << "Opened scenes database successfully." << endl;*/


	//// Create a table for tranforms.
	//string createTable = "CREATE TABLE IF NOT EXISTS main.Transforms("
	//	"ID	INT PRIMARY	KEY			NOT NULL, "
	//	"Name			TEXT					NOT NULL );";

	//exit = sqlite3_exec(db, createTable.c_str(), NULL, 0, &msgErr);

	//if (exit != SQLITE_OK)
	//{
	//	cout << "Could not create entity table! " << msgErr << endl;
	//	sqlite3_free(msgErr);
	//}
	//else
	//	cout << "Entity table created successfully." << endl;

	//// Create a table for entities.
	//createTable = "CREATE TABLE IF NOT EXISTS main.Entities("
	//	"ID				INT						NOT NULL, "
	//	"Name			TEXT					NOT NULL, "
	//	"Transform_ID	INT								, "
	//	"PRIMARY KEY (ID), "
	//	"FOREIGN KEY (Transform_ID) REFERENCES Transforms (ID) );";

	//exit = sqlite3_exec(db, createTable.c_str(), NULL, 0, &msgErr);

	//if (exit != SQLITE_OK)
	//{
	//	cout << "Could not create entity table! " << msgErr << endl;
	//	sqlite3_free(msgErr);
	//}
	//else
	//	cout << "Entity table created successfully." << endl;

	//// TO-DO
	//// Re-create all scenes


	//sqlite3_close(db);
}
