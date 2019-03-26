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


using std::vector;
using std::unordered_map;
using std::cerr;
using std::endl;
using std::ifstream;
using std::stringstream;



class UIAnimation
{
public:
	UIAnimation(const string& name, const vector<UIKeyFrame*>& keyFrames);
	UIAnimation(const string& name);

	float getDuration() const;
	string getName() const;

	vector<UIKeyFrame*> getKeyFrames() const;
	void setKeyframes(const vector<UIKeyFrame*>& keyFrames);

	static void loadAllAnimsInFile(const string& path);
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