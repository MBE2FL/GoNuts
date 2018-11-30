#include "Component.h"

Component::Component()
{
	setCompType(CompType::Transform);
}

Component::Component(CompType compType)
{
	setCompType(compType);
}

Component::~Component()
{
}

CompType Component::getCompType() const
{
	return _compType;
}

void Component::setCompType(const CompType & compType)
{
	_compType = compType;
}

int Component::getID() const
{
	return _id;
}

void Component::setID(const int id)
{
	_id = id;
}
