#include "SkeletalMesh.h"

SkeletalMesh::SkeletalMesh()
{
	// Create new animator

	_rootJoint->calculateInverseBindTransform(mat4::Identity);
}

SkeletalMesh::~SkeletalMesh()
{
}

Joint * SkeletalMesh::getRootJoint() const
{
	return _rootJoint;
}

bool SkeletalMesh::loadFromFile(const string & file)
{
	return false;
}

void SkeletalMesh::uploadToGPU()
{
}

mat4 * SkeletalMesh::getJointTransforms()
{
	//vector<mat4> jointTransforms;
	//jointTransforms.reserve(_numOfJoints);

	mat4* jointTransforms = new mat4[_numOfJoints];
	getJointTransformsHelper(_rootJoint, jointTransforms);

	return jointTransforms;
}

void SkeletalMesh::getJointTransformsHelper(Joint * joint, mat4 * jointTransforms)
{
	jointTransforms[joint->getIndex()] = joint->getAnimatedTransform();

	for (Joint* child : joint->getChildren())
	{
		getJointTransformsHelper(child, jointTransforms);
	}
}
