#include "AI.h"

AI::AI(void)
	: destination(0,0)
	, updating(true)
{
}

AI::~AI(void)
{

}

void AI::Init()
{

}

void AI::Update(double dt)
{
	Character::Update(dt);
}

void AI::SetUpdate(bool updating)
{
	this->updating = updating;
}

bool AI::GetUpdate(void)
{
	return updating;
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

void AI::changeAni(StateMachine::STATE unitState)
{

}