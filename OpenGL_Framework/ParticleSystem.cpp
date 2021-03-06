#include "ParticleSystem.h"
#include <math.h>


ParticleSystem::ParticleSystem(const string& name, const size_t numParticles)
{
	_name = name;


	vector<Texture*> textures;
	textures.push_back(ObjectLoader::getTexture("Dust"));
	renderer = new MeshRendererComponent(ObjectLoader::getMesh("ParticleQuad"), ObjectLoader::getShaderProgram("InstancedShader"), textures);


	for (size_t i = 0; i < numParticles; ++i)
	{
		spawnParticle();
	}
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* particle : _particles)
	{
		delete particle;
		particle = nullptr;
	}

	delete renderer;
	renderer = nullptr;
}

void ParticleSystem::update(float deltaTime)
{
	if (_active)
	{
		if (_isFluid)
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

					if (dist <= _smoothWidth)
						particle->_density += neighbour->_mass * poly6Kernel(dist); // TODO poly6Kernel could just be passed in dist sqaured
				}


				// Calculate the pressure for the current particle
				particle->_pressure = _gasConstant * (particle->_density - _restDensity);
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

					if (dist <= _smoothWidth)
					{
						// Calculate pressure force
						pressureGrad = -neighbour->_mass * ((particle->_pressure + neighbour->_pressure) / 2.0f * neighbour->_density) * spikyKernel(dist);
						pressureForce += dir * pressureGrad;

						// Calculate viscosity force
						viscosityForce += _viscosityConst * neighbour->_mass * ((neighbour->_velocity - particle->_velocity) / neighbour->_density) * viscosityKernel(dist);
					}
				}


				// Update the position of the current particle
				vec3 gravity = _gravity * particle->_density;	// Force of _gravity * density of current particle

				float interValue = invLerp(particle->_currLifeTime, 0.0f, particle->_totalLifeTime);
				vec3 extVelocity = lerp(particle->_startVelocity, particle->_endVelocity, interValue);

				particle->_force = pressureForce + viscosityForce + gravity; //+ vec3(static_cast<float>(rand() % 2), static_cast<float>(rand() % 2), 0.0f);		// Total force = force of pressure gradient + force of _gravity + external forces

				vec3 totalAccel = (particle->_density > 0) ? (particle->_force / particle->_density) : 0.0f;

				particle->_velocity += totalAccel * deltaTime + extVelocity;	// v = prevV + (force / density) * dt

				if (particle->_velocity.x > 600.0f)
					particle->_velocity.x = 600.0f;

				if (particle->_velocity.y > 600.0f)
					particle->_velocity.y = 600.0f;

				if (particle->_velocity.x < -600.0f)
					particle->_velocity.x = -600.0f;

				if (particle->_velocity.y < -600.0f)
					particle->_velocity.y = -600.0f;

				particle->transform->setLocalPosition(particle->transform->getWorldPosition() + particle->_velocity * deltaTime);	// p = prevP + (v * dt)

				particle->transform->update(deltaTime);


				particle->_force = vec3::Zero;
			}



		}
		else
		{
			for (Particle* particle : _particles)
			{
				particle->transform->update(deltaTime);
			}
		}



		// Clean up particles
		// Update the lifeTime of the particle, and check if it has expired.
		vector<Particle*>::iterator it;
		Particle* particle = nullptr;
		for (it = _particles.begin(); it != _particles.end();)
		{
			particle = *it;

			particle->_currLifeTime += deltaTime;

			if (particle->_currLifeTime >= particle->_totalLifeTime)
			{
				if (getLoop())
				{
					spawnParticle(particle);
				}
				else
				{
					it = _particles.erase(it);
					delete particle;
					particle = nullptr;
					continue;
				}
			}

			++it;
		}
	}
}

