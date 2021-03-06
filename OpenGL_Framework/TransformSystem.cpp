#include "TransformSystem.h"

TransformSystem::TransformSystem(EntityManager * entityManager)
	: System(entityManager)
{
}

TransformSystem::~TransformSystem()
{
}

void TransformSystem::update(float deltaTime)
{
	vector<TransformComponent*> transforms = _entityManager->getAllTransforms();

	// Sort the vector, so all the root transforms are at the back of the vector.
	// When we update the root transforms, forward kinematics will take into account their children's local transformations.
	//sort(entities.begin(), entities.end(), rootSort);
	EntityManager* entityManager = _entityManager;
	partition(transforms.begin(), transforms.end(), [entityManager](TransformComponent* transform) -> bool
	{
		// Make sure transform component exists.
		if (!transform)
			return false;

		return !transform->isRoot();
	});


	for (TransformComponent* transform : transforms)
	{
		if (!transform)
			return;

		// Also check if the current entity possesses a physics body.
		PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, transform->getEntity());


		// Update the transforms position using physics, if the entity possesses a physics body.
		if (physicsBody)
		{
			vec3 position = transform->getLocalPosition();
			physicsBody->update(deltaTime, transform);
			position = position + physicsBody->getVelocity() * deltaTime;
			transform->setLocalPosition(position);
		}

		// Only update the root transform of every entity.
		// Update the transforms position, using forward kinematics.
		if (transform->isRoot())
			transform->update(deltaTime);
	}


	//vector<PhysicsBodyComponent*> physicsBodies = _entityManager->getAllPhysicsBodyComponents();
	//TransformComponent* playerTransform = EntityManager::getPlayerTransform();
	//if (!playerTransform)
	//	return;
	//// Update collider bounds, after current world transformations have been calculated.
	//for (PhysicsBodyComponent* physicsBody : physicsBodies)
	//{
	//	// Get the transform component for the current entity.
	//	TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, physicsBody->getEntity());
	//	if (!transform)
	//		return;

	//	// Also check if the current entity possesses a physics body.
	//	//PhysicsBodyComponent* physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, entity);

	//	// Update the transforms position using physics, if the entity possesses a physics body.
	//	if (physicsBody)
	//	{
	//		physicsBody->updateBounds(transform);

	//		bool active = (vec3::sqrDistance(playerTransform->getWorldPosition(), transform->getWorldPosition()) <= CULL_DISTANCE);
	//		physicsBody->setActive(active);
	//	}
	//}


	vector<Collider*> colliders = _entityManager->getAllColliders();
	TransformComponent* playerTransform = EntityManager::getPlayerTransform();
	if (!playerTransform)
		return;
	// Update colliders, after current world transformations have been calculated.
	for (Collider* collider : colliders)
	{
		// Get the transform component for the current entity.
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, collider->getEntity());
		if (!transform)
			return;

		// Update collider.
		collider->update(transform);


		bool active = (vec3::sqrDistance(playerTransform->getWorldPosition(), transform->getWorldPosition()) <= CULL_DISTANCE);

		collider->setEnabled(active);


		// Also check if the current entity possesses a physics body.
		PhysicsBodyComponent* physicsBody = collider->getPhysicsBody();

		// Update the transforms position using physics, if the entity possesses a physics body.
		if (physicsBody)
		{
			//bool active = (vec3::sqrDistance(playerTransform->getWorldPosition(), transform->getWorldPosition()) <= CULL_DISTANCE);
			physicsBody->setActive(active);
		}
	}
}
