#include "MeshRendererSystem.h"

MeshRendererSystem::MeshRendererSystem(EntityManager * entityManager)
	: System(entityManager)
{
}

MeshRendererSystem::~MeshRendererSystem()
{
}

void MeshRendererSystem::draw(Light* light, Light* spotLight)
{
	// Retrieve all entities, possessing a mesh renderer.
	vector<Entity*> entities = _entityManager->getAllEntitiesWithComponent(ComponentType::MeshRenderer);

	// Retrieve the main camera.
	Entity* mainCamera = EntityManager::getMainCamera();

	// If no main camera was set, draw nothing.
	if (!mainCamera)
		return;

	// Retrieve the necessary camera details.
	_cameraComp = _entityManager->getComponent<CameraComponent*>(ComponentType::Camera, mainCamera);
	_cameraTrans = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, mainCamera);
	Matrix44 cameraInverse = _cameraTrans->getLocalToWorldMatrix().GetInverse(_cameraTrans->getWorldRotation(), _cameraTrans->getWorldPosition());


	// Sort the vector, so all the transparent meshes are at the back of the vector.
	vector<Entity*>::iterator transIt = partition(entities.begin(), entities.end(), [](Entity* entity) -> bool
	{
		// Get the mesh renderer component.
		EntityManager* entityManager = EntityManager::getInstance();
		MeshRendererComponent* meshRenderer = entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, entity);

		// Make sure mesh renderer component exists.
		if (!meshRenderer)
			return false;

		return !meshRenderer->getIsTransparent();
	});

	// Cull both the opaque and transparent objects. Sort the transparent objects afterwards.
	_opaqueObjects.clear();
	_transObjects.clear();
	_opaqueObjects.assign(entities.begin(), transIt);
	_transObjects.assign(transIt, entities.end());
	cull(_opaqueCullList, _opaqueObjects);
	cull(_transCullList, _transObjects);
	sortMeshes(_transCullList);


	// Draw both the culled opaque and transparent objects.
	glDisable(GL_BLEND);
	drawHelper(_opaqueCullList, light, spotLight, cameraInverse);
	glEnable(GL_BLEND);
	drawHelper(_transCullList, light, spotLight, cameraInverse);
}

void MeshRendererSystem::cull(vector<Entity*>& cullList, vector<Entity*>& objectList)
{
	// Clear cullList from previous frame.
	cullList.clear();

	// Check if the camera is suppose to cull.
	if (_cameraComp->getCullingActive())
	{
		// The camera is set to perspective.
		if (_cameraComp->getProjType() == Perspective)
		{
			TransformComponent* transform = nullptr;

			for (Entity* entity : objectList)
			{
				transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);

				Vector3 direction = _cameraTrans->getLocalToWorldMatrix().getTranslation() - transform->getLocalToWorldMatrix().getTranslation();
				float distance = direction.Length();
				direction /= distance;
				Vector3 forward = _cameraTrans->getLocalToWorldMatrix().getForward();

				if (Vector3::dot(direction, forward) > cos(MathLibCore::toRadians(_cameraComp->getFov().x * 0.8f))
					&& (distance < (_cameraComp->getFar() * 1.4f)))
				{
					cullList.push_back(entity);
				}
			}
		}
		// The camera is set to orthographic.
		else
		{
			TransformComponent* transform = nullptr;

			for (Entity* entity : objectList)
			{
				transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);

				Vector3 direction = _cameraTrans->getLocalToWorldMatrix().getTranslation() - transform->getLocalToWorldMatrix().getTranslation() - Vector3(0.0f, 0.0f, -20.0f);
				float distance = direction.Length();
				direction /= distance;
				Vector3 forward = _cameraTrans->getLocalToWorldMatrix().getForward();
				forward.z -= 20.0f;
				if (Vector3::dot(direction, forward) > cos(MathLibCore::toRadians(_cameraComp->getFov().x * 0.5f))
					&& (distance < (_cameraComp->getFar() * 1.4f + 20.0f)))
				{
					cullList.push_back(entity);
				}
			}
		}
	}
	else
	{
		cullList = objectList;
	}
}

//bool perspectiveSort(Entity* a, Entity* b)
//{
//	EntityManager* entityManager = EntityManager::getInstance();
//	TransformComponent* aTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, a);
//	TransformComponent* bTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, b);
//
//	float distanceA = Vector3::distance(aTrans->getLocalToWorldMatrix().getTranslation(), camPos);
//	float distanceB = Vector3::distance(bTrans->getLocalToWorldMatrix().getTranslation(), camPos);
//
//	return distanceA < distanceB;
//}
//
//bool orthographicSort(Entity* a, Entity* b)
//{
//	EntityManager* entityManager = EntityManager::getInstance();
//	TransformComponent* aTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, a);
//	TransformComponent* bTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, b);
//
//	float distanceA = (camView * aTrans->getLocalToWorldMatrix()).getTranslation().z;
//	float distanceB = (camView * bTrans->getLocalToWorldMatrix()).getTranslation().z;
//	
//	return distanceA < distanceB;
//}

