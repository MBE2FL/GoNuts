#include "MeshRendererSystem.h"

MeshRendererSystem::MeshRendererSystem(EntityManager * entityManager)
	: System(entityManager)
{
}

MeshRendererSystem::~MeshRendererSystem()
{
}

void MeshRendererSystem::draw(Light * light, Light * spotLight)
{
	// Retrieve all entities, possessing a mesh renderer.
	vector<MeshRendererComponent*> meshRenderers = _entityManager->getAllMeshRenderers();

	// Retrieve the main camera.
	Entity* mainCamera = EntityManager::getMainCamera();

	// If no main camera was set, draw nothing.
	if (!mainCamera)
		return;

	// Retrieve the necessary camera details.
	_cameraComp = _entityManager->getComponent<CameraComponent*>(ComponentType::Camera, mainCamera);
	_cameraTrans = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, mainCamera);
	mat4 cameraInverse = _cameraTrans->getLocalToWorldMatrix().getInverse(_cameraTrans->getWorldRotation(), _cameraTrans->getWorldPosition());


	// Sort the vector, so all the transparent meshes are at the back of the vector.
	vector<MeshRendererComponent*>::iterator transIt = partition(meshRenderers.begin(), meshRenderers.end(), [](MeshRendererComponent* meshRenderer) -> bool
	{
		// Make sure mesh renderer component exists.
		if (!meshRenderer)
			return false;

		return !meshRenderer->getIsTransparent();
	});

	// Cull both the opaque and transparent objects. Sort the transparent objects afterwards.
	_opaqueObjects.clear();
	_transObjects.clear();
	_opaqueObjects.assign(meshRenderers.begin(), transIt);
	_transObjects.assign(transIt, meshRenderers.end());
	cull(_opaqueCullList, _opaqueObjects);
	cull(_transCullList, _transObjects);
	sortMeshes(_transCullList);


	// Draw both the culled opaque and transparent objects.
	glDisable(GL_BLEND);
	drawHelper(_opaqueCullList, light, spotLight, cameraInverse);
	glEnable(GL_BLEND);
	drawHelper(_transCullList, light, spotLight, cameraInverse);
}

void MeshRendererSystem::cull(vector<MeshRendererComponent*>& cullList, vector<MeshRendererComponent*>& objectList)
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

			for (MeshRendererComponent* meshRenderer : objectList)
			{
				transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, meshRenderer->getEntity());

				vec3 direction = _cameraTrans->getTranslation() - transform->getTranslation();
				float distance = static_cast<float>(direction.length());
				direction /= distance;
				vec3 forward = _cameraTrans->getForward();

				if (vec3::dot(direction, forward) > cos(toRadians(_cameraComp->getFov().x * 0.8f))
					&& (distance < (_cameraComp->getFar() * 1.4f)))
				{
					cullList.push_back(meshRenderer);
				}
			}
		}
		// The camera is set to orthographic.
		else
		{
			TransformComponent* transform = nullptr;

			for (MeshRendererComponent* meshRenderer : objectList)
			{
				transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, meshRenderer->getEntity());

				vec3 direction = _cameraTrans->getTranslation() - transform->getTranslation() - vec3(0.0f, 0.0f, -20.0f);
				float distance = static_cast<float>(direction.length());
				direction /= distance;
				vec3 forward = _cameraTrans->getForward();
				forward.z -= 20.0f;
				if (vec3::dot(direction, forward) > cos(toRadians(_cameraComp->getFov().x * 0.5f))
					&& (distance < (_cameraComp->getFar() * 1.4f + 20.0f)))
				{
					cullList.push_back(meshRenderer);
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
//	float distanceA = vec3::distance(aTrans->getLocalToWorldMatrix().getTranslation(), camPos);
//	float distanceB = vec3::distance(bTrans->getLocalToWorldMatrix().getTranslation(), camPos);
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

void MeshRendererSystem::sortMeshes(vector<MeshRendererComponent*>& cullList)
{
	//// Sort opaque objects.
	//if (!isTrans)
	//{
	//	vec3 camPos = _cameraTrans->getWorldPosition();

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
		vec3 camPos = _cameraTrans->getWorldPosition();

		sort(cullList.begin(), cullList.end(), [camPos](MeshRendererComponent* a, MeshRendererComponent* b) -> bool
		{
			EntityManager* entityManager = EntityManager::getInstance();
			TransformComponent* aTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, a->getEntity());
			TransformComponent* bTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, b->getEntity());

			float sqrDistanceA = vec3::sqrDistance(aTrans->getTranslation(), camPos);
			float sqrDistanceB = vec3::sqrDistance(bTrans->getTranslation(), camPos);

			return sqrDistanceA > sqrDistanceB;
		});
	}
	// Orthographic sort.
	else
	{
		mat4 camView = _cameraTrans->getLocalToWorldMatrix();

		sort(cullList.begin(), cullList.end(), [camView](MeshRendererComponent* a, MeshRendererComponent* b) -> bool
		{
			EntityManager* entityManager = EntityManager::getInstance();
			TransformComponent* aTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, a->getEntity());
			TransformComponent* bTrans = entityManager->getComponent<TransformComponent*>(ComponentType::Transform, b->getEntity());

			float distanceA =(camView * aTrans->getLocalToWorldMatrix()).getTranslation().z;
			float distanceB = (camView * bTrans->getLocalToWorldMatrix()).getTranslation().z;

			return distanceA > distanceB;
		});
	}


}

