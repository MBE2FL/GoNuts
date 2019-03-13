#include "UIKeyFrame.h"

UIKeyFrame::UIKeyFrame(float _startTime, vec3 _pos, vec3 _scale, Quaternion _rot, float _alpha)
{
	startTime = _startTime;
	pos = _pos;
	scale = _scale;
	rot = _rot;
	alpha = _alpha;
}

float UIKeyFrame::getStartTime()
{
	return startTime;
}

void UIKeyFrame::setStartTIme(float newTime)
{
	startTime = newTime;
}

vec3 UIKeyFrame::getPos()
{
	return pos;
}

void UIKeyFrame::setPos(vec3 newPos)
{
	pos = newPos;
}

vec3 UIKeyFrame::getScale()
{
	return scale;
}

void UIKeyFrame::setscale(vec3 newScale)
{
	scale = newScale;
}

Quaternion UIKeyFrame::getRot()
{
	return rot;
}

void UIKeyFrame::setRot(Quaternion newRot)
{
	rot = newRot;
}

float UIKeyFrame::getAlpha()
{
	return alpha;
}

void UIKeyFrame::setAlpha(float newAlpha)
{
	alpha = newAlpha;
}
