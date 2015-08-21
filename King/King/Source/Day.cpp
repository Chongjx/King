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
		currentDay++;
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

void Day::setCurrentTime(int Hr,int Min)
{
	currentTime.min = Min;
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