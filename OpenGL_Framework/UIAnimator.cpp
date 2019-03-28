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
		cout << "Animation with name: " << name << " already exists!" << endl;
		system("pause");
	}
	_animations[name] = anim;
}

void UIAnimator::removeAnimation(const string & name)
{
	// Remove animation with name, iff one exists
	if (_animations.find(name) != _animations.end())
		_animations.erase(name);
	else
	{
		cout << "Animation with name: " << name << " does not exist!" << endl;
		system("pause");
	}

	// Stop playing the current animation if it is the one being removed
	if (_currentAnimation && (_currentAnimation->getName() == name))
	{
		stopCurrent();
	}
}

void UIAnimator::update(float deltaTime)
{
	if (_active)
	{
		// Only play animation set as the currentAnimation
		if (!_currentAnimation)
			return;

		// Increment animator time, and use to compute the current frame of the currentAnimation
		_currentTime += deltaTime;

		if (_currentTime > _currentAnimation->getDuration())
		{
			// Re-play the animation, iff it is set to loop
			if (_currentAnimation->getLoop())
			{
				//currentTime -= currentAnimation->getDuration();
				//_currentTime = 0.0f;
				play();
			}
			// Stop the current animation
			else
			{
				stopCurrent();
				return;
			}
		}

		getPrevNextFrames();
	}
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
		cerr << "Animation with name: <" << name << "> does not exists!" << endl;
	}
	
}

unordered_map<string, UIAnimation*> UIAnimator::getAnimations() const
{
	return _animations;
}

void UIAnimator::setImage(UIImage * image)
{
	_image = image;
}

float UIAnimator::getCurrentTime() const
{
	return _currentTime;
}

void UIAnimator::setCurrentTime(const float time)
{
	_currentTime = time;
}

void UIAnimator::play(const string & animName)
{
	_currentTime = 0.0f;

	// Play the animation with animName, iff one exists
	if (_animations.find(animName) != _animations.end())
	{
		UIAnimation* anim = _animations[animName];
		_animOrder.push(anim);
		_currentAnimation = anim;
	}
	else
		cerr << "Animation with name: <" << animName << "> does not exists!" << endl;
}

void UIAnimator::play()
{
	_currentTime = 0.0f;

	if (!_animOrder.empty())
	{
		_currentAnimation = _animOrder.top();

		// Check if the current animation has been removed from this animator
		if (_animations.find(_currentAnimation->getName()) == _animations.end())
		{
			_currentAnimation = nullptr;
			_animOrder.pop();
			play();
		}
	}
	else
		cerr << "Animator with name: <" << _image->getName() << "> contains no animations!" << endl;
}

void UIAnimator::stopCurrent()
{
	_currentTime = 0.0f;

	// Stop current animation
	if (!_animOrder.empty())
	{
		_animOrder.pop();

		// Play next animation in the stack, iff one exists
		if (!_animOrder.empty())
		{
			play();
		}
		else
			_currentAnimation = nullptr;
	}
	else
		cerr << "Animator with name: <" << _image->getName() << "> contains no animations!" << endl;
}

void UIAnimator::stopAll()
{
	_currentTime = 0.0f;

	_animOrder = stack<UIAnimation*>();

	_currentAnimation = nullptr;
}

stack<UIAnimation*> UIAnimator::getAnimOrder() const
{
	return _animOrder;
}

bool UIAnimator::getActive() const
{
	return _active;
}

void UIAnimator::setActive(const bool active)
{
	_active = active;
}

void UIAnimator::getPrevNextFrames()
{
	// Find the previous and next Keyframe of animation.
	vector<UIKeyFrame*> allFrames = _currentAnimation->getKeyFrames();
	
	// Don't try and compute current animation, if it has no key frames.
	if (allFrames.size() <= 0)
		return;


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
