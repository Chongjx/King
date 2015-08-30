#include "Node.h"

Node::Node(void) :
	parent(NULL)
{
}

Node::Node(Vector2 pos, Node* parent) :
	pos(pos),
	parent(parent),
	ID(0),
	G(0),
	H(0),
	passable(false)
{
}

Node::~Node(void)
{
	if (parent != NULL)
		delete parent;
}

void Node::Init(Vector2 pos, int ID, Node* parent, bool passable)
{
	this->pos = pos;
	this->ID = ID;
	this->parent = parent;
	this->G = 0;
	this->H = 0;
	this->passable = passable;
}

void Node::setPos(Vector2 pos)
{
	this->pos = pos;
}

void Node::setID(int ID)
{
	this->ID = ID;
}

void Node::setParentNode(Node* parentNode)
{
	this->parent = parentNode;
}

void Node::setG(float g)
{
	this->G = g;
}

void Node::setH(float h)
{
	this->H = h;
}

void Node::setPassable(bool passable)
{
	this->passable = passable;
}

Vector2 Node::getPos(void) const
{
	return this->pos;
}

int Node::getID(void) const
{
	return this->ID;
}

Node* Node::getParentNode(void) const
{
	return this->parent;
}

float Node::getG(void) const
{
	return this->G;
}

float Node::getH(void) const
{
	return this->H;
}

float Node::getF(void) const
{
	return G + H;
}

bool Node::getPassable(void) const
{
	return this->passable;
}

float Node::ManHattanDistance(Node* nodeEnd)
{
	float x = (float)(Math::FAbs(this->pos.x - nodeEnd->getPos().x));
	float z = (float)(Math::FAbs(this->pos.y - nodeEnd->getPos().y));

	return x + z;
}