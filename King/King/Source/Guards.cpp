#include "Guards.h"

Guards::Guards(void)
{
}

Guards::~Guards(void)
{
}

void Guards::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room& currentRoom, string waypoint)
{
	this->pos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->currentRoom = currentRoom;
	this->waypoint = waypoint;
	this->destination = pos;
	this->changeAni(Guards_StateMachine::IDLE_STATE);
	AI::Init();
}

void Guards::Update(double dt)
{
	if(GetUpdate() == true)
	{
		AI::Update(dt);
	}
}

void Guards::PathFinding(int worldWidth, int worldHeight, int tileSize, double dt)
{
	bool collide = tileBasedMovement(worldWidth, worldHeight, tileSize, dt);

	if (pos == destination && this->guardStateMachine.GetState() == Guards_StateMachine::IDLE_STATE || !collide)
	{
		do 
		{
			SetDestination();
		}
		while (CheckDestination() == false);
	}

	if (Math::FAbs(destination.y - pos.y) < size.y * 0.2f)
	{
		pos.y = destination.y;
		targetPos.y = pos.y;
	}

	if (Math::FAbs(destination.x - pos.x) < size.x * 0.2f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
	}

	if (Math::FAbs(destination.y - pos.y) < size.y * 0.2f && Math::FAbs(destination.x - pos.x) < size.x * 0.2f)
	{
		pos = destination;
		targetPos = pos;
	}

	else if (destination.x > pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f)
	{
		targetPos.Set(pos.x + currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].getTileSize(), pos.y);
	}

	else if (destination.x < pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f)
	{
		targetPos.Set(pos.x - currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].getTileSize(), pos.y);
	}

	else if (destination.y > pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f)
	{
		pos.x = destination.x;
		targetPos.Set(pos.x, pos.y + currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].getTileSize());
	}

	else if (destination.y < pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f)
	{
		pos.x = destination.x;
		targetPos.Set(pos.x, pos.y - currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].getTileSize());
	}

	if (destination != pos)
	{
		changeAni(Guards_StateMachine::WALK_STATE);
		Character::changeAni(StateMachine::WALK_STATE);
	}

	else
	{
		changeAni(Guards_StateMachine::IDLE_STATE);
		Character::changeAni(StateMachine::IDLE_STATE);
	}

	Character::tileBasedOffset();
}

void Guards::changeAni(Guards_StateMachine::GUARD_STATE unitState)
{
	this->guardStateMachine.SetState(unitState);

	if (sprite != NULL)
	{
		switch(unitState)
		{
		case Guards_StateMachine::IDLE_STATE:
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

		case Guards_StateMachine::WALK_STATE:
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

		case Guards_StateMachine::RUN_STATE:
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

		case Guards_StateMachine::TALK_STATE:
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

		case Guards_StateMachine::SLEEP_STATE:
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

			case Guards_StateMachine::CHASE_STATE:
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

			case Guards_StateMachine::PATROL_STATE:
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

			case Guards_StateMachine::RETURN_STATE:
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