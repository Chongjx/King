#ifndef NODES_H
#define NODES_H

#include "Vector2.h"

class Node
{
	
public:
	Node(void);
	Node(Vector2 pos, Node* parent = NULL);
	~Node(void);

	void Init(Vector2 pos, int ID, Node* parent = NULL, bool passable = false);
	void setPos(Vector2 pos);
	void setID(int ID);
	void setParentNode(Node* parentNode);
	void setG(float g);
	void setH(float h);
	void setPassable(bool passable);
	
	Vector2 getPos(void) const;
	int getID(void) const;
	Node* getParentNode(void) const;
	float getG(void) const;
	float getH(void) const;
	float getF(void) const;
	bool getPassable(void) const;

	float ManHattanDistance(Node* nodeEnd);
private:
	Vector2 pos;
	int ID;
	Node* parent;
	float G;
	float H;
	bool passable;
};

#endif