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

void SoundComponent::playSound(string soundName, FMOD::Channel * channel, bool loops, float freqMin, float freqMax, float volume)
{
	Sound* temp = getSound(soundName);
	channel = temp->Play(loops);
	setFrequencyRange(channel, freqMin, freqMax);
	Sound::SetVolume(channel, volume);
}

void SoundComponent::setFrequencyRange(FMOD::Channel * channel, float freqMin, float freqMax)
{
	float frequency = 0.0f;
	channel->getFrequency(&frequency);
	frequency += (float(rand()) / float(RAND_MAX))*(freqMax - freqMin) + freqMin;
	cout << frequency << endl;
	channel->setFrequency(frequency);
}

void SoundComponent::setFrequency(FMOD::Channel * channel, float frequency)
{
	channel->setFrequency(frequency);
}

FMOD::Channel * SoundComponent::getBGChannel()
{
	return backGroundChannel;
}

FMOD::Channel * SoundComponent::getPlayerChannel()
{
	return playerChannel;
}

FMOD::Channel * SoundComponent::getActionChannel()
{
	return actionChannel;
}
