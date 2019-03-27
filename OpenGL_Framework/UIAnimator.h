#pragma once
#include "MathLib/MathLibCore.h"
#include"UIAnimation.h"
#include <unordered_map>
#include <iostream>
#include <stack>

using MathUtils::invLerp;
using MathUtils::lerp;
using std::unordered_map;
using std::cout;
using std::endl;
using std::stack;

class UIImage;

class UIAnimator
{
public:
	UIAnimator(UIImage* image);
	UIAnimator();

	void addAnimation(UIAnimation* anim);
	void removeAnimation(const string& name);
	void update(float deltaTime);

	UIAnimation* getCurrentAnimation() const;
	void setCurrentAnimation(const string& name);
	unordered_map<string, UIAnimation*> getAnimations() const;
	void setImage(UIImage* image);
	float getCurrentTime() const;
	void setCurrentTime(const float time);


	void play(const string& animName);
	void play();
	void stopCurrent();
	void stopAll();

	stack<UIAnimation*> getAnimOrder() const;

	bool getActive() const;
	void setActive(const bool active);
	

private:
	UIImage* _image;
	UIAnimation* _currentAnimation;
	float _currentTime = 0.0f;
	unordered_map<string, UIAnimation*> _animations;
	stack<UIAnimation*> _animOrder;
	bool _active = false;

	void getPrevNextFrames();
};
