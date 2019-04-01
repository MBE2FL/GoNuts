#pragma once

#include "MathLib/MathLibCore.h"
#include <vector>
#include "MeshRendererComponent.h"

using std::vector;

struct Particle
{
	Particle()
	{
		transform = new TransformComponent();
		float randX = 800.0f + (rand() % 400);
		float randY = 400.0f + (rand() % 200);
		float randScaleXY = 10.0f + (rand() % 20);
		transform->setWorldPosition(vec3(randX, randY, 0.0f));
		transform->setLocalScale(vec3(randScaleXY, randScaleXY, 1.0f));

		vector<Texture*> textures;
		textures.push_back(ObjectLoader::getTexture("Dust"));
		renderer = new MeshRendererComponent(ObjectLoader::getMesh("UIQuad"), ObjectLoader::getShaderProgram("UIShader"), textures);

		_mass = 15.0f;
	};
	~Particle()
	{
		delete transform;
		transform = nullptr;

		delete renderer;
		renderer = nullptr;
	};


	float _lifeTime;
	float _mass;
	float _density;
	float _pressure;


	vec3 _force;
	vec3 _velocity;


	vec3 _startPos;
	vec3 _endPos;
	vec3 _startScale;
	vec3 _endScale;
	vec3 _startForce;
	vec3 _endForce;

	TransformComponent* transform;
	MeshRendererComponent* renderer;
};

class ParticleEffect
{
public:
	ParticleEffect(const size_t numParticles);
	~ParticleEffect();

	void update(float deltaTime);
	void draw(mat4 & camView, mat4 & camProj);

private:
	vector<Particle*> _particles;


	vector<Particle*> getNeighbours(Particle* particle);
	float gaussianKernel(const float dist);
	float poly6Kernel(const float dist);
	float spikyKernel(const float dist);
	float viscosityKernel(const float dist);
	
};