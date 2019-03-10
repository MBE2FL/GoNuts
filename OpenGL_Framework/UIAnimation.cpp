#include "UIAnimation.h"

UIAnimation::UIAnimation(float _duration,string _name, vector<UIKeyFrame*> _keyFrames)
{
	duration = _duration;
	name = _name;
	keyFrames = _keyFrames;
}

float UIAnimation::getDuration()
{
	return duration;
}

string UIAnimation::getName()
{
	return name;
}

vector<UIKeyFrame*> UIAnimation::getKeyFrames()
{
	return keyFrames;
}
