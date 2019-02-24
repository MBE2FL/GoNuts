#pragma once

#include "KeyFrame.h"
#include <vector>
#include <string>


using std::vector;
using std::string;


class JointAnimation
{
public:
	JointAnimation(const vector<KeyFrame*>& keyFrames);
	~JointAnimation();

	vector<KeyFrame*> getKeyFrames() const;
	string getName() const;
	void setName(const string& name);

private:
	vector<KeyFrame*> _keyFrames;
	string _name;
};