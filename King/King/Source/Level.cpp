#include "Level.h"

Level::Level(void):
currentState(LEVEL_UNASSIGNED)
{
}


Level::~Level(void)
{

}

bool Level::levelcheck(void)
{
	for (vector<Objective>::iterator itr = objectives.begin(); itr != objectives.end(); ++itr)
	{
		if(itr->getObjectiveState() != itr->OBJECTIVE_COMPLETED)
		{
			return false;
		}
		else return true;
	}
}

void Level::setState(LEVEL_STATE state)
{

	this->currentState = state; 
}

Level::LEVEL_STATE Level::getObjectiveState(void) const
{
	return this->currentState;
}

void Level::levelUpdate(void)
{
	if(levelcheck()!=true)
	{
		currentState=LEVEL_INPROGRESS;
	}
	else 
	{
		currentState=LEVEL_COMPLETED;
	}
}

void Level::objectiveCheck(void)
{
	for (vector<Objective>::iterator itr = objectives.begin(); itr != objectives.end(); ++itr)
	{
		if(itr->getGet()== true)
		{
			itr->setState(itr->OBJECTIVE_COMPLETED);
		}
	}
}