#include "TransformSystem.h"

TransformSystem::TransformSystem(EntityManager * entityManager)
	: System(entityManager)
{
}

TransformSystem::~TransformSystem()
{
}

//bool rootSort(Entity* first, Entity* second)
//{
//	// Get the transform components for both entities.
//	Component* component = _entityManager->getComponent(ComponentType::Transform, first);
//	TransformComponent* firstTransform = dynamic_cast<TransformComponent*>(component);
//
//	//component = _entityManager->getComponent(ComponentType::Transform, second);
//	//TransformComponent* secondTransform = dynamic_cast<TransformComponent*>(component);
//
//	//// Make sure both components exist. Return false otherwise.
//	//if (!firstTransform || !secondTransform)
//	//	return false;
//
//	// Make sure transform component exists.
//	if (!firstTransform)
//		return false;
//
//	// Only change order if one of the transforms is not a root.
//	//if (firstTransform->isRoot() && !secondTransform->isRoot())
//	//	return true;
//	//else
//	//	return false;
//	return firstTransform->isRoot();
//}

void TransformSystem::update(float deltaTime)
{
	vector<Entity*> entities = _entityManager->getAllEntitiesWithComponent(ComponentType::Transform);

	// Sort the vector, so all the root transforms are at the back of the vector.
	// When we update the root transforms, forward kinematics will take into account their children's local transformations.
	//sort(entities.begin(), entities.end(), rootSort);
	EntityManager* entityManager = _entityManager;
	partition(entities.begin(), entities.end(), [entityManager](Entity* entity) -> bool
	{
		// Get the transform component.
		TransformComponent* transform = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);

		// Make sure transform component exists.
		if (!transform)
			return false;

		return !transform->isRoot();
	});


	for (Entity* entity : entities)
	{
		// Get the transform component for the current entity.
		//Component* component = _entityManager->getComponent(ComponentType::Transform, entity);
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);
		if (!transform)
			return;

		// Also check if the current entity possesses a physics body.
		//component = _entityManager->getComponent(ComponentType::PhysicsBody, entity);
		PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, entity);
		

		// Update the transforms position using physics, if the entity possesses a physics body.
		if (physicsBody)
		{
			Vector3 position = transform->getLocalPosition();
			physicsBody->update(deltaTime, transform);
			position = position + physicsBody->getVelocity() * deltaTime;
			transform->setLocalPosition(position);
		}

		// Only update the root transform of every entity.
		// Update the transforms position, using forward kinematics.
		if (transform->isRoot())
			transform->update(deltaTime);
	}


	// Update collider bounds, after current world transformations have been calculated.
	for (Entity* entity : entities)
	{
		// Get the transform component for the current entity.
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);
		if (!transform)
			return;

		// Also check if the current entity possesses a physics body.
		PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, entity);

		// Update the transforms position using physics, if the entity possesses a physics body.
		if (physicsBody)
		{
			physicsBody->updateBounds(transform);
		}
	}
}
