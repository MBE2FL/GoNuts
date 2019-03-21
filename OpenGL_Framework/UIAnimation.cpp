#include "UIAnimation.h"

unordered_map<string, UIAnimation*> UIAnimation::_animations = unordered_map<string, UIAnimation*>();
unordered_map<string, string> UIAnimation::_filePaths = unordered_map<string, string>();

UIAnimation::UIAnimation(const string & name, vector<UIKeyFrame*> & keyFrames)
{
	_name = name;
	_keyFrames = keyFrames;
	_duration = keyFrames.back()->getStartTime();
}

UIAnimation::UIAnimation(const string & name)
{
	if (_animations.find(name) != _animations.end())
	{
		UIAnimation* anim = _animations[name];
		_name = anim->getName();
		_duration = anim->getDuration();
		_keyFrames = anim->getKeyFrames();
	}
	else
	{
		cerr << "UI Animation with name: " << name << " could not be found!" << endl;
		system("pause");
	}
}

float UIAnimation::getDuration() const
{
	return _duration;
}

string UIAnimation::getName() const
{
	return _name;
}

vector<UIKeyFrame*> UIAnimation::getKeyFrames() const
{
	return _keyFrames;
}

void UIAnimation::loadAllAnimsInFile(const string & path)
{
	// TO-DO
}

UIAnimation * UIAnimation::getAnimation(const string & name)
{
	if (_animations.find(name) != _animations.end())
	{
		return _animations[name];
	}
	else
	{
		cerr << "UI Animation with name: " << name << " could not be found!" << endl;
		system("pause");
	}
}
