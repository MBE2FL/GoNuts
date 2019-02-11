#pragma once


#include "MathLib/MathLibCore.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "Texture.h"
//#include "Camera.h"
#include "Component.h"
#include "Light.h"
#include "GameObject.h"
#include "ObjectLoader.h"


using MathUtils::lerp;
using MathUtils::invLerp;


// Params for each particle
class Particle : public GameObject
{
public:
	
	//void draw(Camera& camera, Light* light);
	mat4 getLocalToWorldMatrix() const;

	// Physics properties
	// Position is where the particle currently is
	// Velocity is the rate of change of position
	// Acceleration is the rate of change of velocity
	// p' = p + v*dt
		//	current position is the previous position plus the change in position multiplied by the amount of time passed since we last calculated position
	// v' = v + a*dt
	//vec3 position;
	vec3 velocity;
	vec3 acceleration;
	vec3 force;
	// You can add an option to limit the parameters above
	float mass;


	// Visual Properties
	float size;
	float sizeBegin;
	float sizeEnd;
	vec4 color;
	vec4 colorBegin;
	vec4 colorEnd;
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
	//virtual void draw(Camera &camera, Light *light, Light* spotLight, Matrix44& cameraInverse);

	void applyForceToParticle(unsigned int idx, vec3 force);

	unsigned int getNumParticles() { return m_pNumParticles; }

	vec3 getParticlePosition(unsigned int idx);
	float getParticleMass(unsigned int idx);

	// Creation properties
	bool velocityRandomValues;

	///// Playback properties
	bool playing;

	std::string texName;

	//// Update flags
	bool interpolateColor;

	///// Initial properties for newly spawned particles

	vec3 emitterPosition;
	float spawnDelay = 0.0f;

	// Storing ranges for each property so each particle has some randomness
	vec3 velocity0;
	vec3 velocity1;

	vec3 spawnRadius1;
	vec3 spawnRadius2;
	float range = 0.0f;

	// these properties are single floats, so we can pack the min and max into a vec2, just data!
	vec2 lifeRange;
	vec2 sizeRange;
	vec2 sizeEnd;
	vec2 sizeBegin;
	vec2 massRange;

	vec4 colorBegin0;
	vec4 colorBegin1;
	vec4 colorEnd0;
	vec4 colorEnd1;


	vec3 partLocalPos = vec3(5.0f, -7.5f, 0.0f);

	// ... other properties
	// ... what would be a better way of doing this?
	// Make a keyframe controller for each property! this gives you max control!!
	// (this is what full out particle editors do, ie popcorn fx)
};