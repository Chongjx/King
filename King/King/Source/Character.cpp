#include "Character.h"

Character::Character()
	: pos(0,0)
	, vel(0,0)
	, dir(0,0)
	, targetPos(0, 0)
	, tiles(0)
	, walkSpeed(50.f)
	, runSpeed (100.f)
	, MAX_WALK_SPEED(100.0)
	, MAX_RUN_SPEED(200.0)
{
	this->sprite = new SpriteAnimation;
	this->stateMachine.SetState(StateMachine::IDLE_STATE);
	animationList.clear();
}

Character::~Character()
{
	for (vector<Animation*>::iterator ani = animationList.begin(); ani != animationList.begin(); ++ani)
	{
		Animation* tempAni = *ani;

		if (tempAni != NULL)
		{
			delete tempAni;
			tempAni = NULL;
		}
	}

	if (this->sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}
}

void Character::Init(Vector2 pos, Vector2 dir, Vector2 vel, SpriteAnimation* sa)
{
	this->pos = pos;
	this->dir = dir;
	this->vel = vel;

	*(this->sprite) = *(sa);
}

void Character::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa)
{
	this->pos = pos;
	this->dir = dir;

	*(this->sprite) = *(sa);
}

void Character::Update(double dt)
{
	if (sprite != NULL)
	{
		sprite->Update(dt);
	}
}

/*
if character is walking, acelerate speed to walking speed 
if stop walking, decelerate speed to zero
if character is running, acelerate speed to running speed 
if stop running, decelerate speed to zero
only one speed per character 
*/

// Movement
void Character::moveUp(bool walk, double dt)
{
	this->dir.Set(0, 1);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		if(vel.y < MAX_WALK_SPEED)
		{
			vel.y += walkSpeed * (float)dt;
		}
	}
	// run
	else
	{
		this->changeAni(StateMachine::RUN_STATE);
		if(vel.y < MAX_RUN_SPEED)
		{
			vel.y += runSpeed * (float)dt;
		}
	}

	pos.y += vel.y * (float)(dt);
}

void Character::moveDown(bool walk, double dt)
{
	this->dir.Set(0, -1);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		if(Math::FAbs(vel.y) < MAX_WALK_SPEED)
		{
			vel.y -= walkSpeed * (float)dt;
		}
	}
	// run
	else
	{
		this->changeAni(StateMachine::RUN_STATE);
		if(Math::FAbs(vel.y) < MAX_RUN_SPEED)
		{
			vel.y -= runSpeed * (float)dt;
		}
	}

	pos.y -= vel.y * (float)(dt);
}

void Character::moveLeft(bool walk, double dt)
{
	this->dir.Set(-1, 0);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		if(Math::FAbs(vel.x) < MAX_WALK_SPEED)
		{
			vel.x -= walkSpeed * (float)dt;
		}
	}
	// run
	else
	{
		this->changeAni(StateMachine::RUN_STATE);
		if(Math::FAbs(vel.x) < MAX_RUN_SPEED)
		{
			vel.x -= runSpeed * (float)dt;
		}
	}

	pos.x -= vel.x * (float)(dt);
}

void Character::moveRight(bool walk, double dt)
{
	this->dir.Set(1, 0);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		if(vel.x < MAX_WALK_SPEED)
		{
			vel.x += walkSpeed * (float)dt;
		}
	}
	// run
	else
	{
		this->changeAni(StateMachine::RUN_STATE);
		if(vel.x < MAX_RUN_SPEED)
		{
			vel.x += runSpeed * (float)dt;
		}
	}

	pos.x += vel.x * (float)(dt);
}

void Character::SetFOV(int tiles)
{
	this->tiles = tiles;
}

int Character::GetFOV()
{
	return tiles;
}

void Character::setPos(Vector2 pos)
{
	this->pos = pos;
}

void Character::setDir(Vector2 dir)
{
	this->dir = dir;
}

void Character::setVel(Vector2 vel)
{
	this->vel = vel;
}

void Character::setSize(Vector2 size)
{
	this->size = size;
}

void Character::setTargetPos(Vector2 targetPos)
{
	this->targetPos = targetPos;
}

void Character::setState(StateMachine::STATE unitState)
{
	changeAni(unitState);
}

void Character::setSprite(SpriteAnimation *sa)
{
	*(this->sprite) = *sa;
}

Vector2 Character::getPos(void) const
{
	return this->pos;
}

Vector2 Character::getDir(void) const
{
	return this->dir;
}

Vector2 Character::getVel(void) const
{
	return this->vel;
}

Vector2 Character::getSize(void) const
{
	return this->size;
}

Vector2 Character::getTargetPos(void) const
{
	return this->targetPos;
}

StateMachine::STATE Character::getState(void) const
{
	return stateMachine.GetState();
}

SpriteAnimation* Character::getSprite(void) const
{
	return this->sprite;
}

/*bool Character::moveTo(Vector2 destination)
{

}*/

CInventory Character::getInventory(void) const
{
	return this->inventory;
}

void Character::changeAni(StateMachine::STATE unitState)
{
	this->stateMachine.SetState(unitState);

	if (sprite != NULL)
	{
		switch(unitState)
		{
		case StateMachine::IDLE_STATE:
			{
				if (this->dir.x == 1)
				{
					this->currentAni = IDLE_RIGHT;
				}

				else if (this->dir.x == -1)
				{
					this->currentAni = IDLE_LEFT;
				}

				else if (this->dir.y == 1)
				{
					this->currentAni = IDLE_UP;
				}

				else if (this->dir.y == -1)
				{
					this->currentAni = IDLE_DOWN;
				}
				break;
			}

		case StateMachine::WALK_STATE:
			{
				if (this->dir.x == 1)
				{
					this->currentAni = WALK_RIGHT;
				}

				else if (this->dir.x == -1)
				{
					this->currentAni = WALK_LEFT;
				}

				else if (this->dir.y == 1)
				{
					this->currentAni = WALK_UP;
				}

				else if (this->dir.y == -1)
				{
					this->currentAni = WALK_DOWN;
				}
				break;
			}

		case StateMachine::RUN_STATE:
			{
				if (this->dir.x == 1)
				{
					this->currentAni = RUN_RIGHT;
				}

				else if (this->dir.x == -1)
				{
					this->currentAni = RUN_LEFT;
				}

				else if (this->dir.y == 1)
				{
					this->currentAni = RUN_UP;
				}

				else if (this->dir.y == -1)
				{
					this->currentAni = RUN_DOWN;
				}
				break;
			}

		case StateMachine::TALK_STATE:
			{
				if (this->dir.x == 1)
				{
					this->currentAni = IDLE_RIGHT;
				}

				else if (this->dir.x == -1)
				{
					this->currentAni = IDLE_LEFT;
				}

				else if (this->dir.y == 1)
				{
					this->currentAni = IDLE_UP;
				}

				else if (this->dir.y == -1)
				{
					this->currentAni = IDLE_DOWN;
				}
				break;
			}

		case StateMachine::SLEEP_STATE:
			{
				if (this->dir.x == 1)
				{
					this->currentAni = IDLE_RIGHT;
				}

				else if (this->dir.x == -1)
				{
					this->currentAni = IDLE_LEFT;
				}

				else if (this->dir.y == 1)
				{
					this->currentAni = IDLE_UP;
				}

				else if (this->dir.y == -1)
				{
					this->currentAni = IDLE_DOWN;
				}
				break;
			}
		}

		sprite->currentAni = currentAni;
	}
}