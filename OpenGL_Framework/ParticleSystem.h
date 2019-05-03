#pragma once

#include "MathLib/MathLibCore.h"
#include <vector>
#include "MeshRendererComponent.h"
#include <random>

using std::vector;
using std::random_device;
using std::mt19937;
using std::uniform_real_distribution;

struct Particle
{
	Particle()
	{
		transform = new TransformComponent();
	};
	~Particle()
	{
		delete transform;
		transform = nullptr;
	};


	float _currLifeTime = 0.0f;
	float _totalLifeTime = 0.0f;
	float _mass = 0.0f;
	float _density = 0.0f;
	float _pressure = 0.0f;


	vec3 _force;
	vec3 _velocity;

	vec3 _startVelocity;
	vec3 _endVelocity;

	TransformComponent* transform;
};

class ParticleSystem
{
public:
	ParticleSystem(const string& name, const size_t numParticles);
	~ParticleSystem();

	void update(float deltaTime);
	void draw(mat4 & camView, mat4 & camProj);
	void addToDrawList(vector<Particle*>& drawList) const;
	string getName() const;
	bool isActive() const;
	void setActive(const bool active);
	vector<Particle*> getParticles() const;
	void spawnParticle();
	void spawnParticle(Particle* particle);
	void respawnParticles();
	bool getLoop() const;
	void setLoop(const bool loop);
	Texture* getTexture() const;
	void setTexture(Texture* texture);
	size_t getNumParticles() const;

	float getMinLifetime() const;
	void setMinLifetime(const float lifetime);
	float getMaxLifetime() const;
	void setMaxLifetime(const float lifetime);

	vec3 getMinSpawnPos() const;
	void setMinSpawnPos(const vec3& pos);
	vec3 getMaxSpawnPos() const;
	void setMaxSpawnPos(const vec3& pos);

	vec3 getStartScale() const;
	void setStartScale(const vec3& scale);
	vec3 getEndScale() const;
	void setEndScale(const vec3& scale);

	vec3 getStartVelocity() const;
	void setStartVelocity(const vec3& velocity);
	vec3 getEndVelocity() const;
	void setEndVelocity(const vec3& velocity);

	bool getIsFluid() const;
	void setIsFluid(const bool isFluid);
	float getSmoothWidth() const;
	void setSmoothWidth(const float smoothWidth);
	float getGasConstant() const;
	void setGasConstant(const float gasConstant);
	float getRestDensity() const;
	void setRestDensity(const float restDensity);
	vec3 getGravity() const;
	void setGravity(const vec3& gravity);
	float getViscosityConst() const;
	void setViscosityConst(const float viscosityConst);
	float getParticleMass() const;
	void setParticleMass(const float mass);

private:
	vector<Particle*> _particles;
	string _name;
	bool _active = false;
	bool _loop = false;
	MeshRendererComponent* renderer;

	float _minLifetime = 0.0f;
	float _maxLifetime = 0.0f;

	vec3 _minSpawnPos = vec3(600.0f, 500.0f, 0.0f);
	vec3 _maxSpawnPos = vec3(900.0f, 700.0f, 0.0f);

	vec3 _startScale = vec3(20.0f, 20.0f, 1.0f);
	vec3 _endScale = vec3(50.0f, 50.0f, 1.0f);

	vec3 _startVelocity;
	vec3 _endVelocity;


	// Fluid Dynamics
	bool _isFluid = false;
	float _smoothWidth = 120.0f;
	float _gasConstant = 2000.f;
	float _restDensity = 1000.0f;
	vec3 _gravity = vec3(0.0f, 0.0f, 0.0f);
	float _viscosityConst = 250.0f;
	float _particleMass = 15.0f;



	vector<Particle*> getNeighbours(Particle* particle);
	float gaussianKernel(const float dist);
	float poly6Kernel(const float dist);
	float spikyKernel(const float dist);
	float viscosityKernel(const float dist);
	
};