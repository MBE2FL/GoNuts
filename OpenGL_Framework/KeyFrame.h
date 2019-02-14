#pragma once

#include "JointTransform.h"
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

class KeyFrame
{
public:
	KeyFrame(const float startTime, const unordered_map<string, JointTransform*> jointKeyFrames);
	~KeyFrame();

	float getStartTime() const;
	unordered_map<string, JointTransform*> getJointKeyFrames() const;

private:
	float _startTime;
	unordered_map<string, JointTransform*> _jointKeyFrames;
};