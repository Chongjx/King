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
		LEVEL_INPCOMPLETE,
		LEVEL_COMPLETED,
		MAX_STATE,
	};

	Level(void);
	~Level(void);
	vector<Objective> objectives;

	void setState(LEVEL_STATE state);

	LEVEL_STATE getLevelState(void) const;

	void objectiveCheck(void);
		void UpdateObjective(string name);
	void LevelUpdate (void);
private:
	LEVEL_STATE currentState;
};

#endif