void MeshRendererSystem::sortMeshes(vector<Entity*>& cullList)
{
	//// Sort opaque objects.
	//if (!isTrans)
	//{
	//	Vector3 camPos = _cameraTrans->getWorldPosition();

	//	// Perspetive sort
	//	if (_cameraComp->getProjType() == Perspective)
	//		sort(cullList.begin(), cullList.end(), perspectiveSort);
	//	// Orthographic sort
	//	else
	//		Matrix44 camView = _cameraTrans->getLocalToWorldMatrix();

	//		sort(cullList.begin(), cullList.end(), orthographicSort);
	//}
	//// Sort transparent objects.
	//else
	//{

	//}


	// Sort transparent objects.
	// Perspetive sort.
	if (_cameraComp->getProjType() == Perspective)
	{
		Vector3 camPos = _cameraTrans->getWorldPosition();

		sort(cullList.begin(), cullList.end(), [camPos](Entity* a, Entity* b) -> bool
		{
			EntityManager* entityManager = EntityManager::getInstance();
			TransformComponent* aTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, a);
			TransformComponent* bTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, b);

			float sqrDistanceA = Vector3::sqrDistance(aTrans->getLocalToWorldMatrix().getTranslation(), camPos);
			float sqrDistanceB = Vector3::sqrDistance(bTrans->getLocalToWorldMatrix().getTranslation(), camPos);

			return sqrDistanceA > sqrDistanceB;
		});
	}
	// Orthographic sort.
	else
	{
		Matrix44 camView = _cameraTrans->getLocalToWorldMatrix();

		sort(cullList.begin(), cullList.end(), [camView](Entity* a, Entity* b) -> bool
		{
			EntityManager* entityManager = EntityManager::getInstance();
			TransformComponent* aTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, a);
			TransformComponent* bTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, b);

			float distanceA = (camView * aTrans->getLocalToWorldMatrix()).getTranslation().z;
			float distanceB = (camView * bTrans->getLocalToWorldMatrix()).getTranslation().z;

			return distanceA > distanceB;
		});
	}


}

void MeshRendererSystem::drawHelper(const vector<Entity*>& drawList, Light* light, Light* spotLight, Matrix44& cameraInverse)
{
	for (Entity* entity : drawList)
	{
		// Get the transform and mesh renderer components for the current entity. Only draw if both components exist.
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);
		if (!transform)
			return;

		MeshRendererComponent* meshRenderer = _entityManager->getComponent<MeshRendererComponent*>(ComponentType::MeshRenderer, entity);
		if (!meshRenderer)
			return;

		// Retrieve the mesh, shader program, and textures (if any).
		// Will not draw if the render component does not have a mesh or a sahder program.
		Mesh* mesh = meshRenderer->getMesh();
		ShaderProgram* shaderProgram = meshRenderer->getShaderProgram();
		vector<Texture*> textures = meshRenderer->getTextures();
		if (!mesh || !shaderProgram)
			return;


		// Both components exist, so draw the mesh.
		// Bind and send all the uniforms to the shader program.
		shaderProgram->bind();
		shaderProgram->sendUniformMat4("uModel", transform->getLocalToWorldMatrix().mV, false);
		shaderProgram->sendUniformMat4("uView", cameraInverse.mV, false);
		shaderProgram->sendUniformMat4("uProj", _cameraComp->getProjection().mV, false);

		shaderProgram->sendUniform("uTex", 0);

		shaderProgram->sendUniform("lightPosition", cameraInverse * Vector4(light->getPosition(), 1.0f));
		shaderProgram->sendUniform("lightAmbient", light->getAmbient());
		shaderProgram->sendUniform("lightDiffuse", light->getDiffuse());
		shaderProgram->sendUniform("lightSpecular", light->getSpecular());
		shaderProgram->sendUniform("lightSpecularExponent", light->getSpecularExp());
		shaderProgram->sendUniform("attenuationConstant", light->getAttenuationConstant());
		shaderProgram->sendUniform("attenuationLinear", light->getAttenuationLinear());
		shaderProgram->sendUniform("attenuationQuadratic", light->getAttenuationQuadratic());

		shaderProgram->sendUniform("spotLightPosition", cameraInverse * Vector4(spotLight->getPosition(), 1.0f));
		shaderProgram->sendUniform("spotLightDirection", Vector3(1, 0, 0));
		shaderProgram->sendUniform("spotLightAmbient", spotLight->getAmbient());
		shaderProgram->sendUniform("spotLightDiffuse", spotLight->getDiffuse());
		shaderProgram->sendUniform("spotLightSpecular", spotLight->getSpecular());
		shaderProgram->sendUniform("spotLightSpecularExponent", spotLight->getSpecularExp());
		shaderProgram->sendUniform("spotLightattenuationConstant", spotLight->getAttenuationConstant());
		shaderProgram->sendUniform("spotLightattenuationLinear", spotLight->getAttenuationLinear());
		shaderProgram->sendUniform("spotLightattenuationQuadratic", spotLight->getAttenuationQuadratic());

		// Bind all the textures.
		vector<Texture*>::iterator it;
		unsigned int texLoc = 0;
		for (it = textures.begin(); it != textures.end(); it++)
		{
			(*it)->bind(texLoc);
			texLoc++;
		}

		// Bind the mesh.
		glBindVertexArray(mesh->VAO);

		// If the entity has an animation, play it.
		//if (isAnimated)
			//animation.animate(_shaderProgram, _physicsBody->getVelocity().x / 7.5f);

		glDrawArrays(GL_TRIANGLES, 0, mesh->getNumVertices());
		glBindVertexArray(GL_NONE);

		// Unbind all the textures.
		vector<Texture*>::reverse_iterator revIt;
		for (revIt = textures.rbegin(); revIt != textures.rend(); revIt++)
		{
			texLoc--;
			(*revIt)->unBind(texLoc);
		}

		// Unbind the shader program.
		shaderProgram->unBind();
	}
}
