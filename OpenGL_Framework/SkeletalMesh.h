#pragma once

#include "Mesh.h"
#include "Joint.h"


class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh();
	~SkeletalMesh();

	Joint* getRootJoint() const;

	bool loadFromFile(const string& file);
	void uploadToGPU();
	//void setAnimation(Animation anim);
	//void update();
	mat4* getJointTransforms();

private:
	Joint* _rootJoint;
	unsigned int _numOfJoints;
	// Animator;


	void getJointTransformsHelper(Joint* joint, mat4* jointTransforms);
};