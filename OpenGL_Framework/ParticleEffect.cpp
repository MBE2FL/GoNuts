#include "ParticleEffect.h"
#include <math.h>

#define SMOOTH_WIDTH 120.0f
#define GAS_CONSTANT 2000.0f
#define REST_DENSITY 1000.f
#define GRAVITY vec3(0.0f, 30.0f, 0.0f)
#define VISCOSITY_CONST 250.0f

ParticleEffect::ParticleEffect(const size_t numParticles)
{
	// Create particles
	Particle* particle = nullptr;

	for (size_t i = 0; i < numParticles; ++i)
	{
		particle = new Particle();
		_particles.push_back(particle);
	}
}

ParticleEffect::~ParticleEffect()
{
	for (Particle* particle : _particles)
	{
		delete particle;
		particle = nullptr;
	}
}

void ParticleEffect::update(float deltaTime)
{
	vector<Particle*> neighbours;

	// Calculate the densities of all the particles
	for (Particle* particle : _particles)
	{
		// Reset density from previous frame
		particle->_density = 0.0f;
		particle->_pressure = 0.0f;


		// Get all neighbouring particles
		neighbours = getNeighbours(particle);


		// Calculate the density for the current particle
		float dist = 0.0f;
		for (Particle* neighbour : neighbours)
		{
			if (neighbour == particle) // TODO Remove once getNeighbours has been improved
				continue;

			dist = (neighbour->transform->getWorldPosition() - particle->transform->getWorldPosition()).length();

			if (dist <= SMOOTH_WIDTH)
				particle->_density += neighbour->_mass * poly6Kernel(dist); // TODO poly6Kernel could just be passed in dist sqaured
		}


		// Calculate the pressure for the current particle
		particle->_pressure = GAS_CONSTANT * (particle->_density - REST_DENSITY);
	}



	// Calculate the pressure for the current particle
	for (Particle* particle : _particles)
	{
		// Get all neighbouring particles
		neighbours = getNeighbours(particle);


		// Calculate the acceleration, due to pressure, for the current particle
		vec3 pressureForce;
		float pressureGrad = 0.0f;
		vec3 dir;
		float dist = 0.0f;
		vec3 viscosityForce;


		for (Particle* neighbour : neighbours)
		{
			if (neighbour == particle) // TODO Remove once getNeighbours has been improved
				continue;

			dir = neighbour->transform->getWorldPosition() - particle->transform->getWorldPosition();
			dist = dir.length();
			dir /= dist;

			if (dist <= SMOOTH_WIDTH)
			{
				// Calculate pressure force
				pressureGrad = -neighbour->_mass * ((particle->_pressure + neighbour->_pressure) / 2.0f * neighbour->_density) * spikyKernel(dist);
				pressureForce += dir * pressureGrad;

				// Calculate viscosity force
				//viscosityForce += VISCOSITY_CONST * neighbour->_mass * ((neighbour->_velocity - particle->_velocity) / neighbour->_density) * viscosityKernel(dist);
			}
		}


		// Update the position of the current particle
		vec3 gravity = GRAVITY * particle->_density;	// Force of gravity * density of current particle

		particle->_force = pressureForce + viscosityForce + gravity; //+ vec3(static_cast<float>(rand() % 2), static_cast<float>(rand() % 2), 0.0f);		// Total force = force of pressure gradient + force of gravity + external forces

		vec3 totalAccel = (particle->_density > 0) ? (particle->_force / particle->_density) : 0.0f;

		particle->_velocity += totalAccel * deltaTime;	// v = prevV + (force / density) * dt

		if (particle->_velocity.x > 50.0f)
			particle->_velocity.x = 50.0f;

		if (particle->_velocity.y > 50.0f)
			particle->_velocity.y = 50.0f;

		if (particle->_velocity.x < -50.0f)
			particle->_velocity.x = -50.0f;

		if (particle->_velocity.y < -50.0f)
			particle->_velocity.y = -50.0f;

		particle->transform->setLocalPosition(particle->transform->getWorldPosition() + particle->_velocity * deltaTime);	// p = prevP + (v * dt)

		particle->transform->update(deltaTime);


		particle->_force = vec3::Zero;
	}



	// Update the lifeTime of the particle, and check if it has expired.
	// Clean up particles
}

void ParticleEffect::draw(mat4 & camView, mat4 & camProj)
{
	Mesh* mesh = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	Texture* texture = nullptr;
	TransformComponent* transform = nullptr;

	for (Particle* particle : _particles)
	{
		mesh = particle->renderer->getMesh();
		shaderProgram = particle->renderer->getShaderProgram();
		texture = particle->renderer->getTexture(0);
		transform = particle->transform;


		// Bind and send all the uniforms to the shader program.
		shaderProgram->bind();
		shaderProgram->sendUniformMat4("uModel", transform->getLocalToWorldMatrix().data, false);
		shaderProgram->sendUniformMat4("uView", camView.data, false);
		shaderProgram->sendUniformMat4("uProj", camProj.data, false);


		// Bind the texture.
		texture->bind(0);

		// Bind the mesh.
		mesh->vao.draw();

		// Unbind the texture.
		texture->unBind(0);


		// Unbind the shader program.
		shaderProgram->unBind();
	}
}

vector<Particle*> ParticleEffect::getNeighbours(Particle * particle)
{
	return _particles; // TODO Return the particles, which would actually infuence the current particle
}

float ParticleEffect::gaussianKernel(const float dist)
{
	float result = 1.0f / (pow(PI, 3.0f / 2.0f) * pow(SMOOTH_WIDTH, 3.0f)) * exp(pow(dist, 2.0f) / pow(SMOOTH_WIDTH, 2.0f));

	return result;
}

float ParticleEffect::poly6Kernel(const float dist)
{
	return (315.0f / (64.0f * PI * pow(SMOOTH_WIDTH, 9.0f)) * pow(pow(SMOOTH_WIDTH, 2.0f) - pow(dist, 2.0f), 3.0f));
}

float ParticleEffect::spikyKernel(const float dist)
{
	return (-dist * (45.0f / (PI * pow(SMOOTH_WIDTH, 6) * dist) * pow(SMOOTH_WIDTH - dist, 2.0f)));
}

float ParticleEffect::viscosityKernel(const float dist)
{
	return (45.0f / (PI * pow(SMOOTH_WIDTH, 6.0f) * (SMOOTH_WIDTH - dist)));
}
