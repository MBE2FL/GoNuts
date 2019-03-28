#pragma once
#include "UIKeyFrame.h"
#include "MathLib/MathLibCore.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "dirent.h"
#include <sys/types.h>
#include <algorithm>
#include <iomanip>


using std::vector;
using std::unordered_map;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::sort;
using std::find;
using std::fixed;
using std::setprecision;


#define UI_ANIM_DIR "./Assets/UI Animations/"


class UIAnimation
{
public:
	UIAnimation(const string& name, const vector<UIKeyFrame*>& keyFrames);
	UIAnimation(const string& name);

	float getDuration() const;
	string getName() const;

	vector<UIKeyFrame*> getKeyFrames() const;
	void setKeyframes(const vector<UIKeyFrame*>& keyFrames);
	void addKeyFrame(UIKeyFrame* keyFrame);
	void removeKeyFrames();
	void removeKeyFrame(UIKeyFrame* keyFrame);

	static void loadAllAnimsInFile();
	static void saveAnim(UIAnimation* anim);
	static UIAnimation* getAnimation(const string& name);
	static unordered_map<string, UIAnimation*> getAllAnimations();
	static void addAnimation(UIAnimation* animation);
	bool getLoop() const;
	void setLoop(const bool loop);

private:
	float _duration;
	string _name;
	vector<UIKeyFrame*> _keyFrames;
	bool _loop = false;
	
	static unordered_map<string, UIAnimation*> _animations;
	static unordered_map<string, string> _filePaths;


	static void findLine(string& line, ifstream& file, const string& word);
	static void getNextLine(string& line, ifstream& file);
};