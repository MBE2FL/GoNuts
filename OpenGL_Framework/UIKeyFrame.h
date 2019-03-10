#pragma once
#include "MathLib/MathLibCore.h"

class UIKeyFrame
{
public:
	UIKeyFrame(float _startTime, vec2 _pos, vec2 _scale, Quaternion _rot, float _alpha);

	float getStartTime();
	void setStartTIme(float newTime);

	vec2 getPos();
	void setPos(vec2 newPos);

	vec2 getScale();
	void setscale(vec2 newScale);

	Quaternion getRot();
	void setRot(Quaternion newRot);

	float getAlpha();
	void setAlpha(float newAlpha);

private:
	float startTime;
	vec2 pos;
	vec2 scale;
	Quaternion rot;
	float alpha;
};