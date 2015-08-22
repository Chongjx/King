#ifndef LEVEL_H
#define LEVEL_H

#include "Objective.h"
#include <vector>

using std::vector;

class Level
{
public:
	enum LEVEL_STATE
	{
		STATE_UNASSIGNED,
		STATE_NOTCOMPLETED,
		STATE_COMPLETED,
		MAX_STATE,
	};

	Level(void);
	~Level(void);
	vector<Objective> objectives;
	void levelUpdate(void);
	void levelStart (void);

	void setState(LEVEL_STATE state);

	LEVEL_STATE getObjectiveState(void) const;

private:
	LEVEL_STATE currentState;

};

#endif