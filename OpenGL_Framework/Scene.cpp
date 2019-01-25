#include "Scene.h"

Scene::Scene(const string & name)
{
	_name = name;
	_entityManager = new EntityManager();
	_transformSystem = new TransformSystem(_entityManager);
	_meshRendererSystem = new MeshRendererSystem(_entityManager);
	_physicsSystem = new PhysicsSystem(_entityManager);

#ifdef _DEBUG
	_guiHelper = GUIHelper::getInstance();
#endif
}

Scene::~Scene()
{
}

void Scene::update(float deltaTime)
{
	if (_playerTransform->getLocalPosition().y < -6.0f)
	{
		_playerTransform->setWorldPosition(Vector3(-3.0f, 8.0f, -5.0f));
		_playerPhysicsBody->setVelocity(Vector3::Zero);
		_playerTransform->setLocalScale(0.2f);
	}

	_transformSystem->update(FIXED_DELTA_TIME);
	_physicsSystem->update(FIXED_DELTA_TIME);


	Vector3 offset(-6, -1.5f, -8);
	_mainCameraTransform->setWorldPosition(MathLibCore::lerp(_mainCameraTransform->getWorldPosition(),
		_playerTransform->getWorldPosition() - offset, deltaTime * 3.0f));
}

void Scene::draw()
{
#ifdef _DEBUG
	// New imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();

	// Update imgui widgets
	imguiDraw();

	// Render imgui
	ImGui::Render();
#endif

	_meshRendererSystem->draw(light, spotLight);


	// Update imgui draw data
	glUseProgram(GL_NONE);
#ifdef _DEBUG

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Scene::imguiDraw()
{
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		Vector3 pos; //= camera.getWorldPosition();
		//ImGui::Text("Camera Position: (%f, %f, %f)", pos.x, pos.y, pos.z);
		//ImGui::DragFloat3("Camera Position: ", &pos.x, 0.5f);
		//camera.setWorldPosition(pos);
		pos = _playerTransform->getWorldPosition();
		ImGui::Text("Player Pos: (%f, %f, %f)", pos.x, pos.y, pos.z);


		// Light settings
		if (ImGui::CollapsingHeader("Light Settings:"))
		{
			// Position settings
			Vector3 position = light->getPosition();
			ImGui::DragFloat3("Light Position: ", &position.x, 0.5f);
			light->setPosition(position);
			// Ambient settings
			Vector3 ambient = light->getAmbient();
			ImGui::ColorEdit3("Ambient Colour: ", &ambient.x);
			light->setAmbient(ambient);
			// Diffuse settings
			Vector3 diffuse = light->getDiffuse();
			ImGui::ColorEdit3("Diffuse Colour: ", &diffuse.x);
			light->setDiffuse(diffuse);
			// Specular settings
			Vector3 specular = light->getSpecular();
			ImGui::ColorEdit3("Specular Colour: ", &specular.x);
			light->setSpecular(specular);
			// Specular exponent settings
			float specularExp = light->getSpecularExp();
			ImGui::SliderFloat("Specular Exp: ", &specularExp, 0.0f, 250.0f);
			light->setSpecularExp(specularExp);
			// Attenuation constant settings
			float attenuationConstant = light->getAttenuationConstant();
			ImGui::SliderFloat("Attenuation Constant: ", &attenuationConstant, 0.0f, 20.0f);
			light->setAttenuationConstant(attenuationConstant);
			// Attenuation linear settings
			float attenuationLinear = light->getAttenuationLinear();
			ImGui::SliderFloat("Attenuation Linear: ", &attenuationLinear, 0.0f, 5.0f);
			light->setAttenuationLinear(attenuationLinear);
			// Attenuation quadratic settings
			float attenuationQuadratic = light->getAttenuationQuadratic();
			ImGui::SliderFloat("Attenuation Quadratic: ", &attenuationQuadratic, 0.0f, 5.0f);
			light->setAttenuationQuadratic(attenuationQuadratic);


			//Vector3 spotPositiion = spotLight->getPosition();
			//ImGui::SliderFloat3("Spot Position: ", &offse.x, -60.f, 60.f);
			//spotLight->setPosition(spotPositiion);
		}

		_guiHelper->draw();

		if (ImGui::Button("Save Scene"))
			saveScene();


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}

string Scene::getName() const
{
	return _name;
}

void Scene::setName(const string & name)
{
	_name = name;
}

string Scene::getFilename() const
{
	return _filename;
}

void Scene::setFilename(const string & filename)
{
	_filename = filename;
}

vector<Entity*> Scene::getEntities() const
{
	return _entityManager->getEntities();
}

void Scene::saveScene()
{
	sqlite3* db;
	char* errMsg = 0;
	int exit = 0;

	// Open the database
	exit = sqlite3_open("./Assets/Scenes/Scenes.db", &db);

	if (exit != SQLITE_OK)
	{
		cout << "Could not open scenes database!" << endl;
		return;
	}

	cout << "Opened scenes database successfully." << endl;

	// Save all transforms.
	saveTransforms(db, errMsg);
	// Save all mesh renderers.
	saveMeshRenderers(db, errMsg);
	// Save all physics bodies.
	savePhysicsBodies(db, errMsg);
	// Save all entities.
	saveEntities(db, errMsg);

	sqlite3_close(db);
}

void Scene::loadScene()
{
#ifdef _DEBUG

	_guiHelper->update();
#endif

	_entityFactory = EntityFactory::getInstance();

	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

	Entity* mainCamera = _entityFactory->createPerspectiveCamera(Vector3(0.0f, 4.0f, 5.0f), 60.0f, aspect, 1.0f, 1000.0f);
	_mainCameraTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, mainCamera);

	Entity* player = _entityFactory->createPlayer(Vector3(-3.0f, 10.0f, -5.0f), Vector3(0.2f));
	_playerTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, player);
	_playerPhysicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, player);
	EntityManager::setPlayerTransform(_playerTransform);

	Entity* entity = _entityFactory->createCoin(Vector3(2.0f, 4.0f, -5.0f), Vector3::One);
	_entityFactory->createCoin(Vector3(2.0f, 0.0f, 0.0f), Vector3::One, entity);

	_entityFactory->createBackgrounds(20, Vector3(100.0f, -5.0f, -20.0f), Vector3(25.0f, 25.0f, 1.0f));

	_entityFactory->createPlatforms(15, Vector3(14.0f, -2.0f, -5.0f));

	_entityFactory->createCoins(1, Vector3(35.0f, 4.0f, -5.0f), Vector3::One, 0);
	_entityFactory->createCoins(2, Vector3(42.0f, 4.5f, -5.0f), Vector3::One, 35.0f);
	_entityFactory->createAcorns(3, Vector3(2.0f, 4.0f, -5.0f), Vector3::One, 12.0f);
	_entityFactory->createAcorns(3, Vector3(2.0f, 4.0f, -5.0f), Vector3::One, 54.0f);

	_entityFactory->createCones(2, Vector3(14.0f, 2.6f, -5.0f), Vector3::One, 70.0f);
	_entityFactory->createSpikes(2, Vector3(14.0f, 2.8f, -5.0f), Vector3::One, 28.0f);
	_entityFactory->createVents(2, Vector3(15.0f, 2.95f, -5.0f), Vector3::One, 98.0f);
	_entityFactory->createTopPlatforms(5, Vector3(14.0f, 4.2f, -5.0f), Vector3(0.4f, 1, 1), 125.0f);


	light = new Light();
	light->setPosition(Vector3(4.0f, 0.0f, 0.0f));
	light->setAmbient(Vector3(0.20f, 0.78f, 0.79f));
	//light->setAmbient(Vector3(0));
	light->setDiffuse(Vector3(1.0f, 0.0f, 0.0f));
	//light->setDiffuse(Vector3(0));
	light->setSpecular(Vector3(0.54f, 1.0f, 0.0f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);

	spotLight = new Light();
	spotLight->setPosition(Vector3(-3.2f, 30.0f, -28.0f));
	spotLight->setAmbient(Vector3(1.0f, 1.0f, 1.0f));
	spotLight->setDiffuse(Vector3(1));
	spotLight->setSpecular(Vector3(1.0f, 0.1f, 0.1f));
	spotLight->setSpecularExp(100.0f);
	spotLight->setAttenuationConstant(0.1f);
	spotLight->setAttenuationLinear(0.01f);
	spotLight->setAttenuationQuadratic(0.01f);
}

