#include "Scene.h"

#include "GUIHelper.h"


Scene::Scene(const string & name)
{
	_name = name;
	_entityManager = new EntityManager();
	_transformSystem = new TransformSystem(_entityManager);
	_meshRendererSystem = new MeshRendererSystem(_entityManager);
	_physicsSystem = new PhysicsSystem(_entityManager);
	_entityFactory = EntityFactory::getInstance();

	_uiSystem = new UISystem(_entityManager);
	_uiCamera = _uiSystem->getCamera();

#ifdef _DEBUG
	_guiHelper = GUIHelper::getInstance();
#endif
}

Scene::~Scene()
{
}

void Scene::update(float deltaTime)
{
	if (_entityFactory->getAcornCount() > 18)
	{
		_entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, _playerTransform->getEntity())->setMesh(ObjectLoader::getMesh("Beast Mode"));
		_entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, _playerTransform->getEntity())->setTexture(0, ObjectLoader::getTexture("Beast Mode"));
		_entityManager->getComponent<Collider*>(ComponentType::Collider, _playerTransform->getEntity())->setBounds(ObjectLoader::getMesh("Beast Mode")->getMeshBounds());
		_playerTransform->setLocalScale(vec3(1.5f));
		_entityManager->getComponent<Collider*>(ComponentType::Collider, _playerTransform->getEntity())->beastMode = true;
	}

	light->setPosition(vec3(_playerTransform->getLocalPosition().x - 2.0f, light->getPosition().y, light->getPosition().z));
	if (_playerTransform->getLocalPosition().y < -6.0f)
	{
		front = true;
		_playerTransform->setWorldPosition(vec3(-3.0f, 8.0f, -5.0f));
		_playerPhysicsBody->setVelocity(vec3(0.0f));
		_playerTransform->setLocalScale(vec3(0.2f));
	}

	_transformSystem->update(FIXED_DELTA_TIME);
	_physicsSystem->update(FIXED_DELTA_TIME);

	if (front /*&& !_playerPhysicsBody->getCanJump()*/)
	{
		_playerTransform->setWorldPosition(MathUtils::lerp(_playerTransform->getWorldPosition(),//starting position for when it is pressed 
			vec3(_playerTransform->getWorldPosition().x, _playerTransform->getWorldPosition().y, -5.0f),//where we want to go with lerp
			FIXED_DELTA_TIME * 3.0f));// lerp time
	}
	else if (!front /*&& !_playerPhysicsBody->getCanJump()*/)
	{
		_playerTransform->setWorldPosition(MathUtils::lerp(_playerTransform->getWorldPosition(),//starting position for when it is pressed 
			vec3(_playerTransform->getWorldPosition().x, _playerTransform->getWorldPosition().y, -8.0f),//where we want to go with lerp
			FIXED_DELTA_TIME * 3.0f));// lerp time
	}

	//vec3 offset(-6, -1.5f, -8);
	//_mainCameraTransform->setWorldPosition(lerp(_mainCameraTransform->getWorldPosition(),
	//	_playerTransform->getWorldPosition() - offset, deltaTime * 3.0f));
	if (_followPlayer)
	{
		_mainCameraTransform->setTarget(_playerTransform, vec3(-6, -1.5f, -8));
		_mainCameraTransform->followTarget(deltaTime * 3.0f);
	}


	skeletalTest = true;
	if (skeletalTest && skeletalMeshTestTwo)
	{
		//skeletalMeshTest->update(deltaTime);
		skeletalMeshTestTwo->update(deltaTime);
	}

	_uiSystem->update(deltaTime);
}

void Scene::draw()
{
	_meshRendererSystem->draw(light, spotLight);

#ifdef _DEBUG
	if (_guiHelper->getPhysicsDebugEnabled())
	{
		// Retrieve the main camera.
		Entity* mainCamera = EntityManager::getMainCamera();
		vector<Collider*> colliders = _entityManager->getAllColliders();

		// Retrieve the necessary camera details.
		CameraComponent* _cameraComp = _entityManager->getComponent<CameraComponent*>(ComponentType::Camera, mainCamera);
		TransformComponent* _cameraTrans = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, mainCamera);
		//mat4 cameraInverse = _cameraTrans->getLocalToWorldMatrix().getInverse(_cameraTrans->getWorldRotation(), _cameraTrans->getWorldPosition());
		mat4 cameraInverse = _cameraTrans->getLocalToWorldMatrix().getInverse();

		for (Collider* collider : colliders)
		{
			ShaderProgram* bboxShader = ObjectLoader::getShaderProgram("BBox");
			bboxShader->bind();
			bboxShader->sendUniformMat4("uModel", mat4::Identity.data, false);
			bboxShader->sendUniformMat4("uView", cameraInverse.data, false);
			bboxShader->sendUniformMat4("uProj", _cameraComp->getProjection().data, false);

			collider->draw();
		}
	}
#endif


	glUseProgram(GL_NONE);


