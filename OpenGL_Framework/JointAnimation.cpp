#include "JointAnimation.h"

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

string JointAnimation::getName() const
{
	return _name;
}

void JointAnimation::setName(const string & name)
{
	_name = name;
}
