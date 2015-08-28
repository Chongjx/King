#ifndef PATHFIND_H
#define PATHFIND_H

#include "Nodes.h"
#include <vector>
using std::vector;

class PathFind
{
	
public:

	PathFind(void);
	~PathFind(void);

	void Init(void);
	void Update(double dt, Vector2 pos);

private:
	
	Nodes* current;

	vector<Nodes*> OpenList;
	vector<Nodes*> CloseList;

	double timer, recalculateTime;

};

#endif