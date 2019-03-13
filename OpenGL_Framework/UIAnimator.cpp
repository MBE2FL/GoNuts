#include "UIAnimator.h"
#include "UIImage.h"

UIAnimator::UIAnimator(UIImage* _image)
{
	image = _image;
}

void UIAnimator::addAnimation(UIAnimation * anim)
{
	string name = anim->getName();
	if (animations.find(name) != animations.end())
	{
		cout << "animation with name: " << name << " already exists" << endl;
		system("pause");
	}
	animations[name] = anim;
	currentAnimation = animations[name];
}

void UIAnimator::update(float deltaTime)
{
	if (!currentAnimation)
		return;

	currentTime += deltaTime;

	if (currentTime > currentAnimation->getDuration())
	{
		//currentTime -= currentAnimation->getDuration();
		currentTime = 0.0f;
	}

	getPrevNextFrames();

}

void UIAnimator::getPrevNextFrames()
{
	// Find the previous and next Keyframe of animation.
	vector<UIKeyFrame*> allFrames = currentAnimation->getKeyFrames();

		UIKeyFrame* prevFrame = allFrames[0];
		UIKeyFrame* nextFrame = allFrames[0];

		// Find the frames before and after the current animation time.
		for (size_t i = 1; i < allFrames.size(); ++i)
		{
			nextFrame = allFrames[i];
			if (nextFrame->getStartTime() > currentTime)
				break;

			prevFrame = allFrames[i];
		}

		// Blend between the previous and next frame of animation.
		float interValue = invLerp(currentTime, prevFrame->getStartTime(), nextFrame->getStartTime());

		image->setLocalPosition(lerp(prevFrame->getPos(), nextFrame->getPos(), interValue));
		image->setScale(lerp(prevFrame->getScale(), nextFrame->getScale(), interValue));
		//image->setLocalRotation(lerp(prevFrame->getRot(), nextFrame->getRot(), interValue));
		image->setAlpha(lerp(prevFrame->getAlpha(), nextFrame->getAlpha(), interValue));
}
