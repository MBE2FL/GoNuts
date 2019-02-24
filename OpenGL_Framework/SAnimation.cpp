#include "SAnimation.h"

SAnimation::SAnimation(const float duration, const vector<JointAnimation*> & jointAnims)
{
	_duration = duration;
	_jointAnims = jointAnims;
}

SAnimation::~SAnimation()
{
}

float SAnimation::getDuration() const
{
	return _duration;
}

vector<JointAnimation*> SAnimation::getJointAnims() const
{
	return _jointAnims;
}
