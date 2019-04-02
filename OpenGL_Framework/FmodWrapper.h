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

#pragma once
#pragma comment(lib, "FMOD/lib/fmod_vc.lib")

#include "FMOD/inc/fmod.hpp"
#include "FMOD/inc/fmod_errors.h"

void FmodErrorCheck(FMOD_RESULT result);

struct Listener
{
	FMOD_VECTOR      pos = { 0.0f, 0.0f, -1.0f };
	FMOD_VECTOR		 vel = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR		 forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR		 up = { 0.0f, 1.0f, 0.0f };
};

class SoundEngine
{
private:
	unsigned int     version;
	void            *extradriverdata = 0;
	FMOD_RESULT      result;
	bool			 init = false;

public:
	~SoundEngine();
	void			 Init();
	void			 Update();

	FMOD::System    *system;
	Listener		 listener;
	
};

class Sound
{
private:
	static FMOD_RESULT result;
	FMOD::Channel   *channel = 0;//NULL//change position through channel
	bool			 init = false;

public:
	~Sound();
	bool			 Load(const char* fileName, bool fmod3D = true);
	FMOD::Channel*	 Play(bool loop);
	FMOD::Channel*	 getChannel();
	static void		 SetPosition(FMOD::Channel* thisChannel, FMOD_VECTOR newPos, FMOD_VECTOR newVel = { 0.0f, 0.0f, 0.0f });
	static void		 SetFalloffLinear(FMOD::Channel* thisChannel);
	static void		 SetFalloffInverse(FMOD::Channel* thisChannel);
	static void		 SetLoop(FMOD::Channel* thisChannel, bool loop);
	static void		 Set3DMinMaxDistance(FMOD::Channel* thisChannel, float min, float max);
	static void		 SetVolume(FMOD::Channel* thisChannel, float volume);// between 0.0f and 1.0f


	FMOD::Sound     *sound;//sound is the file
	FMOD_VECTOR		 pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR		 vel = { 0.0f, 0.0f, 0.0f };
	static SoundEngine engine;

};