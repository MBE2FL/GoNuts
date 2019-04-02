#pragma once

#include "ParticleEffect.h"

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	void addEffect(ParticleEffect* effect);
	void deleteEffect(const string& name);
	unordered_map<string, ParticleEffect*> getEffects() const;
	void update(float deltaTime);
	void draw(mat4 & camView, mat4 & camProj);

private:
	unordered_map<string, ParticleEffect*> _effects;
};