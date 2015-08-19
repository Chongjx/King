#include "Player.h"

Player::Player()
	: mapLocation(0)
	, energy(0)
	, MAX_ENERGY(100)
{
}

Player::~Player()
{

}

void Player::Init(Vector2 pos, int tiles, int mapLocation)
{
	this->pos = pos;
	this->tiles = tiles;
	this->mapLocation = mapLocation;
	this->speed = 0;
	this->energy = MAX_ENERGY;
}

void Player::Update(double dt)
{
	Character::Update(dt);
	UpdateEnergy(dt);
}

void Player::SetMapLocation(int mapLocation)
{
	this->mapLocation = mapLocation;
}

int Player::GetMapLocation()
{
	return mapLocation;
}

double Player::GetEnergy()
{
	return energy;
}

void Player::UpdateEnergy(double dt)
{
	/*	 
	if energy is full and not running, do nothing 
	if running, decrease energy with dt.
	if running and no energy, become walking
	if energy is not max and is not running, add energy with dt 
	*/
}

void Player::ConstrainPlayer() /* parameters to be added */
{

}