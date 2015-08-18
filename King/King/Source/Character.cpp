#include "Character.h"

Character::Character()
	: characterPosition(0,0)
	, tiles(0)
{
}

Character::~Character()
{

}

void Character::Init()
{

}

void Character::Update(double dt)
{

}

void Character::MoveUp(double dt)
{
	characterPosition.y += (int) (4.0 * dt);
}

void Character::MoveDown(double dt)
{
	characterPosition.y -= (int) (4.0 * dt);
}

void Character::MoveLeft(double dt)
{
	characterPosition.x -= (int) (4.0 * dt);
}

void Character::MoveRight(double dt)
{
	characterPosition.x += (int) (4.0 * dt);
}

void Character::SetFOV(int tiles)
{
	this->tiles = tiles;
}

int Character::GetFOV()
{
	return tiles;
}
