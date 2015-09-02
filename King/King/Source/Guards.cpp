#include "Guards.h"

Guards::Guards(void)
	: chase(false)
	, checkTimer(0.0)
	, stunTimer(1.0)
	, stun(false)
{
}

Guards::~Guards(void)
{
}

void Guards::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room* currentRoom, string waypoint)
{
	this->pos = pos;
	this->startPos = pos;
	this->dir = dir;
	*(this->sprite) = *(sa);
	this->tiles = tiles;
	this->currentRoom = currentRoom;
	this->waypoint = waypoint;
	this->destination = pos;
	this->changeAni(Guards_StateMachine::IDLE_STATE);
	AI::Init();
}

void Guards::Update(int worldWidth, int worldHeight, int tileSize, double dt)
{
	if(GetUpdate() == true)
	{
		if (stun && checkTimer < stunTimer)
		{
			std::cout << checkTimer << std::endl;
			checkTimer += dt;

			Character::changeAni(StateMachine::IDLE_STATE);
			this->changeAni(Guards_StateMachine::IDLE_STATE);
			
			int dir = Math::RandIntMinMax(1, 4);

			switch (dir)
			{
			case 1:
				{
					this->dir.Set(1, 0);
					break;
				}
			case 2:
				{
					this->dir.Set(-1, 0);
					break;
				}
			case 3:
				{
					this->dir.Set(0, 1);
					break;
				}
			case 4:
				{
					this->dir.Set(0, -1);
					break;
				}
			}

			if (checkTimer >= stunTimer)
			{
				stun = false;
				chase = false;
				checkTimer = 0;
			}
		}

		else
		{
			if (chase == false)
			{
				Patrolling(worldWidth, worldHeight, tileSize, dt);
			}
			else
			{
				Chasing(worldWidth, worldHeight, tileSize, dt);
			}
		}

		AI::Update(dt);
	}
}

void Guards::CheckChase(Vector2 playerPos, int tileSize, double dt)
{
	// if player is close to the guards
	if (this->CalculateTileBasedDistance(playerPos, tileSize) < this->tiles)
	{
		if (CheckSight(playerPos, tileSize, dt))
		{
			// if player is within the line of sight of the guard
			if (playerPos.x > this->pos.x && this->dir.x == 1)
			{
				this->chase = true;
			}

			else if (playerPos.x < this->pos.x && this->dir.x == -1)
			{
				this->chase = true;
			}

			else if (playerPos.y > this->pos.y && this->dir.y == 1)
			{
				this->chase = true;
			}

			else if (playerPos.y < this->pos.y && this->dir.y == -1)
			{
				this->chase = true;
			}

			else
			{
				this->chase = false;
			}
		}

		if (chase)
		{
			destination = playerPos;
		}
	}

	else
	{
		this->chase = false;
	}
}

