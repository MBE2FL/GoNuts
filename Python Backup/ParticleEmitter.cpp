#include <iostream> // for std::cout
//#include <glm\gtc\random.hpp> // for glm::linearRand
//#include <TTK\GraphicsUtils.h> // for drawing utilities

#include "Math.h"
#include "ParticleEmitter.h"



//void Particle::draw(Camera& camera, Light* light)
//{
//	_shaderProgram->bind();
//	_shaderProgram->sendUniformMat4("uModel", getLocalToWorldMatrix().mV, false);
//	_shaderProgram->sendUniformMat4("uView", camera.getLocalToWorldMatrix().GetInverse().mV, false);
//	_shaderProgram->sendUniformMat4("uProj", camera.getProjection().mV, false);
//
//
//	_shaderProgram->sendUniform("uTex", 0);
//
//
//	_shaderProgram->sendUniform("lightPosition", camera.getLocalToWorldMatrix().GetInverse() * vec4(light->getPosition(), 1.0f));
//	_shaderProgram->sendUniform("lightAmbient", light->getAmbient());
//	_shaderProgram->sendUniform("lightDiffuse", light->getDiffuse());
//	_shaderProgram->sendUniform("lightSpecular", light->getSpecular());
//	_shaderProgram->sendUniform("lightSpecularExponent", light->getSpecularExp());
//	_shaderProgram->sendUniform("attenuationConstant", light->getAttenuationConstant());
//	_shaderProgram->sendUniform("attenuationLinear", light->getAttenuationLinear());
//	_shaderProgram->sendUniform("attenuationQuadratic", light->getAttenuationQuadratic());
//
//	_texture->bind();
//
//	glBindVertexArray(_mesh->VAO);
//	glDrawArrays(GL_TRIANGLES, 0, _mesh->getNumVertices());
//	glBindVertexArray(GL_NONE);
//
//	_texture->unBind();
//
//	_shaderProgram->unBind();
//}

mat4 Particle::getLocalToWorldMatrix() const
{
	return _transform->getLocalToWorldMatrix();
}




ParticleEmitter::ParticleEmitter()
	:
	m_pNumParticles(0),
	playing(true),
	interpolateColor(false),
	velocityRandomValues(true)
{

}

ParticleEmitter::~ParticleEmitter()
{
	freeMemory();
}

void ParticleEmitter::initialize(unsigned int numParticles)
{
	freeMemory(); // destroy any existing particles

	if (numParticles > 0)
	{
		//m_pParticles = new Particle[numParticles];
		//memset(m_pParticles, 0, sizeof(Particle) * numParticles);
		m_pNumParticles = numParticles;

		for (unsigned int i = 0; i < numParticles; i++)
		{
			Particle *Particles = new Particle;
			Particles->_transform = new Transform();
			Particles->_mesh = nullptr;
			Particles->_shaderProgram = nullptr;
			//Particles->_physicsBody = nullptr;
			Particles->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
			Particles->setMesh(ObjectLoader::getMesh("Plane"));
			Particles->setTexture(ObjectLoader::getTexture(texName));
			//Particles->addPhysicsBody(false);
			Particles->setParent(this);
			//Particles->setWorldPosition(getWorldPosition());
			Particles->setLocalPosition(vec3(0.0f));
			Particles->setLocalScale(getLocalScale());
			m_pParticles.push_back(Particles);
		}

	}
}

void ParticleEmitter::freeMemory()
{
	if (!m_pParticles.empty()) // if not a null pointer
	{

		for (unsigned int i = 0; i < m_pParticles.size(); i++)
		{
			delete m_pParticles[i];
			m_pParticles[i] = nullptr;
		}
		m_pParticles.clear();
		m_pNumParticles = 0;
	}
}