#ifdef _DEBUG
	// New imgui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();

	// Update imgui widgets
	imguiDraw();

	// Render imgui
	ImGui::Render();
#endif

#ifdef _DEBUG
	// Update imgui draw data
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Scene::drawUI()
{
	_uiSystem->draw();
}

void Scene::imguiDraw()
{
	_guiHelper->draw();
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
	string path = "./Assets/Scenes/" + _name + ".db";
	exit = sqlite3_open(path.c_str(), &db);

	if (exit != SQLITE_OK)
	{
		cout << "Could not open scenes database!" << endl;
		return;
	}

	cout << "Opened scenes database successfully." << endl;

	// Save all transforms.
	saveTransforms(db, errMsg);
	// Save all cameras.
	saveCameras(db, errMsg);
	// Save all mesh renderers.
	saveMeshRenderers(db, errMsg);
	// Save all physics bodies.
	savePhysicsBodies(db, errMsg);
	// Save all colliders
	saveColliders(db, errMsg);
	// Save all entities.
	saveEntities(db, errMsg);

	sqlite3_close(db);
}

Scene* Scene::saveSceneAs(const string & name)
{
	sqlite3* db;
	char* errMsg = 0;
	int exit = 0;
	Scene* newScene = new Scene(name);

	// Open the database.
	string path = "./Assets/Scenes/" + name + ".db";
	exit = sqlite3_open(path.c_str(), &db);

	if (exit != SQLITE_OK)
	{
		cout << "Could not open scenes database!" << endl;
		return nullptr;
	}

	cout << "Opened scenes database successfully." << endl;

	// Create tables to store the entities, and their components.
	createTables(db, errMsg);

	// Save all transforms.
	saveTransforms(db, errMsg);
	// Save all cameras.
	saveCameras(db, errMsg);
	// Save all mesh renderers.
	saveMeshRenderers(db, errMsg);
	// Save all physics bodies.
	savePhysicsBodies(db, errMsg);
	// Save all colliders
	saveColliders(db, errMsg);
	// Save all entities.
	saveEntities(db, errMsg);

	// Load save into new scene.
	//newScene->loadSceneFromFile(path);
	newScene->loadEntities(db, errMsg);

	sqlite3_close(db);


	return newScene;
}

