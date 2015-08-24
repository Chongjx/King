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
		LEVEL_UNASSIGNED,
		LEVEL_INPROGRESS,
		LEVEL_COMPLETED,
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