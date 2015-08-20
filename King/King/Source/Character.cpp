#include "Character.h"

Character::Character()
	: pos(0,0)
	, vel(0,0)
	, dir(0,0)
	, targetPos(0, 0)
	, tiles(0)
	, speed(0)
	, MAX_WALK_SPEED(3.0)
	, MAX_RUN_SPEED(5.0)
{
	this->sprite = new SpriteAnimation;
	this->state.SetState(StateMachine::IDLE);
}

Character::~Character()
{
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

}

void Character::Update(double dt)
{
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(mesh);

	if(sa != NULL)
	{
		sa->Update(dt);
	}
}

/*
	if character is walking, acelerate speed to walking speed 
	if stop walking, decelerate speed to zero
	if character is running, acelerate speed to running speed 
	if stop running, decelerate speed to zero
	only one speed per character 
*/

/*void Character::MoveUp(double dt)
{
	// Walk
	if(speed < MAX_WALK_SPEED)
	{
		vel.y += (int) (speed * dt);
		pos.y += vel.y * static_cast<float>(dt);
	}

	// Run
	/*if(speed < MAX_RUN_SPEED)
	{
		vel.y += (int) (speed * dt);
		pos.y += vel.y * static_cast<float>(dt);
	}*/
//}

/*void Character::MoveDown(double dt)
{
	// Walk
	if(speed < MAX_WALK_SPEED)
	{
		vel.y -= (int) (speed * dt);
		pos.y -= vel.y * static_cast<float>(dt);
	}

	// Run
	/*if(speed < MAX_RUN_SPEED)
	{
		vel.y -= (int) (speed * dt);
		pos.y -= vel.y * static_cast<float>(dt);
	}*/
//}

/*void Character::MoveLeft(double dt)
{
	// Walk
	if(speed < MAX_WALK_SPEED)
	{
		vel.x -= (int) (speed * dt);
		pos.x -= vel.x * static_cast<float>(dt);
	}

	// Run
	/*if(speed < MAX_RUN_SPEED)
	{
		vel.x -= (int) (speed * dt);
		pos.x -= vel.x * static_cast<float>(dt);
	}*/
//}

/*void Character::MoveRight(double dt)
{
	// Walk
	if(speed < MAX_WALK_SPEED)
	{
		vel.x += (int) (speed * dt);
		pos.x += vel.x * static_cast<float>(dt);
	}

	// Run
	/*if(speed < MAX_RUN_SPEED)
	{
		vel.x += (int) (speed * dt);
		pos.x += vel.x * static_cast<float>(dt);
	}*/
}

void Character::SetFOV(int tiles)
{
	this->tiles = tiles;
}

int Character::GetFOV()
{
	return tiles;
}

/*float Character::GetPositionX(void)
{
	return pos.x;
}

float Character::GetPositionY(void)
{
	return pos.y;
}*/