#ifndef LEVEL_H
#define LEVEL_H

#include "Objctive.h"
#include <vector>

using std::vector;

class Level
{
public:
	Level(void);
	~Level(void);

	vector<Objctive> objectives;
};

#endif