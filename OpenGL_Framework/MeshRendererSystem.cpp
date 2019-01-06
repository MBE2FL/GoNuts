#include "MeshRendererSystem.h"

MeshRendererSystem::MeshRendererSystem(EntityManager * entityManager)
	: System(entityManager)
{
}

MeshRendererSystem::~MeshRendererSystem()
{
}

void MeshRendererSystem::draw(Camera& camera, Light* light, Light* spotLight, Matrix44& cameraInverse)
{
	vector<Entity*> entities = _entityManager->getAllEntitiesWithComponent(ComponentType::MeshRenderer);

	for (Entity* entity : entities)
	{
		// Get the transform and mesh renderer components for the current entity. Only draw if both components exist.
		//Component* component = _entityManager->getComponent(ComponentType::Transform, entity);
		TransformComponent* transform = _entityManager->getComponent<TransformComponent*>(ComponentType::Transform, entity);
		if (!transform)
			return;

		//component = _entityManager->getComponent(ComponentType::MeshRenderer, entity);
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
		shaderProgram->sendUniformMat4("uProj", camera.getProjection().mV, false);

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
