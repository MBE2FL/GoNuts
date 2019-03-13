#pragma once
#include "UIKeyFrame.h"
#include "MathLib/MathLibCore.h"
#include <vector>

using std::vector;

class UIAnimation
{
public:
	UIAnimation(string _name, vector<UIKeyFrame*> _keyFrames);

	float getDuration();
	string getName();

	vector<UIKeyFrame*> getKeyFrames();
private:
	float duration;
	string name;
	vector<UIKeyFrame*> keyFrames;
};