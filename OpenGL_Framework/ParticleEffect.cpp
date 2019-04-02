#include "ParticleEffect.h"

ParticleEffect::ParticleEffect(const string & name)
{
	_name = name;
}

ParticleEffect::~ParticleEffect()
{
	for (auto const& systemKV : _systems)
	{
		delete systemKV.second;
	}

	_systems.clear();
}

void ParticleEffect::update(float deltaTime)
{
	if (_active)
	{
		for (auto const& systemKV : _systems)
		{
			systemKV.second->update(deltaTime);
		}
	}
}

void ParticleEffect::draw(mat4 & camView, mat4 & camProj)
{
	for (auto const& systemKV : _systems)
	{
		systemKV.second->draw(camView, camProj);
	}
}

void ParticleEffect::addSystem(ParticleSystem * system)
{
	string name = system->getName();

	if (_systems.find(name) != _systems.end())
		cerr << "Particle system with name <" + name + "> already exists in particle effect: " + _name << "!" << endl;
	else
		_systems[name] = system;
}

void ParticleEffect::deleteSystem(const string & name)
{
	unordered_map<string, ParticleSystem*>::iterator it = _systems.find(name);
	if (it != _systems.end())
	{
		delete it->second;
		_systems.erase(name);
	}
}

string ParticleEffect::getName() const
{
	return _name;
}

bool ParticleEffect::isActive() const
{
	return _active;
}

void ParticleEffect::setActive(const bool active)
{
	_active = active;
}

unordered_map<string, ParticleSystem*> ParticleEffect::getSystems() const
{
	return _systems;
}
