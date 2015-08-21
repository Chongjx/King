#include "Guards.h"

Guards::Guards(void)
{

}

Guards::~Guards(void)
{

}

void Guards::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, int mapLocation)
{
	this->pos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->mapLocation = mapLocation;
	this->changeAni(StateMachine::IDLE_STATE);
}

void Guards::Update(double dt)
{
	if(GetUpdate() == true)
	{
		AI::Update(dt);
	}
}

void Guards::changeAni(StateMachine::STATE unitState)
{
}