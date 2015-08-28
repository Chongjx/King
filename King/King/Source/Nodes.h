#ifndef NODES_H
#define NODES_H

#include "Vector2.h"

class Nodes
{
	
public:

	Nodes(void);
	~Nodes(void);

	void SetNode(Vector2 node);
	Vector2 GetNode(void);

	void SetStartNode(Vector2 startNode);
	Vector2 GetStartNode(void);

	void SetEndNode(Vector2 endNode);
	Vector2 GetEndNode(void);

private:

	Vector2 node;
	Vector2 startNode, endNode;

};

#endif