void ParticleSystem::draw(mat4 & camView, mat4 & camProj)
{
	Mesh* mesh = nullptr;
	ShaderProgram* shaderProgram = nullptr;
	Texture* texture = nullptr;
	vector<mat4> transforms;

	transforms.reserve(_particles.size());

	for (Particle* particle : _particles)
	{
		transforms.push_back(particle->transform->getLocalToWorldMatrixRef());
	}


	mesh = renderer->getMesh();
	shaderProgram = renderer->getShaderProgram();
	texture = renderer->getTexture(0);


	// Bind and send all the uniforms to the shader program.
	shaderProgram->bind();
	//shaderProgram->sendUniformMat4("uModel", transform->getLocalToWorldMatrix().data, false);
	shaderProgram->sendUniformMat4("uView", camView.data, false);
	shaderProgram->sendUniformMat4("uProj", camProj.data, false);


	// Bind the texture.
	texture->bind(0);

	// Bind the mesh.
	dynamic_cast<InstVertexArrayObject*>(mesh->vao)->draw(_particles.size(), transforms);

	// Unbind the texture.
	texture->unBind(0);


	// Unbind the shader program.
	shaderProgram->unBind();
}

void ParticleSystem::addToDrawList(vector<Particle*> & drawList) const
{
	drawList.insert(drawList.end(), _particles.begin(), _particles.end());
}

string ParticleSystem::getName() const
{
	return _name;
}

bool ParticleSystem::isActive() const
{
	return _active;
}

void ParticleSystem::setActive(const bool active)
{
	_active = active;
}

vector<Particle*> ParticleSystem::getParticles() const
{
	return _particles;
}

void ParticleSystem::spawnParticle()
{
	const float rangeFrom = 0.0f;
	const float rangeTo = 1.0f;
	random_device randDev;
	mt19937 generator(randDev());
	uniform_real_distribution<float> distr(rangeFrom, rangeTo);

	float lifetime = lerp(_minLifetime, _maxLifetime, distr(generator));

	float xPos = lerp(_minSpawnPos.x, _maxSpawnPos.x, distr(generator));
	float yPos = lerp(_minSpawnPos.y, _maxSpawnPos.y, distr(generator));
	vec3 spawnPos = vec3(xPos, yPos, 0.0f);

	float scaleX = lerp(_startScale.x, _endScale.x, distr(generator));
	//float scaleY = lerp(_startScale.y, _endScale.y, distr(generator));
	vec3 scale = vec3(scaleX, scaleX, 1.0f);

	//vec3 velocity = lerp(_startVelocity, _endVelocity, distr(generator));

	Particle* particle = new Particle();
	particle->transform->setWorldPosition(spawnPos);
	particle->transform->setLocalScale(scale);
	particle->_mass = _particleMass;
	particle->_totalLifeTime = lifetime;
	particle->_startVelocity = _startVelocity;
	particle->_endVelocity = _endVelocity;

	_particles.push_back(particle);
}

void ParticleSystem::spawnParticle(Particle * particle)
{
	const float rangeFrom = 0.0f;
	const float rangeTo = 1.0f;
	random_device randDev;
	mt19937 generator(randDev());
	uniform_real_distribution<float> distr(rangeFrom, rangeTo);

	float lifetime = lerp(_minLifetime, _maxLifetime, distr(generator));

	float xPos = lerp(_minSpawnPos.x, _maxSpawnPos.x, distr(generator));
	float yPos = lerp(_minSpawnPos.y, _maxSpawnPos.y, distr(generator));
	vec3 spawnPos = vec3(xPos, yPos, 0.0f);

	float scaleX = lerp(_startScale.x, _endScale.x, distr(generator));
	//float scaleY = lerp(_startScale.y, _endScale.y, distr(generator));
	vec3 scale = vec3(scaleX, scaleX, 1.0f);

	//vec3 velocity = lerp(_startVelocity, _endVelocity, distr(generator));

	particle->transform->setWorldPosition(spawnPos);
	particle->transform->setLocalScale(scale);
	particle->_mass = _particleMass;
	particle->_totalLifeTime = lifetime;
	particle->_currLifeTime = 0.0f;
	particle->_velocity = vec3::Zero;
	particle->_startVelocity = _startVelocity;
	particle->_endVelocity = _endVelocity;
}

void ParticleSystem::respawnParticles()
{
	//size_t numParticles = _particles.size();

	//for (Particle* particle : _particles)
	//{
	//	delete particle;
	//}

	//_particles.clear();

	//for (size_t i = 0; i < numParticles; ++i)
	//{
	//	spawnParticle();
	//}

	for (Particle* particle : _particles)
	{
		spawnParticle(particle);
	}
}

bool ParticleSystem::getLoop() const
{
	return _loop;
}

void ParticleSystem::setLoop(const bool loop)
{
	_loop = loop;
}

Texture* ParticleSystem::getTexture() const
{
	return renderer->getTexture(0);
}

