#ifndef SOUND_H
#define SOUND_H
#include <iostream>
#include <vector>
#include <string>
#include <irrKlang.h>
using namespace std;

class Sound
{
public:
	Sound(void);
	~Sound(void);
	void Init(std::string soundName, std::string soundFile,float volume,bool loop);
	vector<Sound> sounds;

	void setSoundName(string soundName);
	void setSoundFile(string soundFile);
	void setVolume(float volume);
	void setLoop(bool loop);

	string getSoundName(void)const;
	string getSoundFile(void)const;
	float getVolume(void)const;
	bool getLoop(void)const;

	void VolumeUp (void);
	void VolumeDown (void);

	void Play(string soundName);
	void Stop(void);
private:
	std::string soundName;
	std::string soundFile;
	float volume;
	bool loop;
	irrklang::ISoundEngine* Soundengine;


};

#endif