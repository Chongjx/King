#include "Level.h"


Level::Level(void):
currentState(STATE_UNASSIGNED)
{
}


Level::~Level(void)
{
}

void Level::levelUpdate(void)
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

void Level::levelStart(void)
{
	for (vector<Objective>::iterator it = objectives.begin(); it != objectives.end(); ++it)
	{
			Objective tempObjective = *it;
			tempObjective.setState(Objective::STATE_COMPLETED);
	}
}