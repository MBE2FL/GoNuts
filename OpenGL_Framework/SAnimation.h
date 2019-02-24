#pragma once

#include "JointAnimation.h"

class SAnimation
{
public:
	SAnimation(const float duration, const vector<JointAnimation*>& jointAnims);
	~SAnimation();

	float getDuration() const;
	vector<JointAnimation*> getJointAnims() const;

private:
	float _duration;
	vector<JointAnimation*> _jointAnims;
};