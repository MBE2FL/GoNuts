#include "Joint.h"

Joint::Joint(unsigned int index, const string & name, mat4 & localBindTransform)
{
	_index = index;
	_name = name;
	_localBindTransform = localBindTransform;
}

Joint::~Joint()
{
	for (Joint* child : _children)
	{
		delete child;
		child = nullptr;
	}
}

unsigned int Joint::getIndex() const
{
	return _index;
}

void Joint::setIndex(const unsigned int index)
{
	_index = index;
}

string Joint::getName() const
{
	return _name;
}

void Joint::setName(const string & name)
{
	_name = name;
}

vector<Joint*> Joint::getChildren() const
{
	return _children;
}

mat4 Joint::getAnimatedTransform() const
{
	return _animatedTransform;
}

mat4 Joint::getInverseBindTransfrom() const
{
	return _inverseBindTransform;
}

void Joint::addChild(Joint * child)
{
	_children.push_back(child);
}

void Joint::calculateInverseBindTransform(const mat4 & parentBindTransform)
{
	mat4 bindTransform = parentBindTransform * _localBindTransform;
	_inverseBindTransform = bindTransform.getInverse();

	for (Joint* child : _children)
	{
		child->calculateInverseBindTransform(bindTransform);
	}
}
