#pragma once

#include "Mesh.h"
#include "Animator.h"


class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh();
	~SkeletalMesh();

	Joint* getRootJoint() const;

	bool loadFromFile(const string& file);
	void uploadToGPU();
	void setAnimation(SAnimation* anim);
	void update(float deltaTime);
	mat4* getJointTransforms();

private:
	Joint* _rootJoint;
	unsigned int _numOfJoints;
	Animator* _animator;


	void getJointTransformsHelper(Joint* joint, mat4* jointTransforms);
};