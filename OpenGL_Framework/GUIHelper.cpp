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
}

void GUIHelper::update()
{
	_entityManager = EntityManager::getInstance();
	_entityFactory = EntityFactory::getInstance();
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
	vector<Entity*> entities = _entityManager->getAllEntitiesWithComponent(ComponentType::Transform);

	// Sort the vector, so all the root transform are in the front of the vector.
	// Erase all the non-root transforms from the vector.
	EntityManager* entityManager = _entityManager;
	vector<Entity*>::iterator it;
	it = partition(entities.begin(), entities.end(), [entityManager](Entity* entity) -> bool
	{
		// Get the transform component.
		TransformComponent* transform = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);

		// Make sure transform component exists.
		if (!transform)
			return false;

		return transform->isRoot();
	});

	entities.erase(it, entities.end());

	// Display all the root transforms.
	for (Entity* entity : entities)
	{
		TransformComponent* transform = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);
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

char* GUIHelper::tagToChar(TTag tag)
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
	case TTag::TopPlatform:
		return "Top Platform";
		break;
	default:
		cerr << "Tag to Char failed!" << endl;
		system("pause");
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
	else if (strcmp(tag, "Top Platform") == 0)
			return TTag::TopPlatform;
	else
	{
		cerr << "Char to Tag failed!" << endl;
		system("pause");
		return TTag::Acorn;
	}
}

void GUIHelper::drawCollider(Collider * collider)
{
	// Set the tag.
	TTag tag = collider->getTag();
	char* currTagChar = tagToChar(tag);
	if (ImGui::BeginCombo("Tag", currTagChar))
	{
		static const TTag allTags[6] = { TTag::Player, TTag::Platform, TTag::Enemy, TTag::Coin, TTag::Acorn, TTag::Spike };
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
	Scene* _currentScene = _sceneManager->getCurrentScene();
	Light* light = _currentScene->light;

	// Position settings
	vec3 position = light->getPosition();
	ImGui::DragFloat3("Light Position: ", &position.x, 0.5f);
	light->setPosition(position);
	// Ambient settings
	vec3 ambient = light->getAmbient();
	ImGui::ColorEdit3("Ambient Colour: ", &ambient.x);
	light->setAmbient(ambient);
	// Diffuse settings
	vec3 diffuse = light->getDiffuse();
	ImGui::ColorEdit3("Diffuse Colour: ", &diffuse.x);
	light->setDiffuse(diffuse);
	// Specular settings
	vec3 specular = light->getSpecular();
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

	ImGui::End();
}
