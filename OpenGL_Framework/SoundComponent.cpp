#include "SoundComponent.h"

SoundComponent* SoundComponent::_instance = nullptr;

SoundComponent * SoundComponent::getInstance()
{
	if (!_instance)
	{
		_instance = new SoundComponent();
	}

	return _instance;
}

void SoundComponent::loadSound(string soundName, string soundfile, bool is3D)
{
	Sound* sound = new Sound;
	string directory = "./Assets/Sounds/" + soundfile;
	sound->Load( directory.c_str(),is3D);
	if (_sounds.find(soundName) == _sounds.end())
		_sounds[soundName] = sound;
	else
	{
		cout << "sound: " + soundName + " already exists" << endl;
		system("pause");
	}
}

Sound* SoundComponent::getSound(string soundName)
{
	if (_sounds.find(soundName) != _sounds.end())
		return _sounds[soundName];
	else
	return nullptr;
}

void SoundComponent::playSound(string soundName, FMOD::Channel * channel, bool loops, float volume)
{
	Sound* temp = getSound(soundName);
	channel = temp->Play(loops);
	Sound::SetVolume(channel, volume);
}

FMOD::Channel * SoundComponent::getBGChannel()
{
	return backGroundChannel;
}

FMOD::Channel * SoundComponent::getPlayerChannel()
{
	return playerChannel;
}
