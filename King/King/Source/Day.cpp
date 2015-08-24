#include "Day.h"


Day::Day(void):
currentDay(1)
{
}

Day::~Day(void)
{
}

void Day::UpdateDay(double dt,float gameSpeed)
{
	currentTime.min += (float)dt * gameSpeed * difficulty;

	if(currentTime.min > 60.0f)
	{
		currentTime.min = 0;
		currentTime.hour += 1;
	}
	if(currentTime.hour == 24)
	{
		currentTime.min = 0;
		currentTime.hour = 0;
		++currentDay;
	}
}

Day::Time Day::getCurrentTime(void)
{
	return currentTime;
}

int Day::getCurrentDay(void)
{
	return currentDay;
}

int Day::getCurrentLevel(void)
{
	return currentLevel;
}


void Day::setCurrentTime(int Hr,int Min)
{
	currentTime.min = (float)Min;
	currentTime.hour = Hr;
}
void Day::setdifficulty(float difficulty)
{
	this->difficulty=difficulty;
}

void Day::setCurrentDay(int currentDay)
{
	this->currentDay = currentDay;
}

void Day::setCurrentLevel(int level)
{
currentLevel = level;
}

void Day::Initicons(string name,Vector2 size,Vector2 pos ,string mesh)
{
	if (name == "Sun")
	{
		sun.size = size;
		sun.pos = pos;
		sun.mesh = mesh;
	}
	else if(name=="Moon")
	{
		moon.size = size;
		moon.pos = pos;
		moon.mesh = mesh;
	}
}