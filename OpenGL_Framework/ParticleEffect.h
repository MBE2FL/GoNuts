#pragma once

#include "ParticleSystem.h"

class ParticleEffect
{
public:
	ParticleEffect(const string& name);
	~ParticleEffect();

	void update(float deltaTime);
	void draw(mat4 & camView, mat4 & camProj);
	void addToDrawList(vector<Particle*>& drawList) const;
	void addSystem(ParticleSystem* system);
	void deleteSystem(const string& name);
	string getName() const;
	bool isActive() const;
	void setActive(const bool active);
	unordered_map<string, ParticleSystem*> getSystems() const;
	size_t getNumParticles() const;

private:
	unordered_map<string, ParticleSystem*> _systems;
	bool _active = false;
	string _name;
};