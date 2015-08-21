#ifndef DAY_H
#define DAY_H
#include "Level.h"
#pragma once
class Day
{
	struct Time
	{
		int hour;
		float min;
	};

public:
	Day(void);
	~Day(void);

	void Init (Time currentTime,float difficulty);
	void UpdateDay(double dt,float gameSpeed);

	Time getCurrentTime(void);
	float getdifficulty(void);
	int getCurrentDay(void);

	void setCurrentTime(int Hr,int Min);
	void setdifficulty(float );
	void setCurrentDay(int);

	vector<Level> levels;

private:
	Time currentTime;
	float difficulty;
	float gameSpeed;
	int currentDay;
};

#endif