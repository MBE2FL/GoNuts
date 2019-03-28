#include "GUIHelper.h"


GUIHelper* GUIHelper::_instance = nullptr;

GUIHelper * GUIHelper::getInstance()
{
	if (!_instance)
		_instance = new GUIHelper();

	return _instance;
}

GUIHelper::~GUIHelper()
{
	delete _instance;
	_instance = nullptr;
}

void GUIHelper::init(float width, float height)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = width;
	io.DisplaySize.y = height;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontFromFileTTF(".\\Assets\\Fonts\\Roboto-Medium.ttf", 15);
	io.Fonts->Build();

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplOpenGL3_Init();
}

GUIHelper::GUIHelper()
{
	_entityManager = EntityManager::getInstance();
	_entityFactory = EntityFactory::getInstance();
	_sceneManager = SceneManager::getInstance();
}

void GUIHelper::draw()
{
	drawMenuBar();
}

void GUIHelper::drawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		// Scene menu options.
		if (ImGui::BeginMenu("Scenes"))
		{
			// Load scene from scene manager.
			if (ImGui::MenuItem("Load Scene"))
			{
				_showSceneSelector = !_showSceneSelector;
			}

			// Save current scene.
			if (ImGui::MenuItem("Save Scene"))
			{
				SceneManager::getInstance()->getCurrentScene()->saveScene();
			}

			// Save a scene as a new scene.
			if (ImGui::MenuItem("Save Scene As"))
			{
				_showSceneSaveModal = !_showSceneSaveModal;
			}

			// Open the scene editor.
			if (ImGui::MenuItem("Scene Editor"))
			{
				_showSceneEditor = !_showSceneEditor;
			}

			ImGui::EndMenu();
		}

		// Light menu options.
		if (ImGui::BeginMenu("Lights"))
		{
			if (ImGui::MenuItem("Settings"))
			{
				_showLightsMenu = !_showLightsMenu;
			}

			ImGui::EndMenu();
		}

		// UI menu options.
		if (ImGui::BeginMenu("UI"))
		{
			if (ImGui::MenuItem("UI Editor"))
			{
				_showUIEditor = !_showUIEditor;
			}

			if (ImGui::MenuItem("UI Animation Editor"))
			{
				_showUIAnimationEditor = !_showUIAnimationEditor;
			}

			ImGui::EndMenu();
		}

		ImGui::Text("				%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::EndMainMenuBar();
	}



	if (_showSceneSelector)
		drawScenes();

	// Name the new scene.
	if (_showSceneSaveModal)
	{
		ImGui::OpenPopup("Scene Name?");
		_showSceneSaveModal = false;
	}
	if (ImGui::BeginPopupModal("Scene Name?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Give the new scene a name.\n");
		ImGui::Separator();
		static char buffer[64] = "";
		ImGui::InputText("Enter Name Here", buffer, 64);

		if (ImGui::Button("Save"))
		{
			_sceneManager->saveSceneAs(buffer);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}


	if (_showLightsMenu)
		drawLights();

	if (_showSceneEditor)
		drawSceneEditor();

	if (_showUIEditor)
		drawUIEditor();	

	if (_showUIAnimationEditor)
		drawUIAnimationEditor();
}

void GUIHelper::update()
{
	_entityManager = EntityManager::getInstance();
	_entityFactory = EntityFactory::getInstance();
	_uiSystem = _sceneManager->getCurrentScene()->getUISystem();
}

bool GUIHelper::getPhysicsDebugEnabled() const
{
	return _enablePhysicsDebug;
}

void GUIHelper::drawHierarchy()
{
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3.0f);

	//ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	// Get all entities in the scene.
	//vector<Entity*> entities = _entityManager->getAllEntitiesWithComponent(ComponentType::Transform);
	vector<TransformComponent*> transforms = _entityManager->getAllTransforms();

	// Sort the vector, so all the root transform are in the front of the vector.
	// Erase all the non-root transforms from the vector.
	EntityManager* entityManager = _entityManager;
	vector<TransformComponent*>::iterator it;
	it = partition(transforms.begin(), transforms.end(), [entityManager](TransformComponent* transform) -> bool
	{
		// Make sure transform component exists.
		if (!transform)
			return false;

		return transform->isRoot();
	});

	transforms.erase(it, transforms.end());

	// Display all the root transforms.
	for (TransformComponent* transform : transforms)
	{
		transform->getName();

		drawHierarchyHelper(transform);
	}

	// Open property editor for the entity with the current transform component.
	if (_showPropertyEditor)
		propertyEditor(_currentTransform, &_showPropertyEditor);

	ImGui::PopStyleVar();
}

