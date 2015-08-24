#ifndef DAY_H
#define DAY_H

#include "Level.h"
#include <string>
#include "Vector2.h"

using std::string;

class Day
{
	struct Time
	{
		int hour;
		float min;
	};
	struct Icon
	{
		Vector2 size;
		Vector2 pos;
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
		int getCurrentLevel(void);

	void setCurrentTime(int Hr,int Min);
	void setdifficulty(float );
	void setCurrentDay(int);
	void setCurrentLevel(int);

	vector<Level> levels;

	void Initicons(string name,Vector2 size,Vector2 pos ,string mesh);
	Icon sun;
	Icon moon;
private:
	Time currentTime;
	float difficulty;
	float gameSpeed;
	int currentDay;
	int currentLevel;
};

#endif