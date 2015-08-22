#include "Prisoners.h"

Prisoners::Prisoners(void)
{

}

Prisoners::~Prisoners(void)
{

}

void Prisoners::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room& currentRoom)
{
	this->pos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->currentRoom = currentRoom;
	this->changeAni(StateMachine::IDLE_STATE);
}

void Prisoners::Update(double dt)
{
	if(GetUpdate() == true)
	{
		AI::Update(dt);
	}
}

void Prisoners::changeAni(StateMachine::STATE unitState)
{
}