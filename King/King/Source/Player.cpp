#include "Player.h"

Player::Player() :
	: mapLocation(0)
	, energy(0)
	, recovering(false)
	, MAX_ENERGY(100)
	, ENERGY_TO_RUN(30.0)
	, DEGENERATE_RUN(10)
	, REGENERATE_WALK(5)
	, REGENERATE_IDLE(7.5) 
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
	this->recovering = false;
	this->changeAni(StateMachine::IDLE_STATE);
}

void Player::Update(double dt)
{
	Character::Update(dt);
	UpdateEnergy(dt);
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
			energy -= DEGENERATE_RUN * dt;
		}
		else
		{
			energy = 0;
			Recovering();
		}
	}

	else if (Character::stateMachine.GetState() == StateMachine::WALK_STATE)
	{
		if ( energy < MAX_ENERGY )
		{
			//recover energy
			energy += REGENERATE_WALK * dt;
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
			energy += REGENERATE_IDLE * dt;
			//cap energy at max energy
			if (energy > MAX_ENERGY)
			{
				energy = MAX_ENERGY;
			}
		}
	}

	if ( recovering == true )
	{
		if(energy > ENERGY_TO_RUN)
		{
			recovering = false;
		}
	}
}

void Player::Recovering()
{
	changeAni(StateMachine::WALK_STATE);
	recovering = true;
}

bool Player::GetRecovering(void)
{
	return recovering;
}

void Player::ConstrainPlayer() /* parameters to be added */
{

}