void GUIHelper::drawHierarchyHelper(TransformComponent * transform)
{
	vector<TransformComponent*> children = transform->getChildren();

	//if (children.size() > 0)
	//{
		if (ImGui::TreeNode(transform->getName().c_str()))
		{
			if (ImGui::SmallButton("Edit"))
			{
				_showPropertyEditor = true;
				_currentTransform = transform;
			}
			if (ImGui::SmallButton("Delete"))
			{
				for (TransformComponent* child : children)
				{
					child->setParent(nullptr);
				}

				if (transform->getParent())
					transform->getParent()->removeChild(transform);
				
				_entityManager->deleteEntity(transform->getEntity());
				ImGui::TreePop();
				return;
			}

			for (TransformComponent* child : children)
				drawHierarchyHelper(child);

			ImGui::TreePop();
		}
	//}
	//else
	//{
	//	ImGui::Text(transform->getName().c_str());

	//	ImGui::SameLine(100.0f, 100.0f);

	//	if (ImGui::SmallButton("Edit"))
	//	{
	//		_showPropertyEditor = true;
	//		_currentTransform = transform;
	//	}
	//}

	//if (ImGui::TreeNode(transform->getName().c_str()))
	//{
	//	if (ImGui::SmallButton("Edit"))
	//	{
	//		_showPropertyEditor = true;
	//		//propertyEditor(transform, &showPropertyEditor);
	//		_currentTransform = transform;
	//	}

	//	vector<TransformComponent*> children = transform->getChildren();
	//	for (TransformComponent* child : children)
	//		drawHierarchyHelper(child);

	//	ImGui::TreePop();
	//}
}

