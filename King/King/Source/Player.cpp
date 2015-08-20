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
	if (Character::state.GetState() == StateMachine::RUN)
	{
		if ( energy > 0 )
		{
			//decrease energy
			energy -= 10 * dt;
		}
		else 
		{
			energy = 0;
			Character::state.SetState(StateMachine::WALK);
		}
	}

	else if (Character::state.GetState() == StateMachine::WALK)
	{
		if ( energy < MAX_ENERGY )
		{
			//recover energy
			energy += 5 * dt;
			//cap energy at max energy
			if (energy > MAX_ENERGY)
			{
				energy = MAX_ENERGY;
			}
		}
	}

	else if (Character::state.GetState() == StateMachine::IDLE)
	{
		if ( energy < MAX_ENERGY )
		{
			//recover energy a littleeee bit faster than walk
			energy += 7.5 * dt;
			//cap energy at max energy
			if (energy > MAX_ENERGY)
			{
				energy = MAX_ENERGY;
			}
		}
	}

}

void Player::ConstrainPlayer() /* parameters to be added */
{

}