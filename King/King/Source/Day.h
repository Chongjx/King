#ifndef DAY_H
#define DAY_H
#include "Level.h"
#include <string>
using std::string;
#pragma once
class Day
{
	struct Time
	{
		int hour;
		float min;
	};
	struct Icon
	{
		int size;
		int pos_x;
		int pos_y;
		string mesh;
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

	void Initicons(string name,int size,int pos_x,int pos_y ,string mesh);
	Icon sun;
	Icon moon;
private:
	Time currentTime;
	float difficulty;
	float gameSpeed;
	int currentDay;
};

#endif