#include "Character.h"

Character::Character()
	: active(false)
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
	Collision();
}

void Character::MoveUp(double dt)
{
	theCharacterPosition.y += (int) (4.0 * dt);
}

void Character::MoveDown(double dt)
{
	theCharacterPosition.y -= (int) (4.0 * dt);
}

void Character::MoveLeft(double dt)
{
	theCharacterPosition.x -= (int) (4.0 * dt);
}

void Character::MoveRight(double dt)
{
	theCharacterPosition.x += (int) (4.0 * dt);
}

void Character::SetFOV(int tiles)
{
	this->tiles = tiles;
}

int Character::GetFOV()
{
	return tiles;
}

void Character::Collision()
{

}