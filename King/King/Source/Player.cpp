#include "Player.h"

Player::Player() :
	energy(100)
	, recovering(false)
	, attack(false)
	, MAX_ENERGY(100)
	, ENERGY_TO_RUN(25.0)
	, DEGENERATE_RUN(20)
	, DEGENERATE_ATTACK(5)
	, REGENERATE_WALK(8)
	, REGENERATE_IDLE(10) 
{
}

Player::~Player()
{
}

void Player::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room* currentRoom)
{
	this->pos = pos;
	this->startPos = pos;
	this->targetPos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->currentRoom = currentRoom;
	this->energy = MAX_ENERGY;
	this->recovering = false;
	this->changeAni(StateMachine::IDLE_STATE);
}

void Player::Update(double dt)
{
	Character::Update(dt);
	UpdateEnergy(dt);
}

void Player::setEnergy(double energy)
{
	this->energy = energy;
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

	if (attack == true)
	{
		energy -= DEGENERATE_ATTACK;
		attack = false;
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

void Player::SetAttack(bool attack)
{
	this->attack = attack;
}

bool Player::GetAttack(void)
{
	return attack;
}

void Player::ConstrainPlayer(double dt) /* parameters to be added */
{
	Vector2 playerPos;
	playerPos.Set((pos.x - size.x - currentRoom->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetX()) * (1.f / size.x), (pos.y - size.y - currentRoom->roomLayout[TileMap::TYPE_VISUAL].getMapOffsetY())  * (1.f / size.y));

	if (playerPos.x < currentRoom->roomLayout[TileMap::TYPE_VISUAL].getNumTilesWidth() * 0.5f - 1)
	{
		for (unsigned i = 0; i < currentRoom->roomLayout.size(); ++i)
		{
			if (this->getState() == StateMachine::WALK_STATE)
			{
				currentRoom->roomLayout[i].setMapOffsetX((int)(currentRoom->roomLayout[i].getMapOffsetX() - currentRoom->roomLayout[i].getScrollSpeed() * (float)dt));
			}

			else
			{
				currentRoom->roomLayout[i].setMapOffsetX((int)(currentRoom->roomLayout[i].getMapOffsetX() - currentRoom->roomLayout[i].getScrollSpeed() * runSpeed / walkSpeed * (float)dt));
			}

			if (currentRoom->roomLayout[i].getMapOffsetX() < 0)
			{
				currentRoom->roomLayout[i].setMapOffsetX(0);
			}
		}
	}

	else if (playerPos.x > currentRoom->roomLayout[TileMap::TYPE_VISUAL].getNumTilesWidth() * 0.5f + 1)
	{
		for (unsigned i = 0; i < currentRoom->roomLayout.size(); ++i)
		{
			if (this->getState() == StateMachine::WALK_STATE)
			{
				currentRoom->roomLayout[i].setMapOffsetX((int)(currentRoom->roomLayout[i].getMapOffsetX() + currentRoom->roomLayout[i].getScrollSpeed() * (float)dt));
			}

			else
			{
				currentRoom->roomLayout[i].setMapOffsetX((int)(currentRoom->roomLayout[i].getMapOffsetX() + currentRoom->roomLayout[i].getScrollSpeed() * (int)(runSpeed / walkSpeed) * (float)dt));
			}

			if (currentRoom->roomLayout[i].getMapOffsetX() > currentRoom->roomLayout[i].getMapWidth() - currentRoom->roomLayout[i].getNumTilesWidth() * currentRoom->roomLayout[i].getTileSize())
			{
				currentRoom->roomLayout[i].setMapOffsetX(currentRoom->roomLayout[i].getMapWidth() - currentRoom->roomLayout[i].getNumTilesWidth() * currentRoom->roomLayout[i].getTileSize());
			}
		}
	}

	if (playerPos.y < currentRoom->roomLayout[TileMap::TYPE_VISUAL].getNumTilesHeight() * 0.5f - 1)
	{
		for (unsigned i = 0; i < currentRoom->roomLayout.size(); ++i)
		{
			if (this->getState() == StateMachine::WALK_STATE)
			{
				currentRoom->roomLayout[i].setMapOffsetY((int)(currentRoom->roomLayout[i].getMapOffsetY() - currentRoom->roomLayout[i].getScrollSpeed() * (float)dt));
			}

			else
			{
				currentRoom->roomLayout[i].setMapOffsetY((int)(currentRoom->roomLayout[i].getMapOffsetY() - currentRoom->roomLayout[i].getScrollSpeed() * runSpeed / walkSpeed * (float)dt));
			}

			if (abs(currentRoom->roomLayout[i].getMapOffsetY()) > currentRoom->roomLayout[i].getMapHeight() - currentRoom->roomLayout[i].getNumTilesHeight() * currentRoom->roomLayout[i].getTileSize())
			{
				currentRoom->roomLayout[i].setMapOffsetY(-(currentRoom->roomLayout[i].getMapHeight() - currentRoom->roomLayout[i].getNumTilesHeight() * currentRoom->roomLayout[i].getTileSize()));
			}
		}
	}

	else if (playerPos.y > currentRoom->roomLayout[TileMap::TYPE_VISUAL].getNumTilesHeight() * 0.5f + 1)
	{
		for (unsigned i = 0; i < currentRoom->roomLayout.size(); ++i)
		{
			if (this->getState() == StateMachine::WALK_STATE)
			{
				currentRoom->roomLayout[i].setMapOffsetY((int)(currentRoom->roomLayout[i].getMapOffsetY() + currentRoom->roomLayout[i].getScrollSpeed() * (float)dt));
			}

			else
			{
				currentRoom->roomLayout[i].setMapOffsetY((int)(currentRoom->roomLayout[i].getMapOffsetY() + currentRoom->roomLayout[i].getScrollSpeed() * runSpeed / walkSpeed * (float)dt));
			}

			if (currentRoom->roomLayout[i].getMapOffsetY() > 0)
			{
				currentRoom->roomLayout[i].setMapOffsetY(0);
			}
		}
	}

	for (unsigned i = 0; i < currentRoom->roomLayout.size(); ++i)
	{
		currentRoom->roomLayout[i].Update();
	}
}