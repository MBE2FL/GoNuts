#include "UIAnimator.h"

UIAnimator::UIAnimator()
{

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
}

void UIAnimator::update(float deltaTime)
{
	if (!currentAnimation)
		return;

	currentTime += deltaTime;

	if (currentTime > currentAnimation->getDuration())
	{
		currentTime -= currentAnimation->getDuration();
	}


}
