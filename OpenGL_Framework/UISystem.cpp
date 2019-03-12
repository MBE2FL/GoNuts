#include "UISystem.h"

UISystem::UISystem(EntityManager* entityManager)
{
	init(entityManager);
}

UISystem::~UISystem()
{
	for (auto const& canvas : canvases)
	{
		delete canvas.second;
	}
}

void UISystem::update(float deltaTime)
{
	for (auto const& canvas : canvases)
	{
		canvas.second->update(deltaTime);
	}
}

void UISystem::draw()
{
	mat4 camView = _camTrans->getLocalToWorldMatrix().getInverse();
	mat4 camProj = _camComp->getProjection();

	for (auto const& canvas : canvases)
	{
		canvas.second->draw(camView, camProj);
	}
}

void UISystem::addCanvas(const string & name, UICanvas * canvas)
{
	if (canvases.find(name) != canvases.end())
	{
		cout << "Canvas with name: " << name << "already exists!" << endl;
		system("pause");
	}

	canvases[name] = canvas;
}

void UISystem::deleteCanvas(const string & name)
{
	canvases.erase(name);
}

Entity * UISystem::getCamera() const
{
	return _camera;
}

void UISystem::checkClick(int x, int y)
{

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
	camera->setOrthographic(-8.0f, 8.0f, -4.5f, 4.5f, -10.0f, 100.0f);
	camera->setCullingActive(false);

	// Add the components to the entity.
	entityManager->addComponent(transform, entity);
	entityManager->addComponent(camera, entity);

	_camera = entity;
	_camTrans = transform;
	_camComp = camera;
}
