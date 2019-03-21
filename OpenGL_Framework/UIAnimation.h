#pragma once
#include "UIKeyFrame.h"
#include "MathLib/MathLibCore.h"
#include <vector>
#include <unordered_map>
#include <iostream>

using std::vector;
using std::unordered_map;
using std::cerr;
using std::endl;

class UIAnimation
{
public:
	UIAnimation(const string& name, vector<UIKeyFrame*>& keyFrames);
	UIAnimation(const string& name);

	float getDuration() const;
	string getName() const;

	vector<UIKeyFrame*> getKeyFrames() const;

	static void loadAllAnimsInFile(const string& path);
	static UIAnimation* getAnimation(const string& name);
private:
	float _duration;
	string _name;
	vector<UIKeyFrame*> _keyFrames;
	
	static unordered_map<string, UIAnimation*> _animations;
	static unordered_map<string, string> _filePaths;
};