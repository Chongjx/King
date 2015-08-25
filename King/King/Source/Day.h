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
		int day;
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
	int getCurrentLevel(void);

	void setCurrentTime(int Hr,int Min, int Day);
	void setdifficulty(float );
	void setCurrentLevel(int level);

	vector<Level> levels;

	void CheckObjectives(void);
	void UpdateLevel(void);


	void Initicons(string name,Vector2 size,Vector2 pos ,string mesh);
	Icon sun;
	Icon moon;

private:
	Time currentTime;
	float difficulty;
	float gameSpeed;
	int currentLevel;
};

#endif