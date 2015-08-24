#include "Level.h"

Level::Level(void):
currentState(LEVEL_UNASSIGNED)
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

}