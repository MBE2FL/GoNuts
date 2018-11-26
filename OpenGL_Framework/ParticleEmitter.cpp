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
//	_shaderProgram->sendUniform("lightPosition", camera.getLocalToWorldMatrix().GetInverse() * Vector4(light->getPosition(), 1.0f));
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

Matrix44 Particle::getLocalToWorldMatrix() const
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
			Particles->_physicsBody = nullptr;
			Particles->setShaderProgram(ObjectLoader::getShaderProgram("Normal"));
			Particles->setMesh(ObjectLoader::getMesh("Plane"));
			Particles->setTexture(ObjectLoader::getTexture("Default"));
			Particles->addPhysicsBody(false);
			Particles->setPosition(emitterPosition);
			Particles->setScale(Vector3(1.f));
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
		
		for (unsigned int i = 0; i < m_pNumParticles; ++i)
		{
			Particle* particle = m_pParticles[i];
			if (particle->life <= 0) // if particle has no life remaining
			{
				// Respawn particle
				// Note: we are not freeing memory, we are "Recycling" the particles
				particle->acceleration = Vector3(0.0f);

				//This will generate a number from 0.0 to 1.0, inclusive.
				float randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				particle->colorBegin = MathLibCore::lerp(colorBegin0, colorBegin1, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->colorEnd = MathLibCore::lerp(colorEnd0, colorEnd1, randomTval);
				particle->color = particle->colorBegin;
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->life = MathLibCore::lerp(lifeRange.x, lifeRange.y, randomTval);
				particle->lifetime = particle->life;
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->mass = MathLibCore::lerp(massRange.x, massRange.y, randomTval);

				spawnRadius1.x = emitterPosition.x + range;
				spawnRadius1.y = emitterPosition.y + range;
				spawnRadius2.x = emitterPosition.x - range;
				spawnRadius2.y = emitterPosition.y - range;

				{
					float randomTval1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float randomTval2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					particle->_transform->setPosition(Vector3(MathLibCore::lerp(spawnRadius1.x, spawnRadius2.x, randomTval1),
						MathLibCore::lerp(spawnRadius1.y, spawnRadius2.y, randomTval2), emitterPosition.z));
				}

				particle->sizeBegin = MathLibCore::lerp(sizeBegin.x, sizeBegin.y, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->sizeEnd = MathLibCore::lerp(sizeEnd.x, sizeEnd.y, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				particle->size = MathLibCore::lerp(sizeRange.x, sizeRange.y, randomTval);
				randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


				if (velocityRandomValues)
				{
					particle->velocity.x = MathLibCore::lerp(velocity0.x, velocity1.x, randomTval);
					randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					particle->velocity.y = MathLibCore::lerp(velocity0.y, velocity1.y, randomTval);
					randomTval = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					particle->velocity.z = MathLibCore::lerp(velocity0.z, velocity1.z, randomTval);
				}
				else
				{
					particle->velocity = MathLibCore::lerp(velocity0, velocity1, randomTval);
				}
			}

			// Update physics

			// Update acceleration
			particle->acceleration = particle->force / particle->mass;
			particle->velocity = particle->velocity + (particle->acceleration * dt);
			particle->_transform->setPosition(particle->_transform->getPosition() + particle->velocity * dt * 60.0f);
			particle->_transform->update(dt);

			// We've applied the force, let's remove it so it does not get applied next frame
			particle->force = Vector3(0.0f);

			// Decrease particle life
			particle->life -= dt;

			{
				float tVal = MathLibCore::invLerp(particle->life, particle->lifetime, 0.0f);
				particle->size = MathLibCore::lerp(particle->sizeBegin, particle->sizeEnd, tVal);
			}

			// Update visual properties
			if (interpolateColor)
			{
				// calculate t value
				float tVal = MathLibCore::invLerp(particle->life, particle->lifetime, 0.0f);
				particle->color = MathLibCore::lerp(particle->colorBegin, particle->colorEnd, tVal);
			}
		}
	}
}

void ParticleEmitter::draw(Camera &camera, Light *light)
{
	// Draw the emitter position
	// Note: not necessary
	//TTK::Graphics::DrawTeapot(emitterPosition, 50.0f, Vector4(1.0f));

	
	for (unsigned int i = 0; i < m_pNumParticles; ++i)
	{
		//Particle* p = m_pParticles[i];
		if (!m_pParticles.empty() && m_pParticles[i]->life) // if particle is alive, draw it
		{
			(m_pParticles[i])->draw(camera, light);

			//TTK::Graphics::DrawTeapot(p->position, p->size, p->color); // low fps alert!! use with low particle count
			//TTK::Graphics::DrawPoint(p->position, p->size, p->color);
			// You can draw anything!
			// ...but you should stick to points in this framework since it uses GL 1.0
		}
	}
}

void ParticleEmitter::applyForceToParticle(unsigned int idx, Vector3 force)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::applyForceToParticle ERROR: idx " << idx << "out of range!" << std::endl;
		return;
	}

	m_pParticles[idx]->force = force;
}

Vector3 ParticleEmitter::getParticlePosition(unsigned int idx)
{
	if (idx >= m_pNumParticles)
	{
		std::cout << "ParticleEmitter::getParticlePosition ERROR: idx " << idx << "out of range!" << std::endl;
		return Vector3();
	}

	return m_pParticles[idx]->_transform->getPosition();
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