void Scene::loadSceneFromFile(const string & path)
{
	sqlite3* db;
	char* errMsg = 0;
	int exit = 0;

	// Open the database
	exit = sqlite3_open(path.c_str(), &db);

	if (exit != SQLITE_OK)
	{
		cout << "Could not open scenes database!" << endl;
		return;
	}

	cout << "Opened scenes database successfully." << endl;

	// Make sure scene has no existing entities.
	//TO-DO

	loadEntities(db, errMsg);
}

EntityManager * Scene::getEntityManager() const
{
	return _entityManager;
}

void Scene::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;

	if (io.WantCaptureKeyboard)
	{
		io.AddInputCharacter(key);
		return;
	}
#endif

	switch (key)
	{
	case 32:
		/*if (camera.getProjType() == ProjectionType::Perspective)
			camera.orthographic(-10.0f, 10.0f, 10.0f, -10.0f, -100.0f, 100.0f);
		else
		{
			float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
			camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
		}*/
		break;
	case 27: // the escape key
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
	if (key == 'w' && !sliding && _playerPhysicsBody->getCanJump())
	{
		_playerPhysicsBody->addForce(Vector3(0, 350.0f, 0.0f));
	}
	if (key == 's' && _playerPhysicsBody->getCanJump())
	{
		sliding = true;
		_playerTransform->setLocalScale(0.1f);
		_playerTransform->setWorldPosition(Vector3(_playerTransform->getLocalPosition().x,
			_playerTransform->getLocalPosition().y - 0.38f, _playerTransform->getWorldPosition().z));
	}
	if (key == 'i')//up
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, LP.y, --LP.z));
	}
	if (key == 'k')//down
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, LP.y, ++LP.z));
	}
	if (key == 'j')//left
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(--LP.x, LP.y, LP.z));
	}
	if (key == 'l')//right
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(++LP.x, LP.y, LP.z));
	}if (key == 'u')//right
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, ++LP.y, LP.z));
	}if (key == 'o')//right
	{
		Vector3 LP = spotLight->getPosition();
		spotLight->setPosition(Vector3(LP.x, --LP.y, LP.z));
	}
}

