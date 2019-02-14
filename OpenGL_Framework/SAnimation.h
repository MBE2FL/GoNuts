#pragma once

#include "KeyFrame.h"
#include <vector>

using std::vector;

class SAnimation
{
public:
	SAnimation(const float duration, const vector<KeyFrame*>& keyFrames);
	~SAnimation();

	float getDuration() const;
	vector<KeyFrame*> getKeyFrames() const;

private:
	float _duration;
	vector<KeyFrame*> _keyFrames;
};