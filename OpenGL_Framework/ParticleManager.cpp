#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
	for (auto const& effectKV : _effects)
	{
		delete effectKV.second;
	}

	_effects.clear();
}

void ParticleManager::addEffect(ParticleEffect * effect)
{
	string name = effect->getName();

	if (_effects.find(name) != _effects.end())
		cerr << "Particle effect with name <" + name + "> already exists in particle manager!" << endl;
	else
		_effects[name] = effect;
}

void ParticleManager::deleteEffect(const string & name)
{
	unordered_map<string, ParticleEffect*>::iterator it = _effects.find(name);
	if (it != _effects.end())
	{
		delete it->second;
		_effects.erase(name);
	}
}

unordered_map<string, ParticleEffect*> ParticleManager::getEffects() const
{
	return _effects;
}

void ParticleManager::update(float deltaTime)
{
	for (auto const& effectKV : _effects)
	{
		effectKV.second->update(deltaTime);
	}
}

void ParticleManager::draw(mat4 & camView, mat4 & camProj)
{
	for (auto const& effectKV : _effects)
	{
		effectKV.second->draw(camView, camProj);
	}
}
