#include "Level.h"

Level::Level(void):
currentState(LEVEL_INPCOMPLETE)
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

Level::LEVEL_STATE Level::getLevelState(void) const
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

void Level::LevelUpdate (void)
{
	for (vector<Objective>::iterator objective = objectives.begin(); objective != objectives.end(); ++objective)
	{
		if(objective->getObjectiveState() != objective->OBJECTIVE_COMPLETED)
		{
			setState(LEVEL_INPCOMPLETE);
			break;
		}
		else
		{
			setState(LEVEL_COMPLETED);
		}	
	}
}

void  Level::UpdateObjective(string name)
{
		for (vector<Objective>::iterator objective = objectives.begin(); objective != objectives.end(); ++objective)
		{
			if (objective->getTitle() == name)
			{
				objective->setState(objective->OBJECTIVE_COMPLETED);
			}
		}
	
}