#include "JointAnimation.h"

JointAnimation::JointAnimation(const string & name)
{
	_name = name;
}

JointAnimation::JointAnimation(const vector<KeyFrame*>& keyFrames)
{
	_keyFrames = keyFrames;
}

JointAnimation::~JointAnimation()
{
}

vector<KeyFrame*> JointAnimation::getKeyFrames() const
{
	return _keyFrames;
}

void JointAnimation::setKeyFrames(const vector<KeyFrame*>& keyFrames)
{
	_keyFrames = keyFrames;
}

string JointAnimation::getName() const
{
	return _name;
}

void JointAnimation::setName(const string & name)
{
	_name = name;
}

void JointAnimation::addKeyFrame(KeyFrame * keyFrame)
{
	_keyFrames.push_back(keyFrame);
}
