#include "Day.h"


Day::Day(void):
currentLevel(0)
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
		currentTime.day++;
		levels[currentLevel].UpdateObjective("Explore");		
	}
	UpdateLevel();

}


Day::Time Day::getCurrentTime(void)
{
	return currentTime;
}
int Day::getCurrentLevel(void)
{
	return currentLevel;
}

void Day::setCurrentTime(int Hr,int Min,int Day)
{
	currentTime.min = (float)Min;
	currentTime.hour = Hr;
	currentTime.day = Day;
}
void Day::setdifficulty(float difficulty)
{
	this->difficulty=difficulty;
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

void Day::UpdateLevel(void)
{
	levels[currentLevel].LevelUpdate();
	if(levels[currentLevel].getLevelState() == Level::LEVEL_COMPLETED)
	{
		currentLevel++;
	}
}

void Day::incrementDay (void)
{
	currentTime.day +=1;
}