bool Guards::CheckSight(Vector2 playerPos, int tileSize, double dt)
{	
	Vector2 lineOfSight(playerPos - this->pos);

	int numRows = 0;
	int numCols = 0;

	numCols = (int)lineOfSight.x / tileSize;
	numRows = (int)lineOfSight.y / tileSize;

	// player above guard
	if (numRows >= 0)
	{
		if (numCols >= 0)
		{
			// player is top right
			for (int i = 0; i <= numRows; ++i)
			{
				for (int j = 0; j <= numCols; ++j)
				{
					for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
					{
						if (currentRoom->specialTiles[special].TileName == "Wall" || currentRoom->specialTiles[special].TileName == "CellDoorClosed")
						{
							if (currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[currentRoom->roomLayout[TileMap::TYPE_COLLISION].getNumTilesHeight() - (int)pos.y / tileSize - i][(int) pos.x / tileSize + j] == currentRoom->specialTiles[special].TileID)
							{
								return false;
							}
						}
					}
				}
			}
		}

		else
		{
			// player is top left
			for (int i = 0; i <= numRows; ++i)
			{
				for (int j = 0; j >= numCols; --j)
				{
					for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
					{
						if (currentRoom->specialTiles[special].TileName == "Wall" || currentRoom->specialTiles[special].TileName == "CellDoorClosed")
						{
							if (currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[currentRoom->roomLayout[TileMap::TYPE_COLLISION].getNumTilesHeight() - (int)pos.y / tileSize - i][(int) pos.x / tileSize + j] == currentRoom->specialTiles[special].TileID)
							{
								return false;
							}
						}
					}
				}
			}
		}
	}

	else
	{
		// bottom right
		if (numCols >= 0)
		{
			for (int i = 0; i >= numRows; --i)
			{
				for (int j = 0; j <= numCols; ++j)
				{
					for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
					{
						if (currentRoom->specialTiles[special].TileName == "Wall" || currentRoom->specialTiles[special].TileName == "CellDoorClosed")
						{
							if (currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[currentRoom->roomLayout[TileMap::TYPE_COLLISION].getNumTilesHeight() - (int)pos.y / tileSize + i][(int) pos.x / tileSize + j] == currentRoom->specialTiles[special].TileID)
							{
								return false;
							}
						}
					}
				}
			}
		}

		else
		{
			// bottom left
			for (int i = 0; i >= numRows; --i)
			{
				for (int j = 0; j >= numCols; --j)
				{
					for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
					{
						if (currentRoom->specialTiles[special].TileName == "Wall" || currentRoom->specialTiles[special].TileName == "CellDoorClosed")
						{
							if (currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[currentRoom->roomLayout[TileMap::TYPE_COLLISION].getNumTilesHeight() - (int)pos.y / tileSize + i][(int) pos.x / tileSize + j] == currentRoom->specialTiles[special].TileID)
							{
								return false;
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void Guards::Chasing(int worldWidth, int worldHeight, int tileSize, double dt)
{
	if (Math::FAbs(destination.x - pos.x) < size.x * 0.2f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
	}

	if (Math::FAbs(destination.y - pos.y) < size.y * 0.2f)
	{
		pos.y = destination.y;
		targetPos.y = pos.y;
	}

	if (Math::FAbs(destination.y - pos.y) < size.y * 0.2f && Math::FAbs(destination.x - pos.x) < size.x * 0.2f)
	{
		pos = destination;
		targetPos = pos;
	}

	if (destination.x > pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f && Math::FAbs(targetPos.x - pos.x) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x + tileSize, targetPos.y);
	}

	else if (destination.x < pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f && Math::FAbs(targetPos.x - pos.x) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x - tileSize, targetPos.y);
	}

	if (destination.y > pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f && Math::FAbs(targetPos.y - pos.y) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x, targetPos.y + tileSize);
	}

	else if (destination.y < pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f && Math::FAbs(targetPos.y - pos.y) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x, targetPos.y - tileSize);
	}

	if (destination != pos)
	{
		changeAni(Guards_StateMachine::RUN_STATE);
		Character::changeAni(StateMachine::RUN_STATE);
	}

	else
	{
		changeAni(Guards_StateMachine::IDLE_STATE);
		Character::changeAni(StateMachine::IDLE_STATE);
	}

	this->tileBasedMovement(worldWidth, worldHeight, tileSize, dt);
}

void Guards::Patrolling(int worldWidth, int worldHeight, int tileSize, double dt)
{
	bool collide = this->tileBasedMovement(worldWidth, worldHeight, tileSize, dt);

	if (pos == destination && this->guardStateMachine.GetState() == Guards_StateMachine::IDLE_STATE || !collide)
	{
		do 
		{
			SetDestination();
		}
		while (CheckDestination() == false);
	}

	if (Math::FAbs(destination.x - pos.x) < size.x * 0.25f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
	}

	if (Math::FAbs(destination.y - pos.y) < size.y * 0.2f)
	{
		pos.y = destination.y;
		targetPos.y = pos.y;
	}

	if (Math::FAbs(destination.y - pos.y) < size.y * 0.2f && Math::FAbs(destination.x - pos.x) < size.x * 0.2f)
	{
		pos = destination;
		targetPos = pos;
	}

	else if (destination.x > pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f && Math::FAbs(targetPos.x - pos.x) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x + tileSize, targetPos.y);
	}

	else if (destination.x < pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f && Math::FAbs(targetPos.x - pos.x) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x - tileSize, targetPos.y);
	}

	else if (destination.y > pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f && Math::FAbs(targetPos.y - pos.y) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x, targetPos.y + tileSize);
	}

	else if (destination.y < pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f && Math::FAbs(targetPos.y - pos.y) < tileSize * 0.2f)
	{
		targetPos.Set(targetPos.x, targetPos.y - tileSize);
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

bool Guards::tileBasedMovement(int worldWidth, int worldHeight, int tileSize, double dt)
{
	// move right
	static bool movable = true;
	movable = true;

	Vector2 targetedLocation;
	targetedLocation.Set(targetPos.x, worldHeight - targetPos.y - tileSize);

	if ((int)targetedLocation.y / tileSize >= currentRoom->roomLayout[TileMap::TYPE_COLLISION].getNumTilesMapHeight() || (int)targetedLocation.y / tileSize < 0 || (int)targetedLocation.x / tileSize >= currentRoom->roomLayout[TileMap::TYPE_COLLISION].getNumTilesMapWidth() || (int)targetedLocation.x / tileSize < 0)
	{
		return false;
	}

	if (targetPos.x > pos.x)
	{
		dir.Set(1,0);

		for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
		{
			if (currentRoom->specialTiles[special].TileName == "Wall" ||
				currentRoom->specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorRightClosed")
			{
				int nextTile = currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];

				if (nextTile == currentRoom->specialTiles[special].TileID)
				{
					movable = false;
					break;
				}
			}
		}

		if (movable)
		{
			if (this->stateMachine.GetState() == StateMachine::WALK_STATE)
			{
				pos.x += walkSpeed * (float)dt;
			}

			else if (this->stateMachine.GetState() == StateMachine::RUN_STATE)
			{
				pos.x += runSpeed * (float)dt;
			}
		}

		else
		{
			this->targetPos = this->pos;
			this->changeAni(Guards_StateMachine::IDLE_STATE);
		}
	}

	// move left
	else if (targetPos.x < pos.x)
	{
		dir.Set(-1,0);

		for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
		{
			if (currentRoom->specialTiles[special].TileName == "Wall" ||
				currentRoom->specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorRightClosed")
			{
				int nextTile = 0;

				if (targetedLocation.x - (int)(targetedLocation.x / tileSize) * tileSize > tileSize * 0.1f)
				{
					nextTile = currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize + 1];
				}
				else
				{
					nextTile = currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];
				}

				if (nextTile == currentRoom->specialTiles[special].TileID)
				{
					movable = false;
					break;
				}
			}
		}

		if (movable)
		{
			if (this->stateMachine.GetState() == StateMachine::WALK_STATE)
			{
				pos.x -= walkSpeed * (float)dt;
			}

			else if (this->stateMachine.GetState() == StateMachine::RUN_STATE)
			{
				pos.x -= runSpeed * (float)dt;
			}
		}

		else
		{
			this->targetPos = this->pos;
			this->changeAni(Guards_StateMachine::IDLE_STATE);
		}
	}

	// move up
	else if (targetPos.y > pos.y)
	{
		dir.Set(0,1);

		for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
		{
			if (currentRoom->specialTiles[special].TileName == "Wall" ||
				currentRoom->specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorRightClosed")
			{
				int nextTile = 0;

				if (targetedLocation.y - (int)(targetedLocation.y / tileSize) * tileSize > tileSize * 0.1f)
				{
					nextTile = currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize + 1][(int)targetedLocation.x / tileSize];
				}
				else
				{
					nextTile = currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];
				}

				if (nextTile == currentRoom->specialTiles[special].TileID)
				{
					movable = false;
					break;
				}
			}
		}

		if (movable)
		{
			if (this->stateMachine.GetState() == StateMachine::WALK_STATE)
			{
				pos.y += walkSpeed * (float)dt;
			}

			else if (this->stateMachine.GetState() == StateMachine::RUN_STATE)
			{
				pos.y += runSpeed * (float)dt;
			}
		}

		else
		{
			this->targetPos = this->pos;
			this->changeAni(Guards_StateMachine::IDLE_STATE);
		}
	}

	// move down
	else if (targetPos.y < pos.y)
	{
		dir.Set(0,-1);

		for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
		{
			if (currentRoom->specialTiles[special].TileName == "Wall" ||
				currentRoom->specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom->specialTiles[special].TileName == "PrisonDoorRightClosed")
			{
				int nextTile = 0;

				nextTile = currentRoom->roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];

				if (nextTile == currentRoom->specialTiles[special].TileID)
				{
					movable = false;
					break;
				}
			}
		}

		if (movable)
		{
			if (this->stateMachine.GetState() == StateMachine::WALK_STATE)
			{
				pos.y -= walkSpeed * (float)dt;
			}

			else if (this->stateMachine.GetState() == StateMachine::RUN_STATE)
			{
				pos.y -= runSpeed * (float)dt;
			}
		}

		else
		{
			this->targetPos = this->pos;
			this->changeAni(Guards_StateMachine::IDLE_STATE);
		}
	}

	tileBasedOffset();

	return movable;
}

bool Guards::getChase (void)
{
	return chase;
}

void Guards::setChase (bool chase)
{
	this->chase = chase;
}

void Guards::setStun(bool stun)
{
	this->stun = stun;
}

bool Guards::getStun(void)
{
	return stun;
}