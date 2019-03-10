#include "UIKeyFrame.h"

UIKeyFrame::UIKeyFrame(float _startTime, vec2 _pos, vec2 _scale, Quaternion _rot, float _alpha)
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

vec2 UIKeyFrame::getPos()
{
	return pos;
}

void UIKeyFrame::setPos(vec2 newPos)
{
	pos = newPos;
}

vec2 UIKeyFrame::getScale()
{
	return scale;
}

void UIKeyFrame::setscale(vec2 newScale)
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
