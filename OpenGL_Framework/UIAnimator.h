#pragma once
#include "MathLib/MathLibCore.h"
#include"UIAnimation.h"
#include <unordered_map>
#include <iostream>

using MathUtils::invLerp;
using MathUtils::lerp;
using std::unordered_map;
using std::cout;
using std::endl;

class UIAnimator
{
public:
	UIAnimator();

	void addAnimation(UIAnimation* anim);
	void update(float deltaTime);


private:
	UIAnimation* currentAnimation;
	UIKeyFrame* currentKeyFrame;
	float currentTime = 0.0f;
	unordered_map<string, UIAnimation*> animations;

	void calculateCurrentPose();
	void getPrevNextFrames();
};