void Scene::loadOldFaithful()
{
	EntityManager::setInstance(_entityManager);
	_entityFactory->setEntityManager();	// Optimize how entity factory and gui helper get updated instances

#ifdef _DEBUG

	_guiHelper->update();
#endif

	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

	_mainCamera = _entityFactory->createPerspectiveCamera(vec3(0.0f, 4.0f, 5.0f), 60.0f, aspect, 1.0f, 1000.0f);
	_mainCameraTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, _mainCamera);
	EntityManager::setMainCamera(_mainCamera);

	_shadowCamera = _entityFactory->createOrthographicCamera(100, -100, 100, -100, 300, -300);
	_shadowCameraTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, _shadowCamera);
	EntityManager::setShadowCamera(_shadowCamera);

	Entity* player = _entityFactory->createPlayer(vec3(-3.0f, 10.0f, -5.0f), vec3(0.2f));
	_playerTransform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, player);
	_playerPhysicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, player);
	EntityManager::setPlayerTransform(_playerTransform);

	Entity* entity = _entityFactory->createCoin(vec3(2.0f, 4.0f, -5.0f), vec3(1.0f));
	_entityFactory->createCoin(vec3(2.0f, 0.0f, 0.0f), vec3(1.0f), entity);



	_entityFactory->createBackgrounds(20, vec3(100.0f, -5.0f, -20.0f), vec3(25.0f, 25.0f, 1.0f));

	_entityFactory->createPlatforms(15, vec3(14.0f, -2.0f, -5.0f));

	_entityFactory->createCoins(1, vec3(35.0f, 4.0f, -5.0f), vec3(1.0f), 0);
	_entityFactory->createCoins(2, vec3(42.0f, 4.5f, -5.0f), vec3(1.0f), 35.0f);
	_entityFactory->createAcorns(3, vec3(2.0f, 4.0f, -5.0f), vec3(1.0f), 12.0f);
	_entityFactory->createAcorns(3, vec3(2.0f, 4.0f, -5.0f), vec3(1.0f), 54.0f);

	_entityFactory->createCones(2, vec3(14.0f, 2.6f, -5.0f), vec3(1.0f), 70.0f);
	_entityFactory->createSpikes(2, vec3(14.0f, 2.8f, -5.0f), vec3(1.0f), 28.0f);
	_entityFactory->createVents(2, vec3(15.0f, 2.95f, -5.0f), vec3(1.0f), 98.0f);
	_entityFactory->createTopPlatforms(5, vec3(14.0f, 4.2f, -5.0f), vec3(0.4f, 1, 1), 125.0f);


	entity = _entityFactory->createEmpty(vec3(1.0f, -2.0f, -3.4f), vec3(0.4f), nullptr, "Skeleton");
	skeletalMeshTest = new SkeletalMesh();
	//skeletalMeshTest->loadFromFile("./Assets/Better Collada Test/Character Running.dae");
	//skeletalMeshTest->loadFromFile("./Assets/model.dae");
	skeletalMeshTest->loadFromFileSMD("./Assets/Source Exports/Cube.smd", "./Assets/Source Exports/anims/ArmatureAction.smd", 24.0f);
	skeletalMeshTest->_isSkeletal = true;
	
	_entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity)->setLocalRotationAngleY(90.0f);
	vector<Texture*> textures = { ObjectLoader::getTexture("Anim Test Tex"), ObjectLoader::getTexture("Toon") };
	MeshRendererComponent* meshRenderer = new MeshRendererComponent(skeletalMeshTest, ObjectLoader::getShaderProgram("SkeletalAnim"), textures);
	_entityManager->addComponent(meshRenderer, entity);

	// ########## FBX MODEL
	//entity = _entityFactory->createEmpty(vec3(1.0f, 2.6f, -3.4f), vec3(0.4f), nullptr, "Skeleton");
	//skeletalMeshTest = new SkeletalMesh();
	////testSkeleton.loadFromFile("./Assets/FatBoi.dae");
	//skeletalMeshTest->loadFromFileFBX("./Assets/Character Running.fbx");
	//skeletalMeshTest->_isSkeletal = true;

	//_entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity)->setLocalRotationAngleY(90.0f);
	//vector<Texture*> textures = { ObjectLoader::getTexture("Anim Test Tex"), ObjectLoader::getTexture("Toon") };
	//MeshRendererComponent* meshRenderer = new MeshRendererComponent(skeletalMeshTest, ObjectLoader::getShaderProgram("SkeletalAnim"), textures);
	//_entityManager->addComponent(meshRenderer, entity);

	_followPlayer = false;
	entity = _entityFactory->createEmpty(vec3(0.0f, 0.8f, 0.0f), vec3(0.6f), nullptr, "SkeletonTwo");
	skeletalMeshTestTwo = new SkeletalMesh();
	//testSkeleton.loadFromFile("./Assets/FatBoi.dae");
	string path = "./Assets/Test Exporter/Character Running/";
	//string path = "./Assets/Test Exporter/Test/";
	skeletalMeshTestTwo->loadFromFileNUT(path + "Armature.nut", path + "Anims/ArmatureAction.nutAnim");
	//skeletalMeshTestTwo->loadFromFileNUT(path + "Armature.nut", path + "Anims/Jump.nutAnim");
	skeletalMeshTestTwo->_isSkeletal = true;

	_entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity)->setLocalRotationAngleX(-90.0f);
	//textures = { ObjectLoader::getTexture("FatBoi"), ObjectLoader::getTexture("Toon") };
	textures = { ObjectLoader::getTexture("Anim Test Tex"), ObjectLoader::getTexture("Toon") };
	meshRenderer = new MeshRendererComponent(skeletalMeshTestTwo, ObjectLoader::getShaderProgram("SkeletalAnim"), textures);
	_entityManager->addComponent(meshRenderer, entity);


	UICanvas* testCanvas = new UICanvas();

	UIImage* testImage = new UIImage(vec3(0.0f, 0.0f, -2.0f));
	testImage->setTexture(ObjectLoader::getTexture("FullNut"));

	testCanvas->addImage("Test", testImage);

	_uiSystem->addCanvas("TESTC", testCanvas);



	light = new Light();
	light->setPosition(vec3(4.0f, 0.0f, 0.0f));
	light->setAmbient(vec3(0.7f));
	//light->setAmbient(vec3(0));
	light->setDiffuse(vec3(0.6f));
	//light->setDiffuse(vec3(0));
	light->setSpecular(vec3(0.5f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);

	spotLight = new Light();
	spotLight->setPosition(vec3(-3.2f, 30.0f, -28.0f));
	spotLight->setAmbient(vec3(1.0f, 1.0f, 1.0f));
	spotLight->setDiffuse(vec3(1));
	spotLight->setSpecular(vec3(1.0f, 0.1f, 0.1f));
	spotLight->setSpecularExp(100.0f);
	spotLight->setAttenuationConstant(0.1f);
	spotLight->setAttenuationLinear(0.01f);
	spotLight->setAttenuationQuadratic(0.01f);
}

