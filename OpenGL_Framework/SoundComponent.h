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
	void stop();
	Sound* getSound(string soundName);
	void playSound(string soundName, bool loops, float volume = 1.0f);
	void playSound(string soundName, bool loops, float freqMin, float freqMax, float volume = 1.0f);
	void setFrequencyRange(FMOD::Channel* channel, float freqMin, float freqMax);
	void setFrequency(FMOD::Channel* channel, float frequency);

	//FMOD::Channel* getBGChannel();
	//FMOD::Channel* getPlayerChannel();
	//FMOD::Channel* getActionChannel();
	FMOD::Channel* getSoundChannel();

private:
	SoundComponent() {};
	static SoundComponent* _instance;

	unordered_map<string, Sound*> _sounds;

	FMOD::Channel* _channel;

};