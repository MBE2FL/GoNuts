#pragma once
#include "FmodWrapper.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using std::vector;
using std::string;
using std::unordered_map;
using std::cout;
using std::endl;

class SoundComponent
{
public:
	static SoundComponent* getInstance();
	void loadSound(string soundName, string soundfile, bool is3D);
	Sound* getSound(string soundName);
	void playSound(string soundName, FMOD::Channel* channel, bool loops, float volume = 1.0f);

	FMOD::Channel* getBGChannel();
	FMOD::Channel* getPlayerChannel();

private:
	SoundComponent() {};
	static SoundComponent* _instance;

	unordered_map<string, Sound*> _sounds;

	FMOD::Channel* backGroundChannel;
	FMOD::Channel* playerChannel;

};