#include "Level.h"

Level::Level(void):
currentState(LEVEL_UNASSIGNED)
{
	objectives.clear();
}


Level::~Level(void)
{

}

void Level::setState(LEVEL_STATE state)
{

	this->currentState = state; 
}

Level::LEVEL_STATE Level::getObjectiveState(void) const
{
	return this->currentState;
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