void ParticleSystem::setTexture(Texture * texture)
{
	renderer->setTexture(0, texture);
}

size_t ParticleSystem::getNumParticles() const
{
	return _particles.size();
}

float ParticleSystem::getMinLifetime() const
{
	return _minLifetime;
}

void ParticleSystem::setMinLifetime(const float lifetime)
{
	_minLifetime = lifetime;
}

float ParticleSystem::getMaxLifetime() const
{
	return _maxLifetime;
}

void ParticleSystem::setMaxLifetime(const float lifetime)
{
	_maxLifetime = lifetime;
}

vec3 ParticleSystem::getMinSpawnPos() const
{
	return _minSpawnPos;
}

void ParticleSystem::setMinSpawnPos(const vec3 & pos)
{
	_minSpawnPos = pos;
}

vec3 ParticleSystem::getMaxSpawnPos() const
{
	return _maxSpawnPos;
}

void ParticleSystem::setMaxSpawnPos(const vec3 & pos)
{
	_maxSpawnPos = pos;
}

vec3 ParticleSystem::getStartScale() const
{
	return _startScale;
}

void ParticleSystem::setStartScale(const vec3 & scale)
{
	_startScale = scale;
}

vec3 ParticleSystem::getEndScale() const
{
	return _endScale;
}

void ParticleSystem::setEndScale(const vec3 & scale)
{
	_endScale = scale;
}

vec3 ParticleSystem::getStartVelocity() const
{
	return _startVelocity;
}

void ParticleSystem::setStartVelocity(const vec3 & velocity)
{
	_startVelocity = velocity;
}

vec3 ParticleSystem::getEndVelocity() const
{
	return _endVelocity;
}

void ParticleSystem::setEndVelocity(const vec3 & velocity)
{
	_endVelocity = velocity;
}

bool ParticleSystem::getIsFluid() const
{
	return _isFluid;
}

void ParticleSystem::setIsFluid(const bool isFluid)
{
	_isFluid = isFluid;
}

float ParticleSystem::getSmoothWidth() const
{
	return _smoothWidth;
}

void ParticleSystem::setSmoothWidth(const float smoothWidth)
{
	_smoothWidth = smoothWidth;
}

float ParticleSystem::getGasConstant() const
{
	return _gasConstant;
}

void ParticleSystem::setGasConstant(const float gasConstant)
{
	_gasConstant = gasConstant;
}

float ParticleSystem::getRestDensity() const
{
	return _restDensity;
}

void ParticleSystem::setRestDensity(const float restDensity)
{
	_restDensity = restDensity;
}

vec3 ParticleSystem::getGravity() const
{
	return _gravity;
}

void ParticleSystem::setGravity(const vec3 & gravity)
{
	_gravity = gravity;
}

float ParticleSystem::getViscosityConst() const
{
	return _viscosityConst;
}

void ParticleSystem::setViscosityConst(const float viscosityConst)
{
	_viscosityConst = viscosityConst;
}

float ParticleSystem::getParticleMass() const
{
	return _particleMass;
}

void ParticleSystem::setParticleMass(const float mass)
{
	_particleMass = mass;

	for (Particle* particle : _particles)
	{
		particle->_mass = mass;
	}
}

vector<Particle*> ParticleSystem::getNeighbours(Particle * particle)
{
	return _particles; // TODO Return the particles, which would actually infuence the current particle
}

float ParticleSystem::gaussianKernel(const float dist)
{
	float result = 1.0f / (pow(PI, 3.0f / 2.0f) * pow(_smoothWidth, 3.0f)) * exp(pow(dist, 2.0f) / pow(_smoothWidth, 2.0f));

	return result;
}

float ParticleSystem::poly6Kernel(const float dist)
{
	return (315.0f / (64.0f * PI * pow(_smoothWidth, 9.0f)) * pow(pow(_smoothWidth, 2.0f) - pow(dist, 2.0f), 3.0f));
}

float ParticleSystem::spikyKernel(const float dist)
{
	return (-dist * (45.0f / (PI * pow(_smoothWidth, 6) * dist) * pow(_smoothWidth - dist, 2.0f)));
}

float ParticleSystem::viscosityKernel(const float dist)
{
	return (45.0f / (PI * pow(_smoothWidth, 6.0f) * (_smoothWidth - dist)));
}
