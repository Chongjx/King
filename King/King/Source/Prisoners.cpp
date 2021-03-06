#include "Prisoners.h"

Prisoners::Prisoners(void)
	: randTimer(0.0)
	, talking(false)
{

}

Prisoners::~Prisoners(void)
{

}

void Prisoners::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room* currentRoom, string waypoint)
{
	this->pos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->currentRoom = currentRoom;
	this->waypoint = waypoint;
	this->changeAni(Prisoners_StateMachine::IDLE_STATE);
	AI::Init();
}

void Prisoners::Update(int worldWidth, int worldHeight, int tileSize, double dt)
{
	if(GetUpdate() == true)
	{
		AI::Update(dt);
		tileBasedMovement(worldWidth, worldHeight, tileSize, dt);
		
		Patrolling(tileSize, dt);
	}
}

void Prisoners::SetTalking(bool talking)
{
	this->talking = talking;
}

bool Prisoners::GetTalking(void)
{
	return talking;
}

void Prisoners::Patrolling(int tileSize, double dt)
{
	randTimer += dt;

	if (randTimer > Math::RandFloatMinMax(1.f,4.f))
	{
		randTimer = 0.0;

		int lepak = rand() % 5;

		if(lepak == 0)
		{
			targetPos.Set(targetPos.x, targetPos.y - tileSize);
		}
		else if(lepak == 1)
		{
			targetPos.Set(targetPos.x, targetPos.y + tileSize);
		}
		else if(lepak == 2)
		{
			targetPos.Set(targetPos.x - tileSize, targetPos.y);
		}
		else if(lepak == 3)
		{
			targetPos.Set(targetPos.x + tileSize, targetPos.y);
		}
		else if(lepak == 4)
		{
			int faceWhere = rand() % 2;

			if(faceWhere == 0)
			{
				dir.Set((float)Math::RandIntMinMax(-1, 1), 0);
			}
			else
			{
				dir.Set(0, (float)Math::RandIntMinMax(-1, 1));
			}
		}
	}

	if (targetPos != pos)
	{
		changeAni(Prisoners_StateMachine::WALK_STATE);
		Character::changeAni(StateMachine::WALK_STATE);
	}

	else
	{
		changeAni(Prisoners_StateMachine::IDLE_STATE);
		Character::changeAni(StateMachine::IDLE_STATE);
	}
}

void Prisoners::changeAni(Prisoners_StateMachine::PRISONER_STATE unitState)
{
	this->prisonerStateMachine.SetState(unitState);

	if (sprite != NULL)
	{
		switch(unitState)
		{
		case Prisoners_StateMachine::IDLE_STATE:
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

				this->vel.SetZero();
				break;
			}

		case Prisoners_StateMachine::WALK_STATE:
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

		case Prisoners_StateMachine::RUN_STATE:
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

		case Prisoners_StateMachine::TALK_STATE:
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

		case Prisoners_StateMachine::SLEEP_STATE:
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