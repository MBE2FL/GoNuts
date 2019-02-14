#pragma once

#include <vector>
#include <string>
#include "MathLib/MathLibCore.h"

using std::vector;
using std::string;

class Joint
{
public:
	Joint(unsigned int index, const string& name, mat4& localBindTransform);
	~Joint();

	unsigned int getIndex() const;
	void setIndex(const unsigned int index);
	string getName() const;
	void setName(const string& name);
	vector<Joint*> getChildren() const;
	mat4 getAnimatedTransform() const;
	void setAnimatedTransform(const mat4& transform);
	mat4 getInverseBindTransfrom() const;

	void addChild(Joint* child);
	void calculateInverseBindTransform(const mat4& parentBindTransform);

private:
	unsigned int _index;
	string _name;
	vector<Joint*> _children;

	mat4 _animatedTransform;
	mat4 _localBindTransform;
	mat4 _inverseBindTransform;
};