void Scene::loadScene()
{
	EntityManager::setInstance(_entityManager);
	_entityFactory->setEntityManager();	// Optimize how entity factory and gui helper get updated instances

#ifdef _DEBUG

	_guiHelper->update();
#endif


	EntityManager::setPlayerTransform(_playerTransform);
	EntityManager::setMainCamera(_mainCamera);

	light = new Light();
	light->setPosition(vec3(4.0f, 0.0f, 0.0f));
	light->setAmbient(vec3(0.7f));
	//light->setAmbient(vec3(0));
	light->setDiffuse(vec3(0.6f));
	//light->setDiffuse(vec3(0));
	light->setSpecular(vec3(0.5f));
	light->setSpecularExp(100.0f);
	light->setAttenuationConstant(1.0f);
	light->setAttenuationLinear(0.1f);
	light->setAttenuationQuadratic(0.01f);

	spotLight = new Light();
	spotLight->setPosition(vec3(-3.2f, 30.0f, -28.0f));
	spotLight->setAmbient(vec3(1.0f, 1.0f, 1.0f));
	spotLight->setDiffuse(vec3(1));
	spotLight->setSpecular(vec3(1.0f, 0.1f, 0.1f));
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
	case 32://space bar to jump

		if (!sliding && _playerPhysicsBody->getCanJump())
		{
			_playerPhysicsBody->addForce(vec3(0, 350.0f, 0.0f));
		}
		break;
	case 'c'://left control for sliding
		if (_playerPhysicsBody->getCanJump())
		{
			_playerTransform->setLocalScale(vec3(0.1f));
			if (!sliding)
			{
				_playerTransform->setWorldPosition(vec3(_playerTransform->getLocalPosition().x,
					_playerTransform->getLocalPosition().y - 0.38f, _playerTransform->getWorldPosition().z));
			}
			sliding = true;
		}
		break;
	case 27: // the escape key
	//case 'q': // the 'q' key
		exit(1);
		break;
	case 'p':
		//skeletalTest = !skeletalTest;
		ObjectLoader::getShaderProgram("SkeletalAnim")->reload();
		break;
	case 'b':
		skeletalMeshTestTwo->getAnimator()->nextFrame();
		break;
	}

	if (key == 'w' && !_playerPhysicsBody->getCanJump())
	{
		front = false;
	}

	if (key == 's' && !_playerPhysicsBody->getCanJump())
	{
		front = true;
	}
	if (key == 'i')//up
	{
		vec3 LP = spotLight->getPosition();
		spotLight->setPosition(vec3(LP.x, LP.y, --LP.z));
	}
	if (key == 'k')//down
	{
		vec3 LP = spotLight->getPosition();
		spotLight->setPosition(vec3(LP.x, LP.y, ++LP.z));
	}
	if (key == 'j')//left
	{
		vec3 LP = spotLight->getPosition();
		spotLight->setPosition(vec3(--LP.x, LP.y, LP.z));
	}
	if (key == 'l')//right
	{
		vec3 LP = spotLight->getPosition();
		spotLight->setPosition(vec3(++LP.x, LP.y, LP.z));
	}if (key == 'u')//right
	{
		vec3 LP = spotLight->getPosition();
		spotLight->setPosition(vec3(LP.x, ++LP.y, LP.z));
	}if (key == 'o')//right
	{
		vec3 LP = spotLight->getPosition();
		spotLight->setPosition(vec3(LP.x, --LP.y, LP.z));
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
	if (key == 'c')
	{
		if (sliding)
		{
			_playerTransform->setWorldPosition(vec3(_playerTransform->getWorldPosition().x,
				_playerTransform->getWorldPosition().y + 0.45f, _playerTransform->getWorldPosition().z));
			sliding = false;
			_playerTransform->setLocalScale(vec3(0.2f));
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
#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
#endif
}

TransformComponent * Scene::getMainCameraTransform() const
{
	return _mainCameraTransform;
}

void Scene::createTables(sqlite3 * db, char * errMsg)
{
	unsigned int exit;
	string sql;

	sql = "CREATE TABLE Entities ("\
		"ID              INT  PRIMARY KEY"\
								" NOT NULL,"\
		"Name            TEXT NOT NULL"\
								" DEFAULT ''"\
								" UNIQUE,"\
		"Transform       INT  REFERENCES Transforms(ID) ON DELETE SET NULL"\
								" NOT NULL ON CONFLICT ABORT,"\
		"[Mesh Renderer] INT  REFERENCES[Mesh Renderers](ID) ON DELETE SET NULL"\
								" DEFAULT NULL,"\
		"[Physics Body]  INT  REFERENCES[Physics Bodies](ID) ON DELETE SET NULL"\
								" DEFAULT NULL,"\
		"Camera          INT  REFERENCES Cameras(ID) ON DELETE SET NULL"\
								" DEFAULT NULL,"\
		"Collider        INT  REFERENCES Colliders(ID) ON DELETE SET NULL"\
								" DEFAULT NULL"\
		"); """;

	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not create Entity table!. " << errMsg << endl;
	else
		cout << "Successfully created Entity table." << endl;



	sql = "CREATE TABLE Transforms("\
		"ID         INT  PRIMARY KEY"\
						" NOT NULL,"\
		"Name       TEXT NOT NULL"\
						" DEFAULT ''"\
						" UNIQUE,"\
		"[Pos.X]    REAL DEFAULT(0.0)"\
						" NOT NULL,"\
		"[Pos.Y]    REAL DEFAULT(0.0)"\
						" NOT NULL,"\
		"[Pos.Z]    REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[Rot.X]    REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[Rot.Y]    REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[Rot.Z]    REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[OrbRot.X] REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[OrbRot.Y] REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[OrbRot.Z] REAL NOT NULL"\
						" DEFAULT(0.0),"\
		"[Scale.X]  REAL NOT NULL"\
						" DEFAULT(1.0),"\
		"[Scale.Y]  REAL NOT NULL"\
						" DEFAULT(1.0),"\
		"[Scale.Z]  REAL NOT NULL"\
						" DEFAULT(1.0),"\
		"Parent     TEXT DEFAULT ''"\
						" NOT NULL,"\
		"Children   TEXT DEFAULT ''"\
						" NOT NULL"\
		");";

	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);


	if (exit != SQLITE_OK)
		cout << "Could not create Transforms table!. " << errMsg << endl;
	else
		cout << "Successfully created Transforms table." << endl;


	sql = "CREATE TABLE Cameras("\
		"ID             INT     PRIMARY KEY"\
								" NOT NULL,"\
		"[Proj Type]    TEXT    NOT NULL"\
								" DEFAULT Perspective,"\
		"Cull           BOOLEAN NOT NULL"\
								" DEFAULT(FALSE),"\
		"[Ortho Size.L] REAL    NOT NULL,"\
		"[Ortho Size.R] REAL    NOT NULL,"\
		"[Ortho Size.T] REAL    NOT NULL,"\
		"[Ortho Size.B] REAL    NOT NULL,"\
		"[FOV.X]        REAL    NOT NULL,"\
		"[FOV.Y]        REAL    NOT NULL,"\
		"[Aspect Ratio] REAL    NOT NULL,"\
		"Near           REAL    NOT NULL,"\
		"Far            REAL    NOT NULL"\
		");";

	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);


	if (exit != SQLITE_OK)
		cout << "Could not create Cameras table!. " << errMsg << endl;
	else
		cout << "Successfully created Cameras table." << endl;



	sql = "CREATE TABLE[Mesh Renderers]("\
		"ID               INT     PRIMARY KEY"\
									" NOT NULL,"\
		"Mesh             TEXT    NOT NULL,"\
		"[Shader Program] TEXT    NOT NULL,"\
		"Transparent      BOOLEAN NOT NULL"\
									" DEFAULT(FALSE),"\
		"Textures         TEXT    NOT NULL"\
		");";

	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);


	if (exit != SQLITE_OK)
		cout << "Could not create Mesh Renderers table!. " << errMsg << endl;
	else
		cout << "Successfully created Mesh Renderers table." << endl;



	sql = "CREATE TABLE[Physics Bodies]("\
		"ID               INT     PRIMARY KEY"\
									" NOT NULL,"\
		"Mass             REAL    NOT NULL"\
									" DEFAULT(1.0),"\
		"[Use Gravity]    BOOLEAN NOT NULL"\
									" DEFAULT(FALSE),"\
		"[Can Jump]       BOOLEAN NOT NULL"\
									" DEFAULT(FALSE),"\
		"[Velocity.X]     REAL    NOT NULL"\
									" DEFAULT(0.0),"\
		"[Velocity.Y]     REAL    NOT NULL"\
									" DEFAULT(0.0),"\
		"[Velocity.Z]     REAL    NOT NULL"\
									" DEFAULT(0.0),"\
		"[Max Velocity.X] REAL    NOT NULL"\
									" DEFAULT(5.0),"\
		"[Max Velocity.Y] REAL    NOT NULL"\
									" DEFAULT(8.0),"\
		"[Max Velocity.Z] REAL    NOT NULL"\
									" DEFAULT(0.0)"\
		");";

	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);


	if (exit != SQLITE_OK)
		cout << "Could not create Physics Bodies table!. " << errMsg << endl;
	else
		cout << "Successfully created Physics Bodies table." << endl;



	sql = "CREATE TABLE Colliders("\
		"ID          INT     PRIMARY KEY"\
								" NOT NULL,"\
		"[Centre.X]  REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Centre.Y]  REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Centre.Z]  REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Size.X]    REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Size.Y]    REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Size.Z]    REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Offset.X]  REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Offset.Y]  REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"[Offset.Z]  REAL    NOT NULL"\
								" DEFAULT(0.0),"\
		"Enabled     BOOLEAN NOT NULL"\
								" DEFAULT(TRUE),"\
		"PhysicsBody INT     REFERENCES[Physics Bodies](ID) ON DELETE SET NULL"\
								" DEFAULT NULL,"\
		"Tag         TEXT    NOT NULL"\
								" DEFAULT 'Platform'"\
		");";

	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);


	if (exit != SQLITE_OK)
		cout << "Could not create Colliders table!. " << errMsg << endl;
	else
		cout << "Successfully created Colliders table." << endl;
}

