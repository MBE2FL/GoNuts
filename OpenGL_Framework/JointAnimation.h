#pragma once

#include "KeyFrame.h"
#include <vector>
#include <string>


using std::vector;
using std::string;


class JointAnimation
{
public:
	JointAnimation(const string& name);
	JointAnimation(const vector<KeyFrame*>& keyFrames);
	~JointAnimation();

	vector<KeyFrame*> getKeyFrames() const;
	void setKeyFrames(const vector<KeyFrame*>& keyFrames);
	string getName() const;
	void setName(const string& name);
	void addKeyFrame(KeyFrame* keyFrame);

private:
	vector<KeyFrame*> _keyFrames;
	string _name;
};