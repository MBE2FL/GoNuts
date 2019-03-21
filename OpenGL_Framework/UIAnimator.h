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

class UIImage;

class UIAnimator
{
public:
	UIAnimator(UIImage* image);
	UIAnimator();

	void addAnimation(UIAnimation* anim);
	void update(float deltaTime);

	UIAnimation* getCurrentAnimation() const;
	void setCurrentAnimation(const string& name);
	unordered_map<string, UIAnimation*> getAnimations() const;
	

private:
	UIImage* _image;
	UIAnimation* _currentAnimation;
	float _currentTime = 0.0f;
	unordered_map<string, UIAnimation*> _animations;

	void getPrevNextFrames();
};
