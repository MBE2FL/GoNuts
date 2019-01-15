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
}

void GUIHelper::draw()
{
	ImGui::Begin("Scene Editor", &_showSceneEditor);

	ImGui::Checkbox("Scene Editor", &_showSceneEditor);

	if (_showSceneEditor)
	{
		ImGui::Begin("Scene Editor", &_showSceneEditor);

		// Draw scene hierarchy
		if (ImGui::CollapsingHeader("Hierarchy:"))
		{
			drawHierarchy();
		}

		ImGui::Separator();

		if (ImGui::Button("Spawn Entity"))
		{
			// TO-DO
		}

		if (ImGui::Button("Close"))
			_showSceneEditor = false;

		ImGui::End();
	}

	ImGui::End();
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

	if (_showPropertyEditor)
		propertyEditor(_currentTransform, &_showPropertyEditor);

	ImGui::PopStyleVar();
}

void GUIHelper::drawHierarchyHelper(TransformComponent * transform)
{
	if (ImGui::TreeNode(transform->getName().c_str()))
	{
		if (ImGui::SmallButton("Edit"))
		{
			_showPropertyEditor = true;
			//propertyEditor(transform, &showPropertyEditor);
			_currentTransform = transform;
		}

		vector<TransformComponent*> children = transform->getChildren();
		for (TransformComponent* child : children)
			drawHierarchyHelper(child);

		ImGui::TreePop();
	}
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
		Vector3 temp = transform->getLocalPosition();
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


	// Display mesh renderer properties
	MeshRendererComponent* meshRenderer = _entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, entity);
	if (meshRenderer && ImGui::CollapsingHeader("Mesh Renderer"))
		drawMeshRenderer(meshRenderer);

	// Display physics body properties
	PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, entity);
	if (physicsBody && ImGui::CollapsingHeader("Physics Body"))
		drawPhysicsBody(physicsBody);


	ImGui::End();
}

void GUIHelper::drawMeshRenderer(MeshRendererComponent * meshRenderer)
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
	//vector<string> textures = ObjectLoader::getTextureNames();
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

	if (ImGui::Button("Add Texture") && texture)
	{
		meshRenderer->addTexture(texture);

		currentTexName = "";
		texture = nullptr;
	}

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

	// remove
	if (ImGui::Button("Remove Texture") && texSelected)
	{
		meshRenderer->removeTexture(texIndex);
		texSelected = false;
	}


	ImGui::Spacing();
	ImGui::Spacing();

	// Meshes
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

char* tagToChar(TTag tag)
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
	default:
		return "";
		break;
	}
}

TTag charToTag(char* tag)
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
	else
		return TTag::Acorn;
}

void GUIHelper::drawPhysicsBody(PhysicsBodyComponent * physicsBody)
{
	Vector3 tempVec3 = physicsBody->getForce();
	ImGui::InputFloat3("Force", &tempVec3.x, 3, ImGuiInputTextFlags_EnterReturnsTrue);
	physicsBody->setForce(tempVec3);

	tempVec3 = physicsBody->getAcceleration();
	ImGui::InputFloat3("Acceleration", &tempVec3.x, 3, ImGuiInputTextFlags_EnterReturnsTrue);
	physicsBody->setAcceleration(tempVec3);

	tempVec3 = physicsBody->getVelocity();
	ImGui::DragFloat3("Velocity", &tempVec3.x);
	physicsBody->setVelocity(tempVec3);

	float mass = physicsBody->getMass();
	ImGui::DragFloat("Mass", &mass);
	physicsBody->setMass(mass);

	bool useGravity = physicsBody->getUseGravity();
	ImGui::Checkbox("Use Gravity", &useGravity);
	physicsBody->setUseGravity(useGravity);

	TTag tag = physicsBody->getTag();
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
				physicsBody->setTag(charToTag(currTagChar));
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
}
