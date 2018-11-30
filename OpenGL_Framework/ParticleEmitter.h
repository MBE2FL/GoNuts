#pragma once

#include "MathLib/MathLibCore.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "Component.h"
#include "Light.h"
#include "GameObject.h"
#include "ObjectLoader.h"

// Params for each particle
class Particle : public GameObject
{
public:
	
	//void draw(Camera& camera, Light* light);
	Matrix44 getLocalToWorldMatrix() const;

	// Physics properties
	// Position is where the particle currently is
	// Velocity is the rate of change of position
	// Acceleration is the rate of change of velocity
	// p' = p + v*dt
		//	current position is the previous position plus the change in position multiplied by the amount of time passed since we last calculated position
	// v' = v + a*dt
	//Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 force;
	// You can add an option to limit the parameters above
	float mass;


	// Visual Properties
	float size;
	float sizeBegin;
	float sizeEnd;
	Vector4 color;
	Vector4 colorBegin;
	Vector4 colorEnd;
	// ... sprite sheet ...





	float life;
	float lifetime; // If we want to inverselerp the life based on the lifetime of the particle
};

// Emitter is responsible for emitting (creating, dispatching) particles
class ParticleEmitter : public GameObject
{
public:
private:
	unsigned int m_pNumParticles;
	

public:
	ParticleEmitter();
	~ParticleEmitter();

	std::vector<Particle*> m_pParticles;

	void initialize(unsigned int numParticles);
	void freeMemory();

	virtual void update(float dt);
	virtual void draw(Camera &camera, Light *light, Light* spotLight, Matrix44& cameraInverse);

	void applyForceToParticle(unsigned int idx, Vector3 force);

	unsigned int getNumParticles() { return m_pNumParticles; }

	Vector3 getParticlePosition(unsigned int idx);
	float getParticleMass(unsigned int idx);

	// Creation properties
	bool velocityRandomValues;

	///// Playback properties
	bool playing;

	std::string texName;

	//// Update flags
	bool interpolateColor;

	///// Initial properties for newly spawned particles

	Vector3 emitterPosition;
	float spawnDelay = 0.0f;

	// Storing ranges for each property so each particle has some randomness
	Vector3 velocity0;
	Vector3 velocity1;

	Vector3 spawnRadius1;
	Vector3 spawnRadius2;
	float range = 0.0f;

	// these properties are single floats, so we can pack the min and max into a vec2, just data!
	Vector2 lifeRange;
	Vector2 sizeRange;
	Vector2 sizeEnd;
	Vector2 sizeBegin;
	Vector2 massRange;

	Vector4 colorBegin0;
	Vector4 colorBegin1;
	Vector4 colorEnd0;
	Vector4 colorEnd1;


	Vector3 partLocalPos = Vector3(5.0f, -7.5f, 0.0f);

	// ... other properties
	// ... what would be a better way of doing this?
	// Make a keyframe controller for each property! this gives you max control!!
	// (this is what full out particle editors do, ie popcorn fx)
};