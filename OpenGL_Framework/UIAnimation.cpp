#include "UIAnimation.h"

UIAnimation::UIAnimation(string _name, vector<UIKeyFrame*> _keyFrames)
{
	name = _name;
	keyFrames = _keyFrames;
	duration = _keyFrames.back()->getStartTime();
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
