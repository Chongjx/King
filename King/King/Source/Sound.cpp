#include "Sound.h"

Sound::Sound(void):
soundName(""),
	soundFile(""),
	volume(1.0f),
	loop(false),
	Soundengine(irrklang::createIrrKlangDevice())
{
	sounds.clear();
}

Sound::~Sound(void)
{
	sounds.clear();
}

void Sound::Init(std::string soundName, std::string soundFile,float volume,bool loop)
{
	this->soundName=soundName;
	this->soundFile =soundFile;
	this->volume = volume;
	this->loop = loop;
	this->Soundsource=Soundengine->addSoundSourceFromFile(this->soundFile.c_str());
}

void Sound::setSoundName(string soundName)
{
	this->soundName=soundName;
}
void Sound::setSoundFile(string soundFile)
{
	this->soundFile =soundFile;
}
void Sound::setVolume(float volume)
{
	this->volume = volume;
}
void Sound::setLoop(bool loop)
{
	this->loop = loop;
}

void Sound::setSource(irrklang::ISoundSource*Soundsource)
{
	this->Soundsource=Soundsource;
}

string Sound::getSoundName(void) const
{
	return this->soundName;
}
string Sound::getSoundFile(void)const
{
	return this->soundFile;
}
float Sound::getVolume(void)const
{
	return this->volume;
}
bool Sound::getLoop(void)const
{
	return this->loop;
}

irrklang::ISoundSource*  Sound::getSource(void) const
{
	return this->Soundsource;
}

void Sound::Play(string Name)
{
	for (vector<Sound>::iterator itr =sounds.begin(); itr != sounds.end(); ++itr)
	{
		if(itr->getSoundName().c_str() == Name)
		{
			if(!Soundengine->isCurrentlyPlaying(itr->getSoundFile().c_str()))
			{
				itr->getSource()->setDefaultVolume(itr->volume);
				Soundengine->play2D(itr->getSource(),itr->loop);
			}
		}
	}
}

void Sound::VolumeUp (void)
{
	for (vector<Sound>::iterator itr =sounds.begin(); itr != sounds.end(); ++itr)
	{	
		Soundengine->setSoundVolume(itr->volume + 0.1f);

	}
}
void Sound::VolumeDown (void)
{
	for (vector<Sound>::iterator itr =sounds.begin(); itr != sounds.end(); ++itr)
	{	
		Soundengine->setSoundVolume(itr->volume - 0.1f);

	}
}

void Sound::Stop(void)
{
	Soundengine->stopAllSounds();
}


void Sound::Stop(string Name)
{
	for (vector<Sound>::iterator itr =sounds.begin(); itr != sounds.end(); ++itr)
	{
		Soundengine->removeSoundSource(itr->getSource()); 
	}
}