void Scene::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;

	if (io.WantCaptureKeyboard)
		return;
#endif

	switch (key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
#ifdef _DEBUG

#endif
	if (key == 's')
	{
		if (sliding)
		{
			_playerTransform->setWorldPosition(Vector3(_playerTransform->getWorldPosition().x,
				_playerTransform->getWorldPosition().y + 0.45f, _playerTransform->getWorldPosition().z));
			sliding = false;
			_playerTransform->setLocalScale(0.2f);
		}
	}
}

void Scene::mouseClicked(int button, int state, int x, int y)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
	//io.MouseDown[button] = state;

	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			io.MouseDown[0] = true;
			break;
		case GLUT_RIGHT_BUTTON:
			io.MouseDown[1] = true;
			break;
		case GLUT_MIDDLE_BUTTON:
			io.MouseDown[2] = true;
			break;
		}
	}
	else
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			io.MouseDown[0] = false;
			break;
		case GLUT_RIGHT_BUTTON:
			io.MouseDown[1] = false;
			break;
		case GLUT_MIDDLE_BUTTON:
			io.MouseDown[2] = false;
			break;
		}
	}
#endif
}

void Scene::mouseMoved(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
}

void Scene::errorCheck(sqlite3 * db, char * errMsg)
{
}

void Scene::saveTransforms(sqlite3 * db, char * errMsg)
{
	int exit = 0;
	vector<TransformComponent*> transforms = _entityManager->getAllTransforms();

	// Clear old save data.
	string sql = "DELETE FROM Transforms;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not delete Transforms!. " << errMsg << endl;
	else
		cout << "Successfully deleted Transforms." << endl;

	sql = "VACUUM;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not Vacuum!. " << errMsg << endl;
	else
		cout << "Successfully Vacuumed." << endl;


	// Save new data.
	for (TransformComponent* transform : transforms)
	{
		string name = transform->getName();
		Vector3 position = transform->getLocalPosition();
		Vector3 rotation = transform->getLocalRotation();
		Vector3 orbRot = transform->getOrbitRotation();
		Vector3 scale = transform->getLocalScale();
		TransformComponent* parent = transform->getParent();
		vector<TransformComponent*> children = transform->getChildren();


		sql = "INSERT INTO Transforms (ID, Name, [Pos.X], [Pos.Y], [Pos.Z], [Rot.X], "
			"[Rot.Y], [Rot.Z], [OrbRot.X], [OrbRot.Y], [OrbRot.Z], [Scale.X], [Scale.Y], [Scale.Z]";

		if (parent)
			sql += ", Parent";

		if (children.size() > 0)
			sql += ", Children";

		sql += ") VALUES (";

		sql += to_string(transform->getEntity()->getEid()) + ", ";
		sql += "'" + name + "'" + ", ";
		sql += to_string(position.x) + ", " + to_string(position.y) + ", " + to_string(position.z) + ", ";
		sql += to_string(rotation.x) + ", " + to_string(rotation.y) + ", " + to_string(rotation.z) + ", ";
		sql += to_string(orbRot.x) + ", " + to_string(orbRot.y) + ", " + to_string(orbRot.z) + ", ";
		sql += to_string(scale.x) + ", " + to_string(scale.y) + ", " + to_string(scale.z);

		if (parent)
		{
			sql += ", ";
			sql += "'" + parent->getName() + "'";
		}

		if (children.size() > 0)
		{
			sql += ", '";

			for (TransformComponent* child : children)
			{
				sql += "<" + child->getName() + ">";
			}

			sql += "'";
		}

		sql += ");";


		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

		if (exit != SQLITE_OK)
			cout << "Could not insert element into Transforms!. " << errMsg << endl;
		else
			cout << "Successfully inserted element into Transforms." << endl;
	}
}

