#pragma once

#include <unordered_map>
#include "MathLib/MathLibCore.h"
#include "SAnimation.h"
#include "Joint.h"

using MathUtils::invLerp;
using std::unordered_map;

class SkeletalMesh;

class Animator
{
public:
	Animator(SkeletalMesh* mesh);
	~Animator();

	void setAnimation(SAnimation* animation);
	void update(float deltaTime);


    // DEBUG
	void nextFrame();

private:
	SkeletalMesh* _mesh;
	SAnimation* _animation;
	float _currentTime = 0.0f;

	//KeyFrame* _prevFrame;
	//KeyFrame* _nextFrame;
	unsigned int frameNum = 0;

	unordered_map<string, mat4> _currentPose;

	void calculateCurrentPose();
	void getPrevNextFrames();
	void blendPoses(KeyFrame* prevFrame, KeyFrame* nextFrame, float interValue, const string& jointName);
	void applyPoseToJoints(Joint* joint, const mat4& parentTransform);
};