#include "UISystem.h"

UISystem::UISystem(EntityManager* entityManager)
{
	init(entityManager);
}

UISystem::~UISystem()
{
	for (auto const& canvas : _canvases)
	{
		delete canvas.second;
	}
}

void UISystem::update(float deltaTime)
{
	for (auto const& canvas : _canvases)
	{
		canvas.second->update(deltaTime);
	}
}

void UISystem::draw()
{
	mat4 camView = _camTrans->getLocalToWorldMatrix().getInverse();
	mat4 camProj = _camComp->getProjection();

	for (auto const& canvas : _canvases)
	{
		canvas.second->draw(camView, camProj);
	}
}

void UISystem::addCanvas(UICanvas * canvas)
{
	string name = canvas->getName();

	if (_canvases.find(name) != _canvases.end())
	{
		cout << "Canvas with name: " << name << "already exists!" << endl;
		system("pause");
	}

	_canvases[name] = canvas;
}

void UISystem::deleteCanvas(const string & name)
{
	_canvases.erase(name);
}

Entity * UISystem::getCamera() const
{
	return _camera;
}

void UISystem::checkClick(int x, int y)
{
	for (auto const& canvasKV : _canvases)
	{
		canvasKV.second->checkClick(x, y);
	}
}

unordered_map<string, UICanvas*> UISystem::getCanvases() const
{
	return _canvases;
}

UICanvas * UISystem::getCanvas(const string & name) const
{
	if (_canvases.find(name) != _canvases.end())
		return _canvases.at(name);
	return nullptr;
}

void UISystem::init(EntityManager * entityManager)
{
	// Create a new entity.
	Entity* entity = entityManager->createEntity();

	// Create transform component.
	TransformComponent* transform = new TransformComponent();
	transform->setLocalPosition(vec3::Zero);
	transform->setLocalScale(vec3::One);
	transform->setName("UICamera " + std::to_string(entity->getEid()));

	// Create camera component.
	CameraComponent* camera = new CameraComponent();
	camera->setOrthographic(0.0f, 1920.0f, 0.0f, 1080.5f, -10.0f, 10.0f);
	camera->setCullingActive(false);

	// Add the components to the entity.
	entityManager->addComponent(transform, entity);
	entityManager->addComponent(camera, entity);

	_camera = entity;
	_camTrans = transform;
	_camComp = camera;

	FontManager::init(transform, camera);
}
