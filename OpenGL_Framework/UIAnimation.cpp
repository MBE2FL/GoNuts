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
		//cerr << "UI Animation with name: " << name << " could not be found!" << endl;
		_name = name;
		_duration = 0.0f;
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

void UIAnimation::addKeyFrame(UIKeyFrame * keyFrame)
{
	_keyFrames.push_back(keyFrame);

	// Increase duration, if necessary.
	if (keyFrame->getStartTime() > _duration)
		_duration = keyFrame->getStartTime();

	// Sort the key frames, according to start times (Ascending order).
	sort(_keyFrames.begin(), _keyFrames.end(), [](UIKeyFrame* a, UIKeyFrame* b) -> bool
	{
		return a->getStartTime() < b->getStartTime();
	});
}

void UIAnimation::removeKeyFrames()
{
	_duration = 0.0f;
	_keyFrames.clear();
}

void UIAnimation::removeKeyFrame(UIKeyFrame * keyFrame)
{
	// Check if the key frame exists in this animation
	vector<UIKeyFrame*>::iterator it = find(_keyFrames.begin(), _keyFrames.end(), keyFrame);
	if (it != _keyFrames.end())
	{
		// Remove the key frame from this animation
		_keyFrames.erase(it);

		// The key frame was the last frame in this animation
		if (keyFrame->getStartTime() == _duration)
		{
			// There is still at least one key frame in this animation
			// Set the new last key frame start time as this animations duration
			if (_keyFrames.size() > 0)
			{
				_duration = _keyFrames.back()->getStartTime();
			}
		}
	}
	else
		cerr << "Key frame with time: " << keyFrame->getStartTime() << " does not exist in animation with name : " << _name << "!" << endl;
}

void UIAnimation::loadAllAnimsInFile()
{
	// Get list of all animation files in the specified path
	// Open the animation directory
	DIR* dir = opendir(UI_ANIM_DIR);
	struct dirent *entry;

	// Make sure the animation directory exists
	if (!dir)
	{
		cerr << "UI Animation path: <" << UI_ANIM_DIR << "> does not exist!" << endl;
		//system("pause");
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
		file.open(UI_ANIM_DIR + string(entry->d_name));

		if (!file.is_open())
		{
			cerr << "Could not open UI animation file with name: <" << entry->d_name << ">!" << endl;
			//system("pause");
			continue;
		}

		string line;


		// Find the name section of the current animation.
		findLine(line, file, "Name");

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a name section!" << endl;
			//system("pause");
			return;
		}

		getNextLine(line, file);

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a name!" << endl;
			//system("pause");
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
			//system("pause");
			return;
		}

		getNextLine(line, file);

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a loop!" << endl;
			//system("pause");
			return;
		}

		bool loop = stoi(line);


		// Find the key frame section of the current animation.
		findLine(line, file, "KeyFrame");

		if (line.empty())
		{
			cerr << "UI animation file file does not contain a key frame section!" << endl;
			//system("pause");
			return;
		}

		getNextLine(line, file);

		if (line.empty())
		{
			cerr << "UI animation file file does not contain any key frames!" << endl;
			//system("pause");
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
					//system("pause");
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

void UIAnimation::saveAnim(UIAnimation * anim)
{
	// Create/Open an UI animation file.
	ofstream file;
	file.open(UI_ANIM_DIR + anim->getName() + ".UINut", std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		cerr << "Could not create/open UI animation file with name: <" << anim->getName() << ">!" << endl;
		return;
	}

	stringstream stream;

	// Name section
	stream << "Name\n";
	stream << anim->getName() + "\n";
	stream << "end\n";

	// Loop section
	stream << "Loop\n";
	stream << to_string(anim->getLoop()) + "\n";
	stream << "end\n";

	// Key frame section
	stream << "KeyFrames\n";

	vector<UIKeyFrame*> keyFrames = anim->getKeyFrames();
	vec3 pos;
	vec3 scale;
	Quaternion rot;
	for (UIKeyFrame* keyFrame : keyFrames)
	{
		pos = keyFrame->getPos();
		scale = keyFrame->getScale();
		rot = keyFrame->getRot();

		stream << fixed << setprecision(3) << keyFrame->getStartTime() << "  ";

		stream << fixed << setprecision(3) << pos.x << " ";
		stream << fixed << setprecision(3) << pos.y << " ";
		stream << fixed << setprecision(3) << pos.z << "  ";

		stream << fixed << setprecision(3) << scale.x << " ";
		stream << fixed << setprecision(3) << scale.y << " ";
		stream << fixed << setprecision(3) << scale.z << "  ";

		stream << fixed << setprecision(3) << rot.getW() << " ";
		stream << fixed << setprecision(3) << rot.getX() << " ";
		stream << fixed << setprecision(3) << rot.getY() << " ";
		stream << fixed << setprecision(3) << rot.getZ() << "  ";

		stream << fixed << setprecision(3) << keyFrame->getAlpha() << "\n";
	}
	stream << "end\n" << "eof\n";


	file << stream.rdbuf();

	file.close();
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
		////system("pause");
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
		//system("pause");
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
