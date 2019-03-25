#include "UIAnimation.h"

unordered_map<string, UIAnimation*> UIAnimation::_animations = unordered_map<string, UIAnimation*>();
unordered_map<string, string> UIAnimation::_filePaths = unordered_map<string, string>();

UIAnimation::UIAnimation(const string & name, const vector<UIKeyFrame*> & keyFrames)
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

void UIAnimation::setKeyframes(const vector<UIKeyFrame*>& keyFrames)
{
	_keyFrames = keyFrames;
}

void UIAnimation::loadAllAnimsInFile(const string & path)
{
	// Get list of all animation files in the specified path
	// Open the animation directory
	DIR* dir = opendir(path.c_str());
	struct dirent *entry;

	// Make sure the animation directory exists
	if (!dir)
	{
		cerr << "UI Animation path: <" << path << "> does not exist!" << endl;
		system("pause");
		return;
	}

	
	// Open each animation file inside the animation directory
	while ((entry = readdir(dir)) != nullptr)
	{
		// Skip "." and ".." entries. (i.e "." directory entry for current directory entry, and ".." directory entry one level up in the hierarchy).
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		// Load in UI animation file.
		ifstream file;
		file.open(path + entry->d_name);

		if (!file.is_open())
		{
			cerr << "Could not open UI animation file with name: <" << entry->d_name << ">!" << endl;
			system("pause");
			continue;
		}

		string line;


		// Find the name section of the current animation.
		findLine(line, file, "Name");

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a name section!" << endl;
			system("pause");
			return;
		}

		getNextLine(line, file);

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a name!" << endl;
			system("pause");
			return;
		}

		string animName = line;
		UIAnimation* animation = nullptr;
		vector<UIKeyFrame*> keyFrames;


		// Find the loop section of the current animation.
		findLine(line, file, "Loop");

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a loop section!" << endl;
			system("pause");
			return;
		}

		getNextLine(line, file);

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a loop!" << endl;
			system("pause");
			return;
		}

		bool loop = stoi(line);


		// Find the key frame section of the current animation.
		findLine(line, file, "KeyFrame");

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a key frame section!" << endl;
			system("pause");
			return;
		}

		getNextLine(line, file);

		if (line.empty())
		{
			cerr << "UI animation file file does not contain any key frames!" << endl;
			system("pause");
			return;
		}

		// Read until the end of the key frame section has been reached
		while (line != "end")
		{
			stringstream ss(line);
			string word;
			size_t wordIndex = 0;
			UIKeyFrame* keyFrame = nullptr;
			vec3 position;
			vec3 scale;
			Quaternion rotation;

			// Extract each key frame's information
			while (std::getline(ss, word, ' '))
			{
				// Skip white spaces
				if (word == "")
					continue;

				// Found time
				if (wordIndex == 0)
				{
					keyFrame = new UIKeyFrame(stof(word));

					++wordIndex;
				}
				// Found pos.X
				else if (wordIndex == 1)
				{
					position.x = stof(word);

					++wordIndex;
				}
				// Found pos.Y
				else if (wordIndex == 2)
				{
					position.y = stof(word);

					++wordIndex;
				}
				// Found pos.Z
				else if (wordIndex == 3)
				{
					position.z = stof(word);

					++wordIndex;
				}
				// Found scale.X
				else if (wordIndex == 4)
				{
					scale.x = stof(word);

					++wordIndex;
				}
				// Found scale.Y
				else if (wordIndex == 5)
				{
					scale.y = stof(word);

					++wordIndex;
				}
				// Found scale.Z
				else if (wordIndex == 6)
				{
					scale.z = stof(word);

					++wordIndex;
				}
				// Found rot.W
				else if (wordIndex == 7)
				{
					rotation.setW(stof(word));

					++wordIndex;
				}
				// Found rot.X
				else if (wordIndex == 8)
				{
					rotation.setX(stof(word));

					++wordIndex;
				}
				// Found rot.Y
				else if (wordIndex == 9)
				{
					rotation.setY(stof(word));

					++wordIndex;
				}
				// Found rot.Z
				else if (wordIndex == 10)
				{
					rotation.setZ(stof(word));

					++wordIndex;
				}
				// Found alpha
				else if (wordIndex == 11)
				{
					keyFrame->setAlpha(stof(word));
					keyFrame->setPos(position);
					keyFrame->setRot(rotation);
					keyFrame->setscale(scale);

					keyFrames.push_back(keyFrame);

					++wordIndex;
				}
				else
				{
					cerr << "Found unknown element in UI keyframe!" << endl;
					system("pause");
				}
			}

			getNextLine(line, file);
		}

		// Add all the key frames to an animation, and add the animation to the unordered map of all animations
		animation = new UIAnimation(animName, keyFrames);
		animation->setLoop(loop);
		UIAnimation::addAnimation(animation);
	}

	// Close the animation directory
	closedir(dir);
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
		return nullptr;
	}
}

unordered_map<string, UIAnimation*> UIAnimation::getAllAnimations()
{
	return _animations;
}

void UIAnimation::addAnimation(UIAnimation * animation)
{
	string name = animation->getName();

	if (_animations.find(name) != _animations.end())
	{
		cerr << "UI Animation with name: <" << name << "> already exists!" << endl;
		system("pause");
	}

	_animations[name] = animation;
}

bool UIAnimation::getLoop() const
{
	return _loop;
}

void UIAnimation::setLoop(const bool loop)
{
	_loop = loop;
}

void UIAnimation::findLine(string & line, ifstream & file, const string & word)
{
	while (line.find(word) == string::npos)
	{
		// Search file for a line containing the specified word.
		if (!file.eof())
			std::getline(file, line);
		// File does not contain a line with the specified word.
		else
		{
			line = "";
			break;
		}
	}
}

void UIAnimation::getNextLine(string & line, ifstream & file)
{
	std::getline(file, line);
}
