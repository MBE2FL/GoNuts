#pragma once
#include "FmodWrapper.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "MathLib/MathLibCore.h"

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
	void stop2();
	Sound* getSound(string soundName);
	void playSound(string soundName, bool loops, float volume = 1.0f);
	void playSound2(string soundName, bool loops, float volume = 1.0f);
	void play3dSound(string soundName, bool loops,vec3 position, float volume = 1.0f);
	void playSound(string soundName, bool loops, float freqMin, float freqMax, float volume = 1.0f);
	void setFrequencyRange(FMOD::Channel* channel, float freqMin, float freqMax);
	void setFrequency(FMOD::Channel* channel, float frequency);

	void setLposition(vec3 position);
	FMOD::Channel* getSoundChannel();

private:
	SoundComponent() {};
	static SoundComponent* _instance;

	unordered_map<string, Sound*> _sounds;

	Listener listener;

	FMOD::Channel* _channel;
	FMOD::Channel* _channel2;
	FMOD::Channel* _channel3D;

};