#include "Entity.h"

unsigned int Entity::eidCounter = 0;

Entity::Entity()
{
	_eid = eidCounter;
	eidCounter++;
}

Entity::~Entity()
{
}

unsigned int Entity::getEid() const
{
	return _eid;
}

void Entity::resetEids()
{
	eidCounter = 0;
}
