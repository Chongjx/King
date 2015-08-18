#include "Player.h"

Player::Player(void)
	: mapLocation(0)
{
}

Player::~Player(void)
{

}

void Player::Init()
{

}

void Player::Update()
{

}


void Player::SetMapLocation(int mapLocation)
{
	this->mapLocation = mapLocation;
}

int Player::GetMapLocation(void)
{
	return mapLocation;
}

void Player::ConstrainPlayer(void) /* parameters to be added */
{

}