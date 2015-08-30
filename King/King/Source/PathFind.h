#ifndef PATHFIND_H
#define PATHFIND_H

#include "Node.h"
#include <vector>
using std::vector;

class PathFind
{
public:
	PathFind(void);
	~PathFind(void);

	void FindPath(Vector2 currentPos, Vector2 targetPos);
	void clearOpenList();
	void clearVisitedList();
	void clearPathToGoal();

	void setDestination(Node start, Node end);
	void pathOpened(Vector2 pos, float newCost, Node* parent);
	Node* getNextNode(void);
	void continuePath(void);
	Vector2 getGameDimension(void);

	void setGameDimension(Vector2 dimension);
	Vector2 nextPathPos(Vector2 pos);
	bool getStart(void);
	bool getReachEnd(void);
private:
	Node* startNode;
	Node* endNode;

	bool initializedStart;
	bool foundEnd;

	vector<Node*> openList;
	vector<Node*> visitedList;
	vector<Vector2*> pathToEnd;
	Vector2 gameDimension;

	double timer, recalculateTime;
};

#endif