void Scene::saveMeshRenderers(sqlite3 * db, char * errMsg)
{
	int exit = 0;
	vector<MeshRendererComponent*> meshRenderers = _entityManager->getAllMeshRenderers();

	// Clear old save data.
	string sql = "DELETE FROM [Mesh Renderers];";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
	{
		cout << "Could not delete Mesh Renderers!. " << errMsg << endl;
		return;
	}
	else
		cout << "Successfully deleted Mesh Renderers." << endl;

	sql = "VACUUM;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
	{
		cout << "Could not Vacuum!. " << errMsg << endl;
		return;
	}
	else
		cout << "Successfully Vacuumed." << endl;


	// Save new data.
	for (MeshRendererComponent* meshRenderer : meshRenderers)
	{
		string meshName = meshRenderer->getMesh()->getName();
		string shaderName = meshRenderer->getShaderProgram()->getProgramName();
		bool isTrans = meshRenderer->getIsTransparent();
		vector<Texture*> textures = meshRenderer->getTextures();


		sql = "INSERT INTO [Mesh Renderers] (ID, Mesh, [Shader Program], Transparent";

		if (textures.size() > 0)
			sql += ", Textures";

		sql += ") VALUES (";

		sql += to_string(meshRenderer->getEntity()->getEid()) + ", ";
		sql += "'" + meshName + "'" + ", ";
		sql += "'" + shaderName + "'" + ", ";
		sql += to_string(isTrans);

		if (textures.size() > 0)
		{
			sql += ", '";

			for (Texture* texture : textures)
			{
				sql += "<" + texture->getName() + ">";
			}

			sql += "'";
		}

		sql += ");";


		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

		if (exit != SQLITE_OK)
			cout << "Could not insert element into Mesh Renderers!. " << errMsg << endl;
		else
			cout << "Successfully inserted element into Mesh Renderers." << endl;
	}
}