void Scene::errorCheck(char* success, char* failure, char * errMsg)
{
	//if (exit != SQLITE_OK)
	//	cout << "Failed to select rows from Entities! " << errMsg << endl;
	//else
	//	cout << "Successfully selected rows from Entities." << endl;
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
		vec3 position = transform->getLocalPosition();
		vec3 rotation = transform->getLocalRotation();
		vec3 orbRot = transform->getOrbitRotation();
		vec3 scale = transform->getLocalScale();
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

void Scene::saveCameras(sqlite3 * db, char * errMsg)
{
	int exit = 0;
	vector<CameraComponent*> cameras = _entityManager->getAllCameras();

	// Clear old save data.
	string sql = "DELETE FROM Cameras;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not delete Cameras!. " << errMsg << endl;
	else
		cout << "Successfully deleted Cameras." << endl;

	sql = "VACUUM;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not Vacuum!. " << errMsg << endl;
	else
		cout << "Successfully Vacuumed." << endl;


	// Save new data.
	for (CameraComponent* camera : cameras)
	{
		string projType = GUIHelper::projToChar(camera->getProjType());
		bool cull = camera->getCullingActive();
		vec4 orthoSize = camera->getOrthoSize();
		vec2 fov = camera->getFov();
		float aspectRatio = camera->getAspectRatio();
		float camNear = camera->getNear();
		float camFar = camera->getFar();


		sql = "INSERT INTO Cameras (ID, [Proj Type], Cull, [Ortho Size.L], [Ortho Size.R], "
			"[Ortho Size.T], [Ortho Size.B], [FOV.X], [FOV.Y], [Aspect Ratio], Near, Far) VALUES (";

		sql += to_string(camera->getEntity()->getEid()) + ", ";
		sql += "'" + projType + "'" + ", ";
		sql += to_string(cull) + ", ";
		sql += to_string(orthoSize.x) + ", " + to_string(orthoSize.y) + ", " + to_string(orthoSize.z) + ", " + to_string(orthoSize.z) + ", ";
		sql += to_string(fov.x) + ", " + to_string(fov.y) + ", ";
		sql += to_string(aspectRatio) + ", ";
		sql += to_string(camNear) + ", ";
		sql += to_string(camFar) + ");";


		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

		if (exit != SQLITE_OK)
			cout << "Could not insert element into Cameras!. " << errMsg << endl;
		else
			cout << "Successfully inserted element into Cameras." << endl;
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
				sql += texture->getName() + ">";
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
		vec3 velocity = physicsBody->getVelocity();
		vec3 maxVelocity = physicsBody->getMaxVelocity();


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

void Scene::saveColliders(sqlite3 * db, char * errMsg)
{
	int exit = 0;
	vector<Collider*> colliders = _entityManager->getAllColliders();

	// Clear old save data.
	string sql = "DELETE FROM Colliders;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not delete Colliders!. " << errMsg << endl;
	else
		cout << "Successfully deleted Colliders." << endl;

	sql = "VACUUM;";
	exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

	if (exit != SQLITE_OK)
		cout << "Could not Vacuum!. " << errMsg << endl;
	else
		cout << "Successfully Vacuumed." << endl;


	// Save new data.
	for (Collider* collider : colliders)
	{
		vec3 centre = collider->getBounds()->centre;
		vec3 size = collider->getBounds()->size;
		vec3 offset = collider->getOffset();
		bool enabled = collider->getEnabled();
		unsigned int eid = collider->getEntity()->getEid();
		string tag = GUIHelper::tagToChar(collider->getTag());


		sql = "INSERT INTO Colliders (ID, [Centre.X], [Centre.Y], [Centre.Z], [Size.X], [Size.Y], "
			"[Size.Z], [Offset.X], [Offset.Y], [Offset.Z], Enabled, PhysicsBody, Tag) VALUES (";

		sql += to_string(collider->getEntity()->getEid()) + ", ";
		sql += to_string(centre.x) + ", " + to_string(centre.y) + ", " + to_string(centre.z) + ", ";
		sql += to_string(size.x) + ", " + to_string(size.y) + ", " + to_string(size.z) + ", ";
		sql += to_string(offset.x) + ", " + to_string(offset.y) + ", " + to_string(offset.z) + ", ";
		sql += to_string(enabled) + ", ";
		sql += to_string(eid) + ", ";
		sql += "'" + tag + "'" + ");";


		exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

		if (exit != SQLITE_OK)
			cout << "Could not insert element into Colliders!. " << errMsg << endl;
		else
			cout << "Successfully inserted element into Colliders." << endl;
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
		CameraComponent* camera = _entityManager->getComponent<CameraComponent*>(ComponentType::Camera, entity);
		Collider* collider = _entityManager->getComponent<Collider*>(ComponentType::Collider, entity);

		unsigned int eid = entity->getEid();

		// Insert all the information for the current entity.
		string name = transform->getName();


		sql = "INSERT INTO Entities (ID, Name, Transform, [Mesh Renderer], [Physics Body], Camera, Collider) VALUES (";

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

		if (camera)
			sql += ", " + to_string(eid);
		else
			sql += ", NULL";

		if (collider)
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

int Scene::loadEntityCallback(void * data, int numCols, char ** rowFields, char ** colNames)
{
	//EntityFactory* entityFactory = EntityFactory::getInstance();
	//Scene* scene = static_cast<Scene*>(data);
	EntityLoad load;
	int transformID;
	int meshRendererID;
	int physicsBodyID;
	int cameraID;
	int colliderID;

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
	if (rowFields[5])
	{
		sscanf(rowFields[5], "%i", &cameraID);
		load.cameraID = cameraID;
	}
	if (rowFields[6])
	{
		sscanf(rowFields[6], "%i", &colliderID);
		load.ColliderID = colliderID;
	}

	static_cast<vector<EntityLoad>*>(data)->push_back(load);

	//if (name == "Camera")
	//	entityFactory->createPerspectiveCamera();
	


	return 0;
}
int Scene::loadTransformCallback(void * data, int numRows, char ** rowFields, char ** colNames)
{
	//TransformComponent* transform = static_cast<TransformComponent*>(data);
	TransformComponent* transform = new TransformComponent();
	char* name = rowFields[1];
	vec3 pos = vec3(stof(rowFields[2]), stof(rowFields[3]), stof(rowFields[4]));
	vec3 rot = vec3(stof(rowFields[5]), stof(rowFields[6]), stof(rowFields[7]));
	vec3 orbRot = vec3(stof(rowFields[8]), stof(rowFields[9]), stof(rowFields[10]));
	vec3 scale = vec3(stof(rowFields[11]), stof(rowFields[12]), stof(rowFields[13]));
	//string parent = rowFields[14];
	//string children = rowFields[15];
	//vector<string> childrenNames;

	//string childName;
	//istringstream stream(children);

	//while (getline(stream, childName, ','))
	//{
	//	childrenNames.push_back(childName);
	//}


	transform->setName(name);
	transform->setLocalPosition(pos);
	transform->setLocalRotation(rot);
	transform->setOrbitRotation(orbRot);
	transform->setLocalScale(scale);

	TransformLoad* transformLoad = static_cast<TransformLoad*>(data);
	transformLoad->transform = transform;
	transformLoad->parentName = rowFields[14];

	//(*static_cast<unordered_map<string, TransformLoad>*>(data))[name] = transformLoad;

	return 0;
}
int Scene::loadCameraCallback(void * data, int numRows, char ** rowFields, char ** colNames)
{
	CameraComponent* camera = static_cast<CameraComponent*>(data);

	ProjectionType projType = GUIHelper::charToProj(rowFields[1]);
	bool cull = stoi(rowFields[2]);
	vec4 orthoSize = vec4(stof(rowFields[3]), stof(rowFields[4]), stof(rowFields[5]), stof(rowFields[6]));
	vec2 fov = vec2(stof(rowFields[7]), stof(rowFields[8]));
	float aspectRatio = stof(rowFields[9]);
	float camNear = stof(rowFields[10]);
	float camFar = stof(rowFields[11]);


	switch (projType)
	{
	case ProjectionType::Perspective:
		camera->setPerspective(fov.y, aspectRatio, camNear, camFar);
			break;
	case ProjectionType::Orthographic:
		camera->setOrthographic(orthoSize.x, orthoSize.y, orthoSize.z, orthoSize.w, camNear, camFar);
		break;
	default:
		break;
	}

	camera->setCullingActive(cull);

	return 0;
}
int Scene::loadMeshRendererCallback(void * data, int numRows, char ** rowFields, char ** colNames)
{
	MeshRendererComponent* meshRenderer = static_cast<MeshRendererComponent*>(data);

	meshRenderer->setMesh(ObjectLoader::getMesh(rowFields[1]));
	meshRenderer->setShaderProgram(ObjectLoader::getShaderProgram(rowFields[2]));
	meshRenderer->setIsTransparent(stoi(rowFields[3]));


	string textureData = rowFields[4];
	string textureName;
	istringstream stream(textureData);
	unsigned int index = 0;

	while (getline(stream, textureName, '>'))
	{
		meshRenderer->setTexture(index, ObjectLoader::getTexture(textureName));
		++index;
	}


	return 0;
}
int Scene::loadPhysicsBodyCallback(void * data, int numRows, char ** rowFields, char ** colNames)
{
	PhysicsBodyComponent* physicsBody = static_cast<PhysicsBodyComponent*>(data);

	physicsBody->setMass(stof(rowFields[1]));
	physicsBody->setUseGravity(stoi(rowFields[2]));
	physicsBody->setCanJump(stoi(rowFields[3]));

	vec3 vel = vec3(stof(rowFields[4]), stof(rowFields[5]), stof(rowFields[6]));
	vec3 maxVel = vec3(stof(rowFields[7]), stof(rowFields[8]), stof(rowFields[9]));

	physicsBody->setVelocity(vel);
	physicsBody->setMaxVelocity(maxVel);


	return 0;
}
int Scene::loadCollidersCallback(void * data, int numRows, char ** rowFields, char ** colNames)
{
	BoxCollider* collider = static_cast<BoxCollider*>(data);

	static vec3 centre;
	static vec3 size;
	static vec3 offset;
	static bool enabled;
	static Bounds bounds;
	static TTag tag;


	centre = vec3(stof(rowFields[1]), stof(rowFields[2]), stof(rowFields[3]));
	size = vec3(stof(rowFields[4]), stof(rowFields[5]), stof(rowFields[6]));
	offset = vec3(stof(rowFields[7]), stof(rowFields[8]), stof(rowFields[9]));
	enabled = stoi(rowFields[10]);
	tag = GUIHelper::charToTag(rowFields[12]);

	//collider = new BoxCollider(centre, size);
	bounds = Bounds(centre, size);
	collider->setBounds(bounds);
	collider->setOffset(offset);
	collider->setEnabled(enabled);
	collider->setTag(tag);


	return 0;
}
void Scene::loadEntities(sqlite3 * db, char * errMsg)
{
	string sql = "SELECT * FROM Entities;";
	int exit = 0;
	//vector<TransformLoad> transformLoads;
	unordered_map<string, TransformLoad> transformLoads;
	PhysicsBodyComponent* physicsBody = nullptr;


	exit = sqlite3_exec(db, sql.c_str(), loadEntityCallback, &_entityLoads, &errMsg);

	if (exit != SQLITE_OK)
	{
		cout << "Failed to select rows from Entities! " << errMsg << endl;
		return;
	}
	else
		cout << "Successfully selected rows from Entities." << endl;

	for (EntityLoad load : _entityLoads)
	{
		// Create a new entity.
		Entity* entity = _entityManager->createEntity();

		// Load in transform component.
		TransformLoad transformLoad;
		TransformComponent* transform = nullptr;
		sql = "SELECT * FROM Transforms WHERE ID = ";
		sql += to_string(load.transformID) + ";";


		exit = sqlite3_exec(db, sql.c_str(), loadTransformCallback, &transformLoad, &errMsg);

		if (exit != SQLITE_OK)
		{
			cout << "Failed to select rows from Transforms! " << errMsg << endl;
			return;
		}
		else
		{
			cout << "Successfully selected rows from Transforms." << endl;

			transform = transformLoad.transform;
			transformLoads[transform->getName()] = transformLoad;
			
			// Add component to entity.
			_entityManager->addComponent(transform, entity);

			if (strstr(transform->getName().c_str(), "Player"))
			{
				_playerTransform = transform;
			}
		}

		// Load in camera component.
		if (load.cameraID != -1)
		{
			CameraComponent* camera = new CameraComponent();
			sql = "SELECT * FROM Cameras WHERE ID = ";
			sql += to_string(load.cameraID) + ";";

			exit = sqlite3_exec(db, sql.c_str(), loadCameraCallback, camera, &errMsg);

			// Add component to entity.
			_entityManager->addComponent(camera, entity);
			_mainCamera = entity; // TO DO Only works with one camera.
			_mainCameraTransform = transform;
		}

		// Load in mesh renderer component.
		if (load.meshRendererID != -1)
		{
			MeshRendererComponent* meshRenderer = new MeshRendererComponent();
			sql = "SELECT * FROM [Mesh Renderers] WHERE ID = ";
			sql += to_string(load.meshRendererID) + ";";

			exit = sqlite3_exec(db, sql.c_str(), loadMeshRendererCallback, meshRenderer, &errMsg);

			// Add component to entity.
			_entityManager->addComponent(meshRenderer, entity);
		}

		// Load in physics body component.
		if (load.physicsBodyID != -1)
		{
			physicsBody = new PhysicsBodyComponent();
			sql = "SELECT * FROM [Physics Bodies] WHERE ID = ";
			sql += to_string(load.physicsBodyID) + ";";

			exit = sqlite3_exec(db, sql.c_str(), loadPhysicsBodyCallback, physicsBody, &errMsg);

			// Add component to entity.
			_entityManager->addComponent(physicsBody, entity);

			if (strstr(transform->getName().c_str(), "Player"))
			{
				_playerPhysicsBody = physicsBody;
			}
		}

		// Load in collider component.
		if (load.ColliderID != -1)
		{
			BoxCollider* collider = new BoxCollider(vec3::Zero, vec3::Zero);
			sql = "SELECT * FROM Colliders WHERE ID = ";
			sql += to_string(load.ColliderID) + ";";

			exit = sqlite3_exec(db, sql.c_str(), loadCollidersCallback, collider, &errMsg);

			// Re-assign physics body to this collider.
			if (physicsBody)
			{
				collider->setPhysicsBody(physicsBody);
				physicsBody = nullptr;
			}

			// Add component to entity.
			_entityManager->addComponent(collider, entity);
		}
	}


	// Re-create transform hierarchy.
	unordered_map<string, TransformLoad>::iterator it;
	for (it = transformLoads.begin(); it != transformLoads.end(); ++it)
	{
		TransformLoad load = it->second;
		if (load.parentName != "")
		{
			TransformComponent* parent = transformLoads[load.parentName].transform;
			load.transform->setParent(parent);
		}
	}
}
