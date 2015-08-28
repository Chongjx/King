#include "PathFind.h"

PathFind::PathFind(void)
	: recalculateTime(1.0)
	, timer(0.0)
	, current(NULL)
{
	OpenList.clear();
	CloseList.clear();
}

PathFind::~PathFind(void)
{
	for (vector<Nodes*>::iterator node = OpenList.begin(); node != OpenList.end(); ++node)
	{
		Nodes* tempNode = *node;
		if(tempNode != NULL)
		{
			delete tempNode;
			tempNode = NULL;
		}
	}
	for (vector<Nodes*>::iterator node = CloseList.begin(); node != CloseList.end(); ++node)
	{
		Nodes* tempNode = *node;
		if(tempNode != NULL)
		{
			delete tempNode;
			tempNode = NULL;
		}
	}

	if(current)
	{
		delete current;
	}
}


void PathFind::Init(void)
{
	
}

void PathFind::Update(double dt, Vector2 pos)
{
	timer += dt;

	if(timer > recalculateTime)
	{
		//OpenList.push_back(node);
		timer = 0.0;
	}
}