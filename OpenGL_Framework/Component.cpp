#include "Component.h"

Component::Component()
{
}

Component::~Component()
{
}

ComponentType Component::getType() const
{
	return _type;
}

void Component::setEntity(Entity * entity)
{
	_entity = entity;
}

Entity * Component::getEntity() const
{
	return _entity;
}
