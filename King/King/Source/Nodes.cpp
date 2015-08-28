#include "Nodes.h"

Nodes::Nodes(void)
{
}

Nodes::~Nodes(void)
{
}

void Nodes::SetNode(Vector2 node)
{
	this->node = node;
}

Vector2 Nodes::GetNode()
{
	return node;
}

void Nodes::SetStartNode(Vector2 startNode)
{
	this->startNode = startNode;
}

Vector2 Nodes::GetStartNode(void)
{
	return startNode;
}

void Nodes::SetEndNode(Vector2 endNode)
{
	this->endNode = endNode;
}

Vector2 Nodes::GetEndNode(void)
{
	return endNode;
}