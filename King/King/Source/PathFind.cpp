#include "PathFind.h"
#include <float.h>

PathFind::PathFind(void)
	: recalculateTime(1.0)
	, timer(0.0)
	, startNode(NULL)
	, endNode(NULL)
	, initializedStart(false)
	, foundEnd(false)
{
	openList.clear();
	visitedList.clear();
	pathToEnd.clear();
}

PathFind::~PathFind(void)
{
	for (vector<Node*>::iterator node = openList.begin(); node != openList.end(); ++node)
	{
		Node* tempNode = *node;
		if(tempNode != NULL)
		{
			delete tempNode;
			tempNode = NULL;
		}
	}
	for (vector<Node*>::iterator node = visitedList.begin(); node != visitedList.end(); ++node)
	{
		Node* tempNode = *node;
		if(tempNode != NULL)
		{
			delete tempNode;
			tempNode = NULL;
		}
	}

	for (vector<Vector2*>::iterator vec = pathToEnd.begin(); vec != pathToEnd.end(); ++vec)
	{
		Vector2* tempVec = *vec;
		if(tempVec != NULL)
		{
			delete tempVec;
			tempVec = NULL;
		}
	}

	if(startNode)
	{
		delete startNode;
		startNode = NULL;
	}

	if (endNode)
	{
		delete endNode;
		endNode = NULL;
	}
}

void PathFind::FindPath(Vector2 currentPos, Vector2 targetPos)
{
	if (!initializedStart)
	{
		for (unsigned i = 0; i < openList.size(); ++i)
		{
			delete openList[i];
		}

		openList.clear();

		for (unsigned i = 0; i < visitedList.size(); ++i)
		{
			delete visitedList[i];
		}

		visitedList.clear();

		for (unsigned i = 0; i < pathToEnd.size(); ++i)
		{
			delete pathToEnd[i];
		}

		pathToEnd.clear();

		// Initialize start
		Node start;
		start.setPos(currentPos);

		// Initialize end
		Node end;
		end.setPos(targetPos);

		setDestination(start, end);
		initializedStart = true;
	}

	if (initializedStart)
	{
		continuePath();
	}
}

void PathFind::setDestination(Node start, Node end)
{
	this->startNode = new Node(start.getPos(), NULL);
	this->endNode = new Node(end.getPos(), NULL);

	this->startNode->setG(0);
	this->startNode->setH(this->startNode->ManHattanDistance(endNode));
	this->startNode->setParentNode(NULL);

	openList.push_back(startNode);
}

Node* PathFind::getNextNode()
{
	float smallestF = FLT_MAX;
	int cellIndex = -1;

	Node* nextNode = NULL;
	
	for (unsigned i = 0; i < openList.size(); ++i)
	{
		if (openList[i]->getF() < smallestF)
		{
			smallestF = openList[i]->getF();
			cellIndex = i;
		}
	}

	if (cellIndex >= 0)
	{
		nextNode = openList[cellIndex];
		visitedList.push_back(nextNode);
		openList.erase(openList.begin() + cellIndex);
	}

	return nextNode;
}

void PathFind::pathOpened(Vector2 pos, float newCost, Node* parent)
{
	if (!parent->getPassable())
	{
		return;
	}
	
	int id = pos.y * gameDimension.y + pos.x;
	for (unsigned i = 0; i < visitedList.size(); ++i)
	{
		// if the node is inside the list already ignore
		if (id == visitedList[i]->getID())
		{
			return;
		}
	}

	Node* nextNode = new Node(pos, parent);

	nextNode->setG(newCost);
	nextNode->setH(parent->ManHattanDistance(endNode));

	for (unsigned i = 0; i < openList.size(); ++i)
	{
		if (id == openList[i]->getID())
		{
			float newValue = nextNode->getG() + newCost + openList[i]->getH();
			// replace with better node
			if (openList[i]->getF() > newValue)
			{
				openList[i]->setG(nextNode->getG() + newCost);
				openList[i]->setParentNode(nextNode);
			}
			
			// if it is not a shorter node
			else
			{
				delete nextNode;
				return;
			}
		}
	}

	openList.push_back(nextNode);
}

void PathFind::continuePath()
{
	for (int i = 0; i < 4; ++i)
	{
		if (openList.empty())
		{
			return;
		}

		Node* currentNode = getNextNode();

		if (currentNode->getID() == endNode->getID())
		{
			endNode->setParentNode(currentNode->getParentNode());

			Node* getPath;

			for (getPath = endNode; getPath != NULL; getPath = getPath->getParentNode())
			{
				pathToEnd.push_back(new Vector2(getPath->getPos()));
			}

			foundEnd = true;
			return;
		}

		else
		{
			// right side
			pathOpened(Vector2(currentNode->getPos().x + 1, currentNode->getPos().y), currentNode->getG() + 1, currentNode);

			// left side
			pathOpened(Vector2(currentNode->getPos().x - 1, currentNode->getPos().y), currentNode->getG() + 1, currentNode);
		
			// up
			pathOpened(Vector2(currentNode->getPos().x, currentNode->getPos().y + 1), currentNode->getG() + 1, currentNode);

			// down
			pathOpened(Vector2(currentNode->getPos().x, currentNode->getPos().y - 1), currentNode->getG() + 1, currentNode);

			// top left
			pathOpened(Vector2(currentNode->getPos().x - 1, currentNode->getPos().y + 1), currentNode->getG() + 1.414f, currentNode);

			// top right
			pathOpened(Vector2(currentNode->getPos().x + 1, currentNode->getPos().y + 1), currentNode->getG() + 1.414f, currentNode);

			// bottom left
			pathOpened(Vector2(currentNode->getPos().x - 1, currentNode->getPos().y - 1), currentNode->getG() + 1.414f, currentNode);

			// bottom right
			pathOpened(Vector2(currentNode->getPos().x + 1, currentNode->getPos().y - 1), currentNode->getG() + 1.414f, currentNode);

			for (unsigned i = 0; i < openList.size(); ++i)
			{
					if (currentNode->getID() == openList[i]->getID())
				{
					openList.erase(openList.begin() + i);
				}
			}
		}
	}
}

Vector2 PathFind::getGameDimension(void)
{
	return this->gameDimension;
}

void PathFind::setGameDimension(Vector2 dimension)
{
	this->gameDimension = dimension;
}

Vector2 PathFind::nextPathPos(Vector2 pos)
{
	unsigned index = 1;

	Vector2 nextPos;
	nextPos = *(pathToEnd[pathToEnd.size() - index]);

	// pos = ai pos
	Vector2 distance = nextPos - pos;

	if (index < pathToEnd.size())
	{
		if (distance.Length() < 32)
		{
			pathToEnd.erase(pathToEnd.end() - index);
		}
	}

	return nextPos;
}

void PathFind::clearOpenList()
{
	for (unsigned i = 0; i < openList.size(); ++i)
	{
		delete openList[i];
	}

	openList.clear();
}

void PathFind::clearVisitedList()
{
	for (unsigned i = 0; i < visitedList.size(); ++i)
	{
		delete visitedList[i];
	}

	visitedList.clear();
}

void PathFind::clearPathToGoal()
{
	for (unsigned i = 0; i < pathToEnd.size(); ++i)
	{
		delete pathToEnd[i];
	}

	pathToEnd.clear();
}

bool PathFind::getStart(void)
{
	return this->initializedStart;
}

bool PathFind::getReachEnd(void)
{
	return this->foundEnd;
}