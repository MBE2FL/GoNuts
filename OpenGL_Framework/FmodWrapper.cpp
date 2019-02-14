/*==============================================================================
3D Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2018.

This example shows how to basic 3D positioning of sounds.

Kevin Lounsbury Feb. 6, 2019
added:
	SetFalloffLinear
	SetFalloffInverse
	SetLoop
	Set3DMinMaxDistance
==============================================================================*/
#include "FmodWrapper.h"
#include <iostream>

using namespace std;

//Uses FMOD_OK to determine if a FMOD command resulted in an error
void FmodErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		cout << FMOD_ErrorString(result) << endl;
		system("pause");
	}
}

//if FMOD engine has been initialized, close it release its memory
SoundEngine::~SoundEngine()
{
	if (init)
	{
		result = system->close();
		FmodErrorCheck(result);
		result = system->release();
		FmodErrorCheck(result);
		init = false;
	}
}

void SoundEngine::Init()
{
	if (!init)//Only initalizes one engine
	{
		//	Create a System object and initialize.
		result = FMOD::System_Create(&system);
		FmodErrorCheck(result);
		if (result != FMOD_OK) return;

		result = system->getVersion(&version);
		FmodErrorCheck(result);

		result = system->init(100, FMOD_INIT_NORMAL, extradriverdata);
		FmodErrorCheck(result);

		//	Set the distance units. (meters/feet etc).
		result = system->set3DSettings(1.0, 1.0f, 1.0f);
		FmodErrorCheck(result);
	}
	init = true;
}

void SoundEngine::Update()
{
	if (init)
	{
		// UPDATE THE LISTENER					//array of listeners could be used for splitscreen
		result = system->set3DListenerAttributes(0, &listener.pos, &listener.vel, &listener.forward, &listener.up);
		FmodErrorCheck(result);

		result = system->update();//updating Fmod, things will only move if this is called, call once per gameloop
		FmodErrorCheck(result);
	}
}

//declaring staic variables
FMOD_RESULT Sound::result;
SoundEngine Sound::engine;

//if a sound has been initialized, deconstruct it and free memory
Sound::~Sound()
{
	if (init)
	{
		result = sound->release();
		FmodErrorCheck(result);
		init = false;
		sound = nullptr;
	}
}

//load a sound from a file
bool Sound::Load(const char * fileName, bool fmod3D)
{
	//initialize the engine
	engine.Init();
	
	//if the sound has not been initialized, initialize it
	if (!init)
	{
		//Load some sounds
		// if the sound is 3D it will be set to 3D, else it will be set to 2D
		if(fmod3D)
			result = engine.system->createSound(fileName, FMOD_3D, 0, &sound);
		else
			result = engine.system->createSound(fileName, FMOD_2D, 0, &sound);

		FmodErrorCheck(result);
		if (result != FMOD_OK)
		{
			cout << fileName << " did not load correctly.\n";
			return false;
		}

		//set the min and max distance that the file can be heard from
		//result = sound->set3DMinMaxDistance(1.0f, 500.0f);
		//FmodErrorCheck(result);
		
		init = true;
	}
	return true;
}

FMOD::Channel * Sound::Play(bool loop)//bool is3D
//overloaded play functions
{
	if (init)
	{
		//Play sounds at certain positions
		//start it paused to init things before it plays
		result = engine.system->playSound(sound, 0, true, &channel);
		FmodErrorCheck(result);
		//result = channel->set3DAttributes(&pos, &vel);
		//FmodErrorCheck(result);
		//sets the sound to loop or not to loop
		if (loop) 
		{
			channel->setMode(FMOD_LOOP_NORMAL); 
		}
		else 
		{ 
			channel->setMode(FMOD_LOOP_OFF); 
		}

		result = channel->setPaused(false);
		FmodErrorCheck(result);
	}

	return channel;
}

void Sound::SetPosition(FMOD::Channel * thisChannel, FMOD_VECTOR newPos, FMOD_VECTOR newVel)
{
	//safety check to make sure the sound is playing
	static bool isPlay;
	thisChannel->isPlaying(&isPlay);
	//safety check for is3D needs to be created
	if (isPlay)
	{
		result = thisChannel->set3DAttributes(&newPos, &newVel);
		FmodErrorCheck(result);
	}
}
//sets the sound fall off mode to linear
void Sound::SetFalloffLinear(FMOD::Channel * thisChannel)
{
	//safety check to make sure the sound is playing
	static bool isPlay;
	thisChannel->isPlaying(&isPlay);
	
	if (isPlay)
	{
		result = thisChannel->setMode(FMOD_3D_LINEARROLLOFF);
		FmodErrorCheck(result);
	}
}
//sets the sound fall off mode to inverse
void Sound::SetFalloffInverse(FMOD::Channel * thisChannel)
{
	//safety check to make sure the sound is playing
	static bool isPlay;
	thisChannel->isPlaying(&isPlay);

	if (isPlay)
	{
		result = thisChannel->setMode(FMOD_3D_INVERSEROLLOFF);
		FmodErrorCheck(result);
	}
}

//set the sound to loop or not to loop
void Sound::SetLoop(FMOD::Channel * thisChannel, bool loop)
{
	if (loop)
	{
		result = thisChannel->setMode(FMOD_LOOP_NORMAL);
		FmodErrorCheck(result);
	}
	else
	{
		result = thisChannel->setMode(FMOD_LOOP_OFF);
		FmodErrorCheck(result);
	}
}

//sets the min and max distance the sound can be heard from
void Sound::Set3DMinMaxDistance(FMOD::Channel * thisChannel, float min, float max)
{
	//safety check to make sure the sound is playing
	static bool isPlay;
	thisChannel->isPlaying(&isPlay);

	if (isPlay)
	{
		result = thisChannel->set3DMinMaxDistance(min, max);
		FmodErrorCheck(result);
	}
}

void Sound::SetVolume(FMOD::Channel * thisChannel, float volume)
{
	thisChannel->setVolume(volume);
}