void GUIHelper::propertyEditor(TransformComponent * transform, bool * open)
{
	if (!ImGui::Begin("Property Editor", open) || !_currentTransform)
	{
		ImGui::End();
		return;
	}

	Entity* entity = transform->getEntity();

	// Display transform properties
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Transform:");
		// Position property
		vec3 temp = transform->getLocalPosition();
		ImGui::DragFloat3("Position: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalPosition(temp);

		// Rotation property
		temp = transform->getLocalRotation();
		ImGui::DragFloat3("Spin Rotation: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalRotation(temp);

		// Scale property
		temp = transform->getLocalScale();
		ImGui::DragFloat3("Scale: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalScale(temp);

		// Orbit property
		temp = transform->getOrbitRotation();
		ImGui::DragFloat3("Orbit Rotation: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setOrbitRotation(temp);
	}


	// Display collider properties
	Collider* collider = _entityManager->getComponent<Collider*>(ComponentType::Collider, entity);
	if (collider && ImGui::CollapsingHeader("Collider"))
		drawCollider(collider);

	// Display mesh renderer properties
	MeshRendererComponent* meshRenderer = _entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, entity);
	if (meshRenderer && ImGui::CollapsingHeader("Mesh Renderer"))
		drawMeshRenderer(meshRenderer, collider);

	// Display physics body properties
	PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, entity);
	if (physicsBody && ImGui::CollapsingHeader("Physics Body"))
		drawPhysicsBody(physicsBody);

	// Display camera properties
	CameraComponent* camera = _entityManager->getComponent<CameraComponent*>(ComponentType::Camera, entity);
	if (camera && ImGui::CollapsingHeader("Camera"))
		drawCamera(camera);


	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	// Add a component


	ImGui::End();
}

void GUIHelper::drawMeshRenderer(MeshRendererComponent * meshRenderer, Collider * collider)
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	// Transparency
	ImGui::Text("Mesh Renderer:");
	bool isTrans = meshRenderer->getIsTransparent();
	ImGui::Checkbox("Transparent", &isTrans);
	meshRenderer->setIsTransparent(isTrans);

	ImGui::Spacing();
	ImGui::Spacing();

	// Textures
	vector<Texture*> textures = ObjectLoader::getTextures();
	static string currentTexName = "";
	static Texture* texture = nullptr;
	if (ImGui::BeginCombo("Textures", currentTexName.c_str()))
	{
		vector<Texture*>::iterator it;
		for (it = textures.begin(); it != textures.end(); it++)
		{
			string texName = (*it)->filename;

			bool isSelected = (currentTexName == texName);

			if (ImGui::Selectable(texName.c_str(), isSelected))
			{
				currentTexName = texName;
				//meshRenderer->setTexture(0, *it);
				texture = *it;
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	// Add a texture
	if (ImGui::Button("Add Texture") && texture)
	{
		meshRenderer->addTexture(texture);

		currentTexName = "";
		texture = nullptr;
	}

	// Select a texture
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild("Textures", ImVec2(0.0f, 90.0f), true, windowFlags);

	textures = meshRenderer->getTextures();
	static unsigned int texIndex = 0;
	static bool texSelected = false;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		string buffer = textures[i]->filename;
		ImGui::PushID(i);
		if (ImGui::Selectable(buffer.c_str(), texIndex == i))
		{
			texIndex = i;
			texSelected = true;
		}
		ImGui::PopID();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Remove a texture
	if (ImGui::Button("Remove Texture") && texSelected)
	{
		meshRenderer->removeTexture(texIndex);
		texSelected = false;
	}


	ImGui::Spacing();
	ImGui::Spacing();

	// Meshes
	// Select a mesh
	vector<Mesh*> meshes = ObjectLoader::getMeshes();
	meshes.push_back(ObjectLoader::getSkeletalMesh("SkeletalBoiTwo"));
	string currentMesh = meshRenderer->getMesh()->getFilename();
	if (ImGui::BeginCombo("Meshes", currentMesh.c_str()))
	{
		vector<Mesh*>::iterator it;
		for (it = meshes.begin(); it != meshes.end(); it++)
		{
			//const char* texture = (*it)->filename.c_str();
			string mesh = (*it)->getFilename();

			bool isSelected = (currentMesh == mesh);

			if (ImGui::Selectable(mesh.c_str(), isSelected))
			{
				currentMesh = mesh;
				meshRenderer->setMesh(*it);

				if (collider)
					collider->setBounds((*it)->getMeshBounds());
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Shaders
	vector<ShaderProgram*> shaderPrograms = ObjectLoader::getShaderPrograms();
	string currentShaderProg = meshRenderer->getShaderProgram()->getProgramName();
	if (ImGui::BeginCombo("Shader Programs", currentShaderProg.c_str()))
	{
		vector<ShaderProgram*>::iterator it;
		for (it = shaderPrograms.begin(); it != shaderPrograms.end(); it++)
		{
			string shaderProgName = (*it)->getProgramName();

			bool isSelected = (currentShaderProg == shaderProgName);

			if (ImGui::Selectable(shaderProgName.c_str(), isSelected))
			{
				currentShaderProg = shaderProgName;
				meshRenderer->setShaderProgram(*it);
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	// Create a new shader program.
	if (ImGui::Button("Create Shader Program"))
		_showShaderProgCreator = true;

	if (_showShaderProgCreator)
	{
		if (!ImGui::Begin("Shader Program Creator", &_showShaderProgCreator))
		{
			ImGui::End();
		}
		else
		{
			// Select a vertex shader.
			static string currentVertShader = "";
			if (ImGui::BeginCombo("Vertex Shaders", currentVertShader.c_str()))
			{
				vector<ShaderProgram*>::iterator it;
				for (it = shaderPrograms.begin(); it != shaderPrograms.end(); it++)
				{
					//const char* texture = (*it)->filename.c_str();
					string shader = (*it)->getVertFilename();

					bool isSelected = (currentVertShader == shader);

					if (ImGui::Selectable(shader.c_str(), isSelected))
					{
						currentVertShader = shader;
						//meshRenderer->setShaderProgram(*it);
						//shaderProgram->load(shader, shaderProgram->getFragFilename());
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			// Select a fragment shader.
			static string currentFragShader = "";
			if (ImGui::BeginCombo("Fragment Shaders", currentFragShader.c_str()))
			{
				vector<ShaderProgram*>::iterator it;
				for (it = shaderPrograms.begin(); it != shaderPrograms.end(); it++)
				{
					//const char* texture = (*it)->filename.c_str();
					string shader = (*it)->getFragFilename();

					bool isSelected = (currentFragShader == shader);

					if (ImGui::Selectable(shader.c_str(), isSelected))
					{
						currentFragShader = shader;
						//meshRenderer->setShaderProgram(*it);
						//shaderProgram->load(shaderProgram->getVertFilename(), shader);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// Name the new shader program.
			if (ImGui::Button("Create Program"))
				ImGui::OpenPopup("Name?");
			if (ImGui::BeginPopupModal("Name?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Give the new program a name.\n");
				ImGui::Separator();
				static char buffer[64] = "";
				ImGui::InputText("Enter Name Here", buffer, 64);

				if (ImGui::Button("Create"))
				{
					ObjectLoader::loadShaderProgram(buffer, currentVertShader, currentFragShader);
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}
}

void GUIHelper::drawPhysicsBody(PhysicsBodyComponent * physicsBody)
{
	// Add a force to the physics body.
	vec3 tempVec3 = physicsBody->getForce();
	ImGui::InputFloat3("Force", &tempVec3.x, 3, ImGuiInputTextFlags_EnterReturnsTrue);
	physicsBody->setForce(tempVec3);

	// Set the acceleration.
	tempVec3 = physicsBody->getAcceleration();
	ImGui::InputFloat3("Acceleration", &tempVec3.x, 3, ImGuiInputTextFlags_EnterReturnsTrue);
	physicsBody->setAcceleration(tempVec3);

	// Set the velocity.
	tempVec3 = physicsBody->getVelocity();
	ImGui::DragFloat3("Velocity", &tempVec3.x);
	physicsBody->setVelocity(tempVec3);

	// Set the mass.
	float mass = physicsBody->getMass();
	ImGui::DragFloat("Mass", &mass);
	physicsBody->setMass(mass);

	// Set if the physics body should use gravity or not.
	bool useGravity = physicsBody->getUseGravity();
	ImGui::Checkbox("Use Gravity", &useGravity);
	physicsBody->setUseGravity(useGravity);
}

char* GUIHelper::projToChar(ProjectionType type)
{
	switch (type)
	{
	case Perspective:
		return "Perspective";
		break;
	case Orthographic:
		return "Orthographic";
		break;
	default:
		return "";
		break;
	}
}

ProjectionType GUIHelper::charToProj(char* type)
{
	if (strcmp(type, "Perspective") == 0)
		return ProjectionType::Perspective;
	else if (strcmp(type, "Orthographic") == 0)
		return ProjectionType::Orthographic;
	else
		return ProjectionType::Perspective;
}

void GUIHelper::drawCamera(CameraComponent * camera)
{
	bool cullingActive = camera->getCullingActive();
	ImGui::Checkbox("Cull", &cullingActive);
	camera->setCullingActive(cullingActive);

	float perspAspect = 1900.0f / 1000.0f;

	float aspect = camera->getAspectRatio();
	ProjectionType projType = camera->getProjType();
	char* currProjChar = projToChar(projType);
	if (ImGui::BeginCombo("Projection Type", currProjChar))
	{
		static const ProjectionType allTypes[2] = { ProjectionType::Perspective, ProjectionType::Orthographic };
		for (ProjectionType currType : allTypes)
		{
			char* projChar = projToChar(currType);

			bool isSelected = (currProjChar == projChar);

			if (ImGui::Selectable(projChar, isSelected))
			{
				currProjChar = projChar;
				if (charToProj(currProjChar) == ProjectionType::Perspective)
				{
					camera->setPerspective(60.0f, perspAspect, 1.0f, 1000.0f);
					projType = ProjectionType::Perspective;
				}
				else
				{
					camera->setOrthographic(-10.0f, 10.0f, 10.0f, -10.0f, 750.0f, -1000.0f);
					projType = ProjectionType::Orthographic;
				}
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}


	float zNear = camera->getNear();
	ImGui::DragFloat("zNear", &zNear, 1.0f, 0.0f, 0.0f, "%.1f");

	float zFar = camera->getFar();
	ImGui::DragFloat("zFar", &zFar, 1.0f, 0.0f, 0.0f, "%.1f");
	
	if (projType == ProjectionType::Perspective)
	{
		vec2 fov = camera->getFov();
		float fovY = toRadians(fov.y);
		ImGui::SliderAngle("FOV Y", &fovY);
		fov.y = toDegrees(fovY);
		
		string fovText = "FOV X " + to_string(fov.x);
		ImGui::Text(fovText.c_str());

		camera->setPerspective(fov.y, perspAspect, zNear, zFar);
	}
	else
	{
		vec4 orthoSize = camera->getOrthoSize();
		ImGui::DragFloat("Left", &orthoSize.x, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat("Right", &orthoSize.y, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat("Top", &orthoSize.z, 1.0f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat("Bottom", &orthoSize.w, 1.0f, 0.0f, 0.0f, "%.1f");

		camera->setOrthographic(orthoSize.x, orthoSize.y, orthoSize.w, orthoSize.z, zNear, zFar);
	}

	string aspectText = "Aspect " + to_string(aspect);
	ImGui::Text(aspectText.c_str());
}

char*GUIHelper::tagToChar(TTag tag)
{
	switch (tag)
	{
	case TTag::Player:
		return "Player";
		break;
	case TTag::Platform:
		return "Platform";
		break;
	case TTag::Enemy:
		return "Enemy";
		break;
	case TTag::Coin:
		return "Coin";
		break;
	case TTag::Acorn:
		return "Acorn";
		break;
	case TTag::Spike:
		return "Spike";
		break;
	case TTag::Checkpoint:
		return "Checkpoint";
		break;
	case TTag::VictoryPoint:
		return "VictoryPoint";
		break;
	case TTag::Generic:
		return "Generic";
		break;
	default:
		return "";
		break;
	}
}

TTag GUIHelper::charToTag(char* tag)
{
	if (strcmp(tag, "Player") == 0)
		return TTag::Player;
	else if (strcmp(tag, "Platform") == 0)
		return TTag::Platform;
	else if (strcmp(tag, "Enemy") == 0)
		return TTag::Enemy;
	else if (strcmp(tag, "Coin") == 0)
		return TTag::Coin;
	else if (strcmp(tag, "Acorn") == 0)
		return TTag::Acorn;
	else if (strcmp(tag, "Spike") == 0)
		return TTag::Spike;
	else if (strcmp(tag, "Checkpoint") == 0)
		return TTag::Checkpoint;
	else if (strcmp(tag, "Victorypoint") == 0)
		return TTag::VictoryPoint;
	else if (strcmp(tag, "Generic") == 0)
		return TTag::Generic;
	else
		return TTag::Acorn;
}

void GUIHelper::drawCollider(Collider * collider)
{
	// Set the tag.
	TTag tag = collider->getTag();
	char* currTagChar = tagToChar(tag);
	if (ImGui::BeginCombo("Tag", currTagChar))
	{
		static const TTag allTags[9] = { TTag::Player, TTag::Platform, TTag::Enemy, TTag::Coin, TTag::Acorn, TTag::Spike, TTag::Checkpoint
		, TTag::VictoryPoint, TTag::Generic};
		for (TTag currTag : allTags)
		{
			char* tagChar = tagToChar(currTag);

			bool isSelected = (currTagChar == tagChar);

			if (ImGui::Selectable(tagChar, isSelected))
			{
				currTagChar = tagChar;
				collider->setTag(charToTag(currTagChar));
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}

void GUIHelper::SpawnEntity()
{
	ImGui::OpenPopup("Spawn Entity");

	if (ImGui::BeginPopupModal("Spawn Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		// Create an input field for position.
		static vec3 position = vec3(0.0f, 0.0f, -5.0f);
		ImGui::InputFloat3("Position", &position.x, 2, ImGuiInputTextFlags_EnterReturnsTrue);

		// Create an input field for scale.
		static vec3 scale = vec3(1.0f);
		ImGui::InputFloat3("Scale", &scale.x, 2, ImGuiInputTextFlags_EnterReturnsTrue);


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Spawn Empty"))
			_entityFactory->createEmpty(position, scale, nullptr);
		else if (ImGui::Button("Spawn Acorn"))
			_entityFactory->createAcorn(position, scale, nullptr);
		else if (ImGui::Button("Spawn Coin"))
			_entityFactory->createCoin(position, scale, nullptr);
		else if (ImGui::Button("Spawn Platform"))
			_entityFactory->createPlatform(position, scale, nullptr);
		else if (ImGui::Button("Spawn Spike"))
			_entityFactory->createSpike(position, scale, nullptr);
		else if (ImGui::Button("Spawn Cone"))
			_entityFactory->createCone(position, scale, nullptr);
		else if (ImGui::Button("Spawn Vent"))
			_entityFactory->createVent(position, scale, nullptr);
			
		ImGui::Spacing();

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			_showSpawnEntity = false;
		}

		ImGui::EndPopup();
	}
}

void GUIHelper::drawScenes()
{
	ImGui::Begin("Scene Selector", &_showSceneSelector);

		vector<Scene*> scenes = _sceneManager->getScenes();

		// Select a scene from the scene manager.
		static Scene* _currentScene = _sceneManager->getCurrentScene();
		static string _currSceneName = _currentScene->getName();
		if (ImGui::BeginCombo("Scene Names", _currSceneName.c_str()))
		{
			for (Scene* scene : scenes)
			{
				string sceneName = scene->getName();

				bool isSelected = (_currSceneName == sceneName);

				if (ImGui::Selectable(sceneName.c_str(), isSelected))
				{
					_currSceneName = sceneName;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		// Load selected scene.
		if (ImGui::Button("Load Scene"))
		{
			_sceneManager->loadScene(_currSceneName);
		}

	ImGui::End();

}

void GUIHelper::drawSceneEditor()
{
	// Open scene editor window
	ImGui::Begin("Scene Editor", &_showSceneEditor);

	// Open spawn menu window
	if (ImGui::Button("Spawn Entity"))
	{
		_showSpawnEntity = true;
	}

	if (_showSpawnEntity)
		SpawnEntity();

	ImGui::Checkbox("Enable Physics Debug", &_enablePhysicsDebug);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Checkbox("Follow Player", &_sceneManager->getCurrentScene()->_followPlayer);
	// Display camera transform properties
	if (ImGui::CollapsingHeader("Main Camera Transform"))
	{
		TransformComponent* transform = _sceneManager->getCurrentScene()->getMainCameraTransform();
		// Position property
		vec3 temp = transform->getLocalPosition();
		ImGui::DragFloat3("Position: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalPosition(temp);

		// Rotation property
		temp = transform->getLocalRotation();
		ImGui::DragFloat3("Spin Rotation: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalRotation(temp);

		// Scale property
		temp = transform->getLocalScale();
		ImGui::DragFloat3("Scale: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalScale(temp);

		// Orbit property
		temp = transform->getOrbitRotation();
		ImGui::DragFloat3("Orbit Rotation: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setOrbitRotation(temp);
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Draw scene hierarchy
	if (ImGui::CollapsingHeader("Hierarchy:"))
	{
		drawHierarchy();
	}

	ImGui::End();
}

void GUIHelper::drawTransforms()
{
	//vector<TransformComponent*> transforms = _entityManager->getAllTransforms();

	//for (TransformComponent* transform : transforms)
	//{

	//}
}

void GUIHelper::drawLights()
{
	ImGui::Begin("Light Settings", &_showLightsMenu);

	// Light settings
//	Scene* _currentScene = _sceneManager->getCurrentScene();
//	Light* light = _currentScene->light;

	// Position settings
//	vec3 position = light->getPosition();
//	ImGui::DragFloat3("Light Position: ", &position.x, 0.5f);
//	light->setPosition(position);
//	// Ambient settings
//	vec3 ambient = light->getAmbient();
//	ImGui::ColorEdit3("Ambient Colour: ", &ambient.x);
//	light->setAmbient(ambient);
//	// Diffuse settings
//	vec3 diffuse = light->getDiffuse();
//	ImGui::ColorEdit3("Diffuse Colour: ", &diffuse.x);
//	light->setDiffuse(diffuse);
//	// Specular settings
//	vec3 specular = light->getSpecular();
//	ImGui::ColorEdit3("Specular Colour: ", &specular.x);
//	light->setSpecular(specular);
//	// Specular exponent settings
//	float specularExp = light->getSpecularExp();
//	ImGui::SliderFloat("Specular Exp: ", &specularExp, 0.0f, 250.0f);
//	light->setSpecularExp(specularExp);
//	// Attenuation constant settings
//	float attenuationConstant = light->getAttenuationConstant();
//	ImGui::SliderFloat("Attenuation Constant: ", &attenuationConstant, 0.0f, 20.0f);
//	light->setAttenuationConstant(attenuationConstant);
//	// Attenuation linear settings
//	float attenuationLinear = light->getAttenuationLinear();
//	ImGui::SliderFloat("Attenuation Linear: ", &attenuationLinear, 0.0f, 5.0f);
//	light->setAttenuationLinear(attenuationLinear);
//	// Attenuation quadratic settings
//	float attenuationQuadratic = light->getAttenuationQuadratic();
//	ImGui::SliderFloat("Attenuation Quadratic: ", &attenuationQuadratic, 0.0f, 5.0f);
//	light->setAttenuationQuadratic(attenuationQuadratic);
//
	ImGui::End();
}

void GUIHelper::drawUIEditor()
{
	// Open UI editor window
	ImGui::Begin("UI Editor", &_showUIEditor);

	// Open UI spawn menu window
	if (ImGui::Button("Spawn UI Element"))
	{
		_showSpawnUIElement = true;
	}

	// Open UI spawn canvas window
	if (ImGui::Button("Spawn UI Canvas"))
	{
		_showSpawnUICanvas = true;
	}

	if (_showSpawnUIElement)
		SpawnUIElement();

	if (_showSpawnUICanvas)
		SpawnUICanvas();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();


	drawCanvases();


	ImGui::End();
}

void GUIHelper::SpawnUIElement()
{
	// Open UI spawn window
	ImGui::Begin("UI Spawn", &_showSpawnUIElement);


	// Create an input field for name.
	static char name[64] = "";
	ImGui::InputText("Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue);

	// Create an input field for position.
	static vec3 position = vec3(0.0f, 2.0f, -5.0f);
	ImGui::InputFloat3("Position", &position.x, 2, ImGuiInputTextFlags_EnterReturnsTrue);

	// Create an input field for rotation.
	static float rot = 0.0f;
	ImGui::InputFloat("Z Rotation", &rot, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);

	// Create an input field for scale.
	static vec3 scale = vec3(1.0f);
	ImGui::InputFloat3("Scale", &scale.x, 2, ImGuiInputTextFlags_EnterReturnsTrue);

	// Create an input field for alpha.
	static float alpha = 1.0f;
	ImGui::InputFloat("Alpha", &alpha, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue);


	// Textures
	vector<Texture*> textures = ObjectLoader::getTextures();
	static string currentTexName = "";
	static Texture* texture = nullptr;
	if (ImGui::BeginCombo("Textures", currentTexName.c_str()))
	{
		vector<Texture*>::iterator it;
		for (it = textures.begin(); it != textures.end(); it++)
		{
			string texName = (*it)->filename;

			bool isSelected = (currentTexName == texName);

			if (ImGui::Selectable(texName.c_str(), isSelected))
			{
				currentTexName = texName;
				texture = *it;
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	// Canvases
	unordered_map<string, UICanvas*> canvases = _uiSystem->getCanvases();
	static string currentCanvasName = "";
	static UICanvas* canvas = nullptr;
	if (ImGui::BeginCombo("Canvases", currentCanvasName.c_str()))
	{
		for (auto const& canvasKV : canvases)
		{
			string canvasName = canvasKV.first;

			bool isSelected = (currentCanvasName == canvasName);

			if (ImGui::Selectable(canvasName.c_str(), isSelected))
			{
				currentCanvasName = canvasName;
				canvas = canvasKV.second;
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	// Spawn image
	if (ImGui::Button("Spawn UIImage") && texture && canvas)
	{
		UIImage* image = new UIImage(name, position, scale, vec3(0.0f, 0.0f, rot), alpha);
		image->setTexture(texture);

		canvas->addImage(image);


		currentTexName = "";
		texture = nullptr;

		currentCanvasName = "";
		canvas = nullptr;
	}


	ImGui::End();
}

void GUIHelper::SpawnUICanvas()
{
	// Open UI spawn window
	ImGui::Begin("UI Spawn Canvas", &_showSpawnUICanvas);


	// Create an input field for name.
	static char name[64] = "";
	ImGui::InputText("Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue);


	// Add new canvas to the current UI system
	if (ImGui::Button("Spawn Canvas"))
	{
		UICanvas* canvas = new UICanvas(name);

		_uiSystem->addCanvas(canvas);
	}


	ImGui::End();
}

void GUIHelper::drawCanvases()
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Canvases");
	ImGui::Spacing();

	// Draw each canvases' UI hierarchy
	unordered_map<string, UICanvas*> canvases = _uiSystem->getCanvases();

	for (auto const& canvasKV : canvases)
	{
		if (ImGui::CollapsingHeader(canvasKV.first.c_str()))
		{
			//_canvasShowImageProperty[canvasKV.second] = canvasKV.second;

			_currentUICanvas = canvasKV.second;
			drawUIHierarchy(canvasKV.second);
		}
	}

	ImGui::Separator();
}

void GUIHelper::drawUIHierarchy(UICanvas * canvas)
{
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3.0f);

	//ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	// Get all images in the canvas.
	unordered_map<string, UIImage*> imagesMap = canvas->getImages();
	vector<UIImage*> images;

	// Convert map to vector.
	for (auto const& imageKV : imagesMap)
	{
		images.push_back(imageKV.second);
	}

	// Sort the vector, so all the root transform are in the front of the vector.
	// Erase all the non-root transforms from the vector.
	vector<UIImage*>::iterator it;
	it = partition(images.begin(), images.end(), [](UIImage* image) -> bool
	{
		TransformComponent* transform = image->getTransform();

		// Make sure transform component exists.
		if (!transform)
			return false;

		return transform->isRoot();
	});

	images.erase(it, images.end());

	// Display all the root UIImages.
	for (UIImage* image : images)
	{
		drawUIHierarchyHelper(image, &_canvasShowImageProperty[canvas]);
	}

	// Open property editor for the current UIImage.
	//if (_showUIPropertyEditor)
	if (_canvasShowImageProperty[canvas])
		propertyUIEditor(canvas->getName() + " UI Property Editor", _currentUIImage, &_canvasShowImageProperty[canvas]);

	ImGui::PopStyleVar();
}

void GUIHelper::drawUIHierarchyHelper(UIImage * image, bool * showImageProperty)
{
	vector<UIImage*> children = image->getChildren();

	if (ImGui::TreeNode(image->getName().c_str()))
	{
		// Edit this image
		if (ImGui::SmallButton("Edit"))
		{
			//_showUIPropertyEditor = true;
			*showImageProperty = true;
			_currentUIImage = image;
		}
		// Delete this image
		if (ImGui::SmallButton("Delete"))
		{
			// Remove this image from all of it's children
			for (UIImage* child : children)
			{
				child->setParent(nullptr);
			}

			// Remove this image from it's parent
			if (image->getParent())
				image->getParent()->removeChild(image);

			// Remove the image from it's canvas
			_currentUICanvas->deleteImage(image->getName());
			ImGui::TreePop();
			return;
		}

		// Recurse through all of this image's children
		for (UIImage* child : children)
			drawUIHierarchyHelper(child, showImageProperty);

		ImGui::TreePop();
	}
}

void GUIHelper::propertyUIEditor(const string & canvasName, UIImage * image, bool * open)
{
	if (!ImGui::Begin(canvasName.c_str(), open) || !_currentUIImage)
	{
		ImGui::End();
		return;
	}

	TransformComponent* transform = image->getTransform();

	// Display transform properties
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Transform:");
		// Position property
		vec3 temp = transform->getLocalPosition();
		ImGui::DragFloat3("Position: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalPosition(temp);

		// Rotation property
		temp = transform->getLocalRotation();
		ImGui::DragFloat3("Spin Rotation: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalRotation(temp);

		// Scale property
		temp = transform->getLocalScale();
		ImGui::DragFloat3("Scale: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setLocalScale(temp);

		// Orbit property
		temp = transform->getOrbitRotation();
		ImGui::DragFloat3("Orbit Rotation: ", &temp.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		transform->setOrbitRotation(temp);
	}


	// Display animator properties
	if (ImGui::CollapsingHeader("UIAnimator"))
	{
		UIAnimator* animator = image->getAnimator();
		drawUIAnimator(animator);
	}


	ImGui::End();
}

void GUIHelper::drawUIAnimator(UIAnimator * animator)
{

	ImGui::Spacing();
	ImGui::Spacing();

	
	// Active property
	bool active = animator->getActive();
	ImGui::Checkbox("Active", &active);
	animator->setActive(active);

	// Current time property
	UIAnimation* currAnim = animator->getCurrentAnimation();
	if (currAnim)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Current Time: ");
		float temp = animator->getCurrentTime();
		ImGui::DragFloat("", &temp, 0.05f, 0.0f, currAnim->getDuration(), "%.3f", 1.0f);
		animator->setCurrentTime(temp);
	}


	ImGui::Spacing();
	ImGui::Spacing();

	// Display all the animations, which are loaded into this animator
	unordered_map<string, UIAnimation*> animations = UIAnimation::getAllAnimations();
	static string currentAnimName = "";
	static UIAnimation* anim = nullptr;

	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Add And Remove Animations");
	if (ImGui::BeginCombo("All Animations In System", currentAnimName.c_str()))
	{
		for (auto const& animKV : animations)
		{
			string animName = animKV.first;

			bool isSelected = (currentAnimName == animName);

			// Check if animName was selected this frame. Skip if it was already selected in a previous frame.
			if (ImGui::Selectable(animName.c_str(), isSelected))
			{
				currentAnimName = animName;
				anim = animKV.second;
			}
			// Highlight the currently selected animName
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	// Add an animation
	if (ImGui::Button("Add Animation") && anim)
	{
		animator->addAnimation(anim);

		currentAnimName = "";
		anim = nullptr;
	}

	// Select an animation
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild("Animations", ImVec2(0.0f, 90.0f), true, windowFlags);

	animations = animator->getAnimations();
	static string currAnimName = animations.size() > 0 ? animations.begin()->first : "";
	static UIAnimation* animSelected = nullptr;

	for (auto const& animKV : animations)
	{
		string animName = animKV.first;
		bool isSelected = (currAnimName == animName);

		if (ImGui::Selectable(animName.c_str(), isSelected))
		{
			currAnimName = animName;
			animSelected = animKV.second;
		}
		if (isSelected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Remove an animation
	if (ImGui::Button("Remove Animation") && animSelected)
	{
		animator->removeAnimation(animSelected->getName());
		animSelected = nullptr;
	}


	ImGui::Spacing();
	ImGui::Spacing();




	// Animation Order
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Animation Order");

	animations = animator->getAnimations();
	static string currentAnimOrderName = "";
	static UIAnimation* animInOrder = nullptr;

	if (ImGui::BeginCombo("All Animations In Animator", currentAnimOrderName.c_str()))
	{
		for (auto const& animKV : animations)
		{
			string animName = animKV.first;

			bool isSelected = (currentAnimOrderName == animName);

			// Check if animName was selected this frame. Skip if it was already selected in a previous frame.
			if (ImGui::Selectable(animName.c_str(), isSelected))
			{
				currentAnimOrderName = animName;
				animInOrder = animKV.second;
			}
			// Highlight the currently selected animName
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	// Push an animation to the stack
	if (ImGui::Button("Push Animation") && animInOrder)
	{
		animator->play(animInOrder->getName());

		currentAnimOrderName = "";
		animInOrder = nullptr;
	}

	// Display all animations in the stack
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	ImGui::BeginChild("Animation Order", ImVec2(0.0f, 90.0f), true, windowFlags);

	stack<UIAnimation*> animOrder = animator->getAnimOrder();

	// Print the names of all the animations in the animation order
	while (!animOrder.empty())
	{
		string animName = animOrder.top()->getName();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), animName.c_str());

		animOrder.pop();

	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Pop an animation
	if (ImGui::Button("Pop Animation"))
	{
		animator->stopCurrent();
	}

	// Pop all animations
	if (ImGui::Button("Pop All Animations"))
	{
		animator->stopAll();
	}
}

void GUIHelper::drawUIAnimationEditor()
{
	// Open UI editor window
	ImGui::Begin("UI Animation Editor", &_showUIAnimationEditor);

	// Display all the animations, which are loaded into this animator
	unordered_map<string, UIAnimation*> animations = UIAnimation::getAllAnimations();
	static string currentAnimName = "";
	static UIAnimation* anim = nullptr;
	static bool showUIAnimationPropertyEditor = false;

	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Edit Animations");

	for (auto const& animKV : animations)
	{
		if (ImGui::TreeNode(animKV.first.c_str()))
		{
			// Edit this image
			if (ImGui::SmallButton("Edit"))
			{
				showUIAnimationPropertyEditor = true;
				anim = animKV.second;
			}

			ImGui::TreePop();
		}
	}


	// If user selected an animation
	if (showUIAnimationPropertyEditor && anim)
	{
		propertyUIAnimationEditor(anim, &showUIAnimationPropertyEditor);
	}


	ImGui::End();
}

void GUIHelper::propertyUIAnimationEditor(UIAnimation * anim, bool * open)
{
	ImGui::Begin("UI Animation Property Editor", open);

	// Display duration
	ImGui::Text("Duration");
	float duration = anim->getDuration();
	ImGui::Text(to_string(duration).c_str());

	// Loop property
	bool loop = anim->getLoop();
	ImGui::Checkbox("Loop", &loop);
	anim->setLoop(loop);

	// Key Frames Property
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_HorizontalScrollbar || ImGuiWindowFlags_AlwaysAutoResize || ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

	vector<UIKeyFrame*> keyFrames = anim->getKeyFrames();
	static float currKeyTime = 0.0f;
	static UIKeyFrame* keyFrame = nullptr;

	ImGui::BeginChild("UI Key Frames", ImVec2(0.0f, 0.0f), true, windowFlags);

	// Select a key frame to edit
	vector<UIKeyFrame*>::iterator it;
	for (it = keyFrames.begin(); it != keyFrames.end(); ++it)
	{
		float keyTime = (*it)->getStartTime();

		bool isSelected = (currKeyTime == keyTime);

		// Check if animName was selected this frame. Skip if it was already selected in a previous frame.
		if (ImGui::Selectable(to_string(keyTime).c_str(), isSelected))
		{
			currKeyTime = keyTime;
			keyFrame = *it;
		}
		// Highlight the currently selected animName
		if (isSelected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	

	// If the user has a key frame selected
	if (keyFrame)
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

		ImGui::BeginChild("UI Key Frames", ImVec2(0.0f, 0.0f), true, windowFlags);

		// Position Property
		vec3 pos = keyFrame->getPos();
		ImGui::DragFloat3("Position", &pos.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		keyFrame->setPos(pos);

		// Scale Property
		vec3 scale = keyFrame->getScale();
		ImGui::DragFloat3("Scale", &scale.x, 0.2f, NULL, NULL, "%.1f", 1.0f);
		keyFrame->setscale(scale);


		ImGui::EndChild();
		ImGui::PopStyleVar();
	}



	ImGui::End();
}
