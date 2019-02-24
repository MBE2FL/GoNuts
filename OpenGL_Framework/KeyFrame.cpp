#include "KeyFrame.h"

KeyFrame::KeyFrame()
{
	_startTime = 0.0f;
	_jointTransform = nullptr;
}

KeyFrame::KeyFrame(const float startTime, JointTransform* jointTransform)
{
	_startTime = startTime;
	_jointTransform = jointTransform;
}

KeyFrame::~KeyFrame()
{
}

float KeyFrame::getStartTime() const
{
	return _startTime;
}

void KeyFrame::setStartTime(const float startTime)
{
	_startTime = startTime;
}

JointTransform* KeyFrame::getJointTransform() const
{
	return _jointTransform;
}

void KeyFrame::setJointTransform(JointTransform * jointTransform)
{
	_jointTransform = jointTransform;
}
