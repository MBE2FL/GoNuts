#pragma once

#include <vector>
#include "Entity.h"

using std::vector;


enum class ComponentType
{
	Transform,
	MeshRenderer,
	PhysicsBody,
	Camera,
	Light,
	Collider
};


class Component
{
public:
	Component();
	virtual ~Component();

	ComponentType getType() const;
	void setEntity(Entity* entity);
	Entity* getEntity() const;

protected:
	ComponentType _type;
	Entity* _entity;
};