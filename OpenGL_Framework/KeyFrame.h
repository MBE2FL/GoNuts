#pragma once

#include "JointTransform.h"


class KeyFrame
{
public:
	KeyFrame();
	KeyFrame(const float startTime, JointTransform* jointTransform);
	~KeyFrame();

	float getStartTime() const;
	void setStartTime(const float startTime);
	//unordered_map<string, JointTransform*> getJointKeyFrames() const;
	JointTransform* getJointTransform() const;
	void setJointTransform(JointTransform* jointTransform);

private:
	float _startTime;
	//unordered_map<string, JointTransform*> _jointKeyFrames;
	JointTransform* _jointTransform;
};