#include "UIAnimator.h"
#include "UIImage.h"

UIAnimator::UIAnimator(UIImage* image)
{
	_image = image;
}

UIAnimator::UIAnimator()
{
	_image = nullptr;
}

void UIAnimator::addAnimation(UIAnimation * anim)
{
	string name = anim->getName();
	if (_animations.find(name) != _animations.end())
	{
		cout << "animation with name: " << name << " already exists" << endl;
		system("pause");
	}
	_animations[name] = anim;
	_currentAnimation = _animations[name];
}

void UIAnimator::update(float deltaTime)
{
	if (!_currentAnimation)
		return;

	_currentTime += deltaTime;

	if (_currentTime > _currentAnimation->getDuration())
	{
		//currentTime -= currentAnimation->getDuration();
		_currentTime = 0.0f;
	}

	getPrevNextFrames();

}

UIAnimation * UIAnimator::getCurrentAnimation() const
{
	return _currentAnimation;
}

void UIAnimator::setCurrentAnimation(const string & name)
{
	if (_animations.find(name) != _animations.end())
	{
		_currentAnimation = _animations[name];
	}
	else
	{
		cerr << "Animation with name: " << name << " does not exists!" << endl;
	}
	
}

unordered_map<string, UIAnimation*> UIAnimator::getAnimations() const
{
	return _animations;
}

void UIAnimator::getPrevNextFrames()
{
	// Find the previous and next Keyframe of animation.
	vector<UIKeyFrame*> allFrames = _currentAnimation->getKeyFrames();

		UIKeyFrame* prevFrame = allFrames[0];
		UIKeyFrame* nextFrame = allFrames[0];

		// Find the frames before and after the current animation time.
		for (size_t i = 1; i < allFrames.size(); ++i)
		{
			nextFrame = allFrames[i];
			if (nextFrame->getStartTime() > _currentTime)
				break;

			prevFrame = allFrames[i];
		}

		// Blend between the previous and next frame of animation.
		float interValue = invLerp(_currentTime, prevFrame->getStartTime(), nextFrame->getStartTime());

		_image->setLocalPosition(lerp(prevFrame->getPos(), nextFrame->getPos(), interValue));
		_image->setScale(lerp(prevFrame->getScale(), nextFrame->getScale(), interValue));
		//image->setLocalRotation(lerp(prevFrame->getRot(), nextFrame->getRot(), interValue));
		_image->setAlpha(lerp(prevFrame->getAlpha(), nextFrame->getAlpha(), interValue));
}
