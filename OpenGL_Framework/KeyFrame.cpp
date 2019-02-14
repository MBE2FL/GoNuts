#include "KeyFrame.h"

KeyFrame::KeyFrame(const float startTime, const unordered_map<string, JointTransform*> jointKeyFrames)
{
	_startTime = startTime;
	_jointKeyFrames = jointKeyFrames;
}

KeyFrame::~KeyFrame()
{
}

float KeyFrame::getStartTime() const
{
	return _startTime;
}

unordered_map<string, JointTransform*> KeyFrame::getJointKeyFrames() const
{
	return _jointKeyFrames;
}
