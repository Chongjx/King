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

void Player::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, int mapLocation)
{
	this->pos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->mapLocation = mapLocation;
	this->energy = MAX_ENERGY;
	this->changeAni(StateMachine::IDLE_STATE);
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
	if (Character::stateMachine.GetState() == StateMachine::RUN_STATE)
	{
		if ( energy > 0 )
		{
			//decrease energy
			energy -= 10 * dt;
		}
		else 
		{
			energy = 0;
			Character::stateMachine.SetState(StateMachine::WALK_STATE);
		}
	}

	else if (Character::stateMachine.GetState() == StateMachine::WALK_STATE)
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

	else if (Character::stateMachine.GetState() == StateMachine::IDLE_STATE)
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