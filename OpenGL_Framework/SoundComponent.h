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
	void playSound(string soundName, FMOD::Channel* channel, bool loops, float freqMin, float freqMax, float volume = 1.0f);
	void setFrequencyRange(FMOD::Channel* channel, float freqMin, float freqMax);
	void setFrequency(FMOD::Channel* channel, float frequency);

	FMOD::Channel* getBGChannel();
	FMOD::Channel* getPlayerChannel();
	FMOD::Channel* getActionChannel();

private:
	SoundComponent() {};
	static SoundComponent* _instance;

	unordered_map<string, Sound*> _sounds;

	FMOD::Channel* backGroundChannel;//background music
	FMOD::Channel* playerChannel;// for the sounds player makes directly (grunts)
	FMOD::Channel* actionChannel;//shifting between z axis

};