void MeshRendererSystem::drawHelper(const vector<MeshRendererComponent*>& drawList, Light* light, Light* spotLight, mat4& cameraInverse)
{
	//vector<Entity*> lightEntities = _entityManager->getAllEntitiesWithComponent(ComponentType::Light);

	for (MeshRendererComponent* meshRenderer : drawList)
	{
		// Get the transform and mesh renderer components for the current entity. Only draw if both components exist.
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, meshRenderer->getEntity());
		if (!transform)
			return;


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
		shaderProgram->sendUniformMat4("uModel", transform->getLocalToWorldMatrix().data, false);
		shaderProgram->sendUniformMat4("uView", cameraInverse.data, false);
		shaderProgram->sendUniformMat4("uProj", _cameraComp->getProjection().data, false);

		shaderProgram->sendUniform("uTex", 0);

		/*for (Entity* lightEntity : lightEntities)
		{
			LightComponent* light = _entityManager->getComponent<LightComponent*>(ComponentType::Light, lightEntity);
			if (!light)
				break;

			if (light->getLightType() == LightType::Directional)
			{
				shaderProgram->sendUniform("lightPosition", cameraInverse * vec4(light->getPosition(), 1.0f));
				shaderProgram->sendUniform("lightAmbient", light->getAmbient());
				shaderProgram->sendUniform("lightDiffuse", light->getDiffuse());
				shaderProgram->sendUniform("lightSpecular", light->getSpecular());
				shaderProgram->sendUniform("lightSpecularExponent", light->getSpecularExp());
				shaderProgram->sendUniform("attenuationConstant", light->getAttenuationConstant());
				shaderProgram->sendUniform("attenuationLinear", light->getAttenuationLinear());
				shaderProgram->sendUniform("attenuationQuadratic", light->getAttenuationQuadratic());
			}
			else
			{
				shaderProgram->sendUniform("spotLightPosition", cameraInverse * vec4(spotLight->getPosition(), 1.0f));
				shaderProgram->sendUniform("spotLightDirection", vec3(1, 0, 0));
				shaderProgram->sendUniform("spotLightAmbient", spotLight->getAmbient());
				shaderProgram->sendUniform("spotLightDiffuse", spotLight->getDiffuse());
				shaderProgram->sendUniform("spotLightSpecular", spotLight->getSpecular());
				shaderProgram->sendUniform("spotLightSpecularExponent", spotLight->getSpecularExp());
				shaderProgram->sendUniform("spotLightattenuationConstant", spotLight->getAttenuationConstant());
				shaderProgram->sendUniform("spotLightattenuationLinear", spotLight->getAttenuationLinear());
				shaderProgram->sendUniform("spotLightattenuationQuadratic", spotLight->getAttenuationQuadratic());
			}
		}*/

		shaderProgram->sendUniform("lightPosition", cameraInverse * vec4(light->getPosition(), 1.0f));
		shaderProgram->sendUniform("lightAmbient", light->getAmbient());
		shaderProgram->sendUniform("lightDiffuse", light->getDiffuse());
		shaderProgram->sendUniform("lightSpecular", light->getSpecular());
		shaderProgram->sendUniform("lightSpecularExponent", light->getSpecularExp());
		shaderProgram->sendUniform("attenuationConstant", light->getAttenuationConstant());
		shaderProgram->sendUniform("attenuationLinear", light->getAttenuationLinear());
		shaderProgram->sendUniform("attenuationQuadratic", light->getAttenuationQuadratic());

		shaderProgram->sendUniform("spotLightPosition", cameraInverse * vec4(spotLight->getPosition(), 1.0f));
		shaderProgram->sendUniform("spotLightDirection", vec3(1, 0, 0));
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
		

		// If the entity has an animation, play it.
		//if (meshRenderer->getIsAnimated())
		//{
		//	PhysicsBodyComponent* _physicsBody = _entityManager->getComponent<PhysicsBodyComponent*>(ComponentType::PhysicsBody, meshRenderer->getEntity());
		//	meshRenderer->getAnimation()->animate(shaderProgram, _physicsBody->getVelocity().x / 7.5f);
		//}

		mesh->vao.draw();

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
