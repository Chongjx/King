#include "AI.h"

AI::AI(void)
	: destination(0,0)
{
}

AI::~AI(void)
{

}

void AI::Init()
{

}

void AI::Update(double dt, bool updating)
{
	if (updating == true)
	{

	}
	else
	{

	}
}

void AI::SetDestination(float x, float y)
{
	this->destination.x = x;
	this->destination.y = y;
}
	
Vector2 AI::GetDestination(void)
{
	return destination;
}