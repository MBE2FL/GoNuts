#include "Component.h"

CompType Component::getCompType() const
{
	return _compType;
}

void Component::setCompType(const CompType & compType)
{
	_compType = compType;
}

string Component::getName() const
{
	return _name;
}

void Component::setName(const string & name)
{
	_name = name;
}

int Component::getID() const
{
	return _id;
}

void Component::setID(const int id)
{
	_id = id;
}