void ParticleEmitter::update(float dt)
{
	if (!m_pParticles.empty() && playing) // make sure memory is initialized and system is playing
	{
		// loop through each particle
		_transform->update(dt);
		
		for (unsigned int i = 0; i < m_pNumParticles; ++i)
		{
			Particle* particle = m_pParticles[i];
			if (particle->life <= 0) // if particle has no life remaining
			{
				// Respawn particle
				// Note: we are not freeing memory, we are "Recycling" the particles
				particle->acceleration = vec3(0.0f);

				//This will generate a number from 0.0 to 1.0, inclusive.
				float randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				particle->colorBegin = lerp(colorBegin0, colorBegin1, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->colorEnd = lerp(colorEnd0, colorEnd1, randomTval);
				particle->color = particle->colorBegin;
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->life = lerp(lifeRange.x, lifeRange.y, randomTval);
				particle->lifetime = particle->life;
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->mass = lerp(massRange.x, massRange.y, randomTval);

				spawnRadius1.x = getLocalPosition().x + range;
				spawnRadius1.y = getLocalPosition().y + range;
				spawnRadius2.x = getLocalPosition().x - range;
				spawnRadius2.y = getLocalPosition().y - range;

				{
					float randomTval1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randomTval2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					particle->setLocalPosition(vec3(lerp(spawnRadius1.x, spawnRadius2.x, randomTval1),
						lerp(spawnRadius1.y, spawnRadius2.y, randomTval2), getLocalPosition().z));
					particle->setLocalPosition(partLocalPos);
				}

				particle->sizeBegin = lerp(sizeBegin.x, sizeBegin.y, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->sizeEnd = lerp(sizeEnd.x, sizeEnd.y, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->size = lerp(sizeRange.x, sizeRange.y, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


				if (velocityRandomValues)
				{
					particle->velocity.x = lerp(velocity0.x, velocity1.x, randomTval);
					randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					particle->velocity.y = lerp(velocity0.y, velocity1.y, randomTval);
					randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					particle->velocity.z = lerp(velocity0.z, velocity1.z, randomTval);
				}
				else
				{
					particle->velocity = lerp(velocity0, velocity1, randomTval);
				}
			}

			// Update physics

			// Update acceleration
			particle->acceleration = particle->force / particle->mass;
			particle->velocity = particle->velocity + (particle->acceleration * dt);
			particle->setLocalPosition(particle->getLocalPosition() + particle->velocity * dt * 60.0f);
			//particle->setLocalPosition(vec3::Zero);
			//particle->setLocalPosition(partLocalPos);
			particle->_transform->update(dt);

			// We've applied the force, let's remove it so it does not get applied next frame
			particle->force = vec3(0.0f);

			// Decrease particle life
			particle->life -= dt;

			{
				float tVal = invLerp(particle->life, particle->lifetime, 0.0f);
				particle->size = lerp(particle->sizeBegin, particle->sizeEnd, tVal);
			}

			// Update visual properties
			if (interpolateColor)
			{
				// calculate t value
				float tVal = invLerp(particle->life, particle->lifetime, 0.0f);
				particle->color = lerp(particle->colorBegin, particle->colorEnd, tVal);
			}
		}
	}
}

//void ParticleEmitter::draw(Camera &camera, Light *light, Light* spotLight, Matrix44& cameraInverse)
//{
//	// Draw the emitter position
//	// Note: not necessary
//	//TTK::Graphics::DrawTeapot(emitterPosition, 50.0f, vec4(1.0f));
//
//	
//	for (unsigned int i = 0; i < m_pNumParticles; ++i)
//	{
//		//Particle* p = m_pParticles[i];
//		if (!m_pParticles.empty() && m_pParticles[i]->life) // if particle is alive, draw it
//		{
//			(m_pParticles[i])->draw(camera, light, spotLight, cameraInverse);
//
//			//TTK::Graphics::DrawTeapot(p->position, p->size, p->color); // low fps alert!! use with low particle count
//			//TTK::Graphics::DrawPoint(p->position, p->size, p->color);
//			// You can draw anything!
//			// ...but you should stick to points in this framework since it uses GL 1.0
//		}
//	}
//}

void ParticleEmitter::applyForceToParticle(unsigned int idx, vec3 force)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::applyForceToParticle ERROR: idx " << idx << "out of range!" << std::endl;
		return;
	}

	m_pParticles[idx]->force = force;
}

vec3 ParticleEmitter::getParticlePosition(unsigned int idx)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::getParticlePosition ERROR: idx " << idx << "out of range!" << std::endl;
		return vec3();
	}

	return m_pParticles[idx]->_transform->getWorldPosition();
}

float ParticleEmitter::getParticleMass(unsigned int idx)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::getParticleMass ERROR: idx " << idx << "out of range!" << std::endl;
		return 0.0f;
	}

	return m_pParticles[idx]->mass;
}
