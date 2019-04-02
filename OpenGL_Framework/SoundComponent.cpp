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

void SoundComponent::stop()
{
	_channel->stop();
}

void SoundComponent::stop2()
{
	_channel2->stop();
}

Sound* SoundComponent::getSound(string soundName)
{
	if (_sounds.find(soundName) != _sounds.end())
		return _sounds[soundName];
	else
	return nullptr;
}

void SoundComponent::playSound(string soundName, bool loops, float volume)
{
	//Sound* temp = getSound(soundName);
	_channel = _sounds[soundName]->Play(loops);
	Sound::SetVolume(_channel, volume);
}

void SoundComponent::playSound2(string soundName, bool loops, float volume)
{
	_channel2 = _sounds[soundName]->Play(loops);
	Sound::SetVolume(_channel2, volume);
}

void SoundComponent::playSound(string soundName, bool loops, float freqMin, float freqMax, float volume)
{
	_channel = _sounds[soundName]->Play(loops);
	setFrequencyRange(_channel, freqMin, freqMax);
	Sound::SetVolume(_channel, volume);
}

void SoundComponent::setFrequencyRange(FMOD::Channel * channel, float freqMin, float freqMax)
{
	float frequency = 0.0f;
	_channel->getFrequency(&frequency);
	frequency += (float(rand()) / float(RAND_MAX))*(freqMax - freqMin) + freqMin;
	//cout << frequency << endl;
	_channel->setFrequency(frequency);
}

void SoundComponent::setFrequency(FMOD::Channel * channel, float frequency)
{
	channel->setFrequency(frequency);
}

FMOD::Channel * SoundComponent::getSoundChannel()
{
	return _channel;
}
