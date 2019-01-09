#pragma once

#include <vector>

using std::vector;


enum class ComponentType
{
	Transform,
	MeshRenderer,
	PhysicsBody,
	Camera,
};


class Component
{
public:
	Component();
	virtual ~Component();

	ComponentType getType() const;

protected:
	ComponentType _type;
};