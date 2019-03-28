#pragma once
#include "MathLib/MathLibCore.h"

class UIKeyFrame
{
public:
	UIKeyFrame(float _startTime = 0.0f, vec3 _pos = vec3::Zero, vec3 _scale = vec3::One, Quaternion _rot = Quaternion::Identity, float _alpha = 1.0f);

	float getStartTime();
	void setStartTIme(float newTime);

	vec3 getPos();
	void setPos(vec3 newPos);

	vec3 getScale();
	void setscale(vec3 newScale);

	Quaternion getRot();
	void setRot(Quaternion newRot);

	float getAlpha();
	void setAlpha(float newAlpha);

private:
	float startTime;
	vec3 pos;
	vec3 scale;
	Quaternion rot;
	float alpha;
};