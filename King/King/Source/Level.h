#ifndef LEVEL_H
#define LEVEL_H

#include "Objective.h"
#include <vector>

using std::vector;

class Level
{
public:
	Level(void);
	~Level(void);

	vector<Objective> objectives;
};

#endif