void Scene::savePhysicsBodies(sqlite3 * db, char * errMsg)
{
	int exit = 0;
	vector<PhysicsBodyComponent*> physicsBodies = _entityManager->getAllPhysicsBodyComponents();

	// Clear old save data.
	string sql = "DELETE FROM [Physics Bodies];";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not delete Physics Bodies!. " << errMsg << endl;
	else
		cout << "Successfully deleted Physics Bodies." << endl;

	sql = "VACUUM;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not Vacuum!. " << errMsg << endl;
	else
		cout << "Successfully Vacuumed." << endl;


	// Save new data.
	for (PhysicsBodyComponent* physicsBody : physicsBodies)
	{
		float mass = physicsBody->getMass();
		bool useGravity = physicsBody->getUseGravity();
		bool canJump = physicsBody->getCanJump();
		Vector3 velocity = physicsBody->getVelocity();
		Vector3 maxVelocity = physicsBody->getMaxVelocity();


		sql = "INSERT INTO [Physics Bodies] (ID, Mass, [Use Gravity], [Can Jump], [Velocity.X], [Velocity.Y], "
			"[Velocity.Z], [Max Velocity.X], [Max Velocity.Y], [Max Velocity.Z]) VALUES (";

		sql += to_string(physicsBody->getEntity()->getEid()) + ", ";
		sql += to_string(mass) + ", ";
		sql += to_string(useGravity) + ", ";
		sql += to_string(canJump) + ", ";
		sql += to_string(velocity.x) + ", " + to_string(velocity.y) + ", " + to_string(velocity.z) + ", ";
		sql += to_string(maxVelocity.x) + ", " + to_string(maxVelocity.y) + ", " + to_string(maxVelocity.z) + ");";


		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

		if (exit != SQLITE_OK)
			cout << "Could not insert element into Physics Bodies!. " << errMsg << endl;
		else
			cout << "Successfully inserted element into Physics Bodies." << endl;
	}
}

void Scene::saveEntities(sqlite3 * db, char * errMsg)
{
	int exit = 0;

	// Clear old save data.
	string sql = "DELETE FROM Entities;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not delete Entities!. " << errMsg << endl;
	else
		cout << "Successfully deleted Entities." << endl;

	sql = "VACUUM;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not Vacuum!. " << errMsg << endl;
	else
		cout << "Successfully Vacuumed." << endl;


	// Save new data.
	vector<Entity*> entities = _entityManager->getEntities();
	for (Entity* entity : entities)
	{
		// Get all components for the current entity.
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);
		if (!transform)
			continue;

		MeshRendererComponent* meshRenderer = _entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, entity);
		PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, entity);

		unsigned int eid = entity->getEid();

		// Insert all the information for the current entity.
		string name = transform->getName();


		sql = "INSERT INTO Entities (ID, Name, Transform, [Mesh Renderer], [Physics Body]) VALUES (";

		sql += to_string(eid) + ", ";
		sql += "'" + transform->getName() + "', ";
		sql += to_string(eid);

		if (meshRenderer)
			sql += ", " + to_string(eid);
		else
			sql += ", NULL";

		if (physicsBody)
			sql += ", " + to_string(eid);
		else
			sql += ", NULL";

		
		sql += ");";


		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

		if (exit != SQLITE_OK)
			cout << "Could not insert element into Entities!. " << errMsg << endl;
		else
			cout << "Successfully inserted element into Entities." << endl;
	}
}

int Scene::storeEntityCallback(void * data, int numCols, char ** rowFields, char ** colNames)
{
	//EntityFactory* entityFactory = EntityFactory::getInstance();
	//Scene* scene = static_cast<Scene*>(data);
	EntityLoad load;
	int transformID;
	int meshRendererID;
	int physicsBodyID;
	//int cameraID;

	//for (unsigned int i = 0; i < numCols; ++i)
	//{
	//	// Retrieve the name of the entity, and determine which function to call in the entity factory.
	//	sscanf(rowFields[1], "%s", name);
	//	sscanf(rowFields[2], "%i", transformID);
	//	sscanf(rowFields[3], "%i", meshRendererID);
	//	sscanf(rowFields[4], "%i", physicsBodyID);
	//}

	sscanf(rowFields[2], "%i", &transformID);
	load.name = rowFields[1];
	load.transformID = transformID;


	if (rowFields[3])
	{
		sscanf(rowFields[3], "%i", &meshRendererID);
		load.meshRendererID = meshRendererID;
	}
	if (rowFields[4])
	{
		sscanf(rowFields[4], "%i", &physicsBodyID);
		load.physicsBodyID = physicsBodyID;
	}

	static_cast<vector<EntityLoad>*>(data)->push_back(load);

	//if (name == "Camera")
	//	entityFactory->createPerspectiveCamera();
	


	return 0;
}
int Scene::storeTransformCallback(void * data, int numRows, char ** rowFields, char ** colNames)
{
	//TransformComponent* transform = static_cast<TransformComponent*>(data);
	TransformComponent* transform = new TransformComponent();
	char* name = rowFields[1];
	Vector3 pos = Vector3(stof(rowFields[2]), stof(rowFields[3]), stof(rowFields[4]));
	Vector3 rot = Vector3(stof(rowFields[5]), stof(rowFields[6]), stof(rowFields[7]));
	Vector3 orbRot = Vector3(stof(rowFields[8]), stof(rowFields[9]), stof(rowFields[10]));
	Vector3 scale = Vector3(stof(rowFields[11]), stof(rowFields[12]), stof(rowFields[13]));
	//string parent = rowFields[14];
	string children = rowFields[15];
	vector<string> childrenNames;


	transform->setName(name);
	transform->setLocalPosition(pos);
	transform->setLocalRotation(rot);
	transform->setOrbitRotation(orbRot);
	transform->setLocalScale(scale);

	TransformLoad transformLoad;
	transformLoad.transform = transform;
	transformLoad.parentName = rowFields[14];

	
	transformLoad.childrenNames;

	static_cast<vector<TransformLoad>*>(data)->push_back(transformLoad);

	return 0;
}
void Scene::loadEntities(sqlite3 * db, char * errMsg)
{
	string sql = "SELECT * FROM Entities;";
	int exit = 0;
	vector<TransformLoad> transformLoads;


	exit = sqlite3_exec(db, sql.c_str(), storeEntityCallback, &_entityLoads, &errMsg);

	if (exit != SQLITE_OK)
	{
		cout << "Failed to select rows from Entities! " << errMsg << endl;
		return;
	}
	else
		cout << "Successfully selected rows from Entities." << endl;

	for (EntityLoad load : _entityLoads)
	{
		sql = "SELECT * FROM Transforms WHERE ID = ";
		sql += to_string(load.transformID) + ";";

		TransformComponent* transform = new TransformComponent();

		exit = sqlite3_exec(db, sql.c_str(), storeTransformCallback, &transformLoads, &errMsg);

		TransformLoad transformLoad;
		transformLoad.transform = transform;
		
		if (transform->getParent())
	}
}
