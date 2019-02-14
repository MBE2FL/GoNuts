#include "SAnimation.h"

SAnimation::SAnimation(const float duration, const vector<KeyFrame*> & keyFrames)
{
	_duration = duration;
	_keyFrames = keyFrames;
}

SAnimation::~SAnimation()
{
}

float SAnimation::getDuration() const
{
	return _duration;
}

vector<KeyFrame*> SAnimation::getKeyFrames() const
{
	return _keyFrames;
}
