#include "Objective.h"

Objective::Objective(void) :
difficulty(1),
	currentState(STATE_UNASSIGNED)
{
}


Objective::~Objective(void)
{
}

void Objective::setDifficulty(int difficulty)
{
	this->difficulty = difficulty;
}

void Objective::setState(OBJECTIVE_STATE state)
{
	this->currentState = state;
}

int Objective::getDifficulty(void) const
{
	return this->difficulty;
}

Objective::OBJECTIVE_STATE Objective::getObjectiveState(void) const
{
	return this->currentState;
}