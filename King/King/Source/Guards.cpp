#include "Guards.h"

Guards::Guards(void)
	: chase(false)
	, checkTimer(0.0)
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

	for (int i = 0; i < 100; ++i)
	{
		GhettoParticle *po = new GhettoParticle(GhettoParticle::PO_DETECTOR);
		m_poList.push_back(po);
	}

}

void Guards::Update(int worldWidth, int worldHeight, int tileSize, double dt)
{
	if(GetUpdate() == true)
	{
		if (chase == false)
		{
			Patrolling(worldWidth, worldHeight, tileSize, dt);
		}
		else
		{
			Chasing(worldWidth, worldHeight, tileSize, dt);
		}

		AI::Update(dt);
	}
}

void Guards::CheckChase(Vector2 playerPos, int tileSize, double dt)
{
	// if player is close to the guards
	if (this->CalculateTileBasedDistance(playerPos, tileSize) < this->tiles)
	{
		//if (CheckSight(playerPos, dt) == true)
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

GhettoParticle* Guards::FetchPO()
{
	for (std::vector<GhettoParticle *>::iterator it = m_poList.begin(); it != m_poList.end(); ++it)
	{
		GhettoParticle *po = (GhettoParticle *) * it;
		if (po->active == false)
		{
			po->active = true;
			return po;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		GhettoParticle *po = new GhettoParticle(GhettoParticle::PO_DETECTOR);
		m_poList.push_back(po);
	}
	return m_poList[m_poList.size() - 1];
}

bool Guards::CheckSight(Vector2 playerPos, double dt)
{	
	checkTimer += dt;
	if( checkTimer > 0.15 )
	{
		GhettoParticle *Spawn = FetchPO(); //create object
		Spawn->type = GhettoParticle::PO_DETECTOR;
		Spawn->scale.Set(3, 3);
		Spawn->vel.Set(this->pos.x - playerPos.x, this->pos.y - playerPos.y);
		checkTimer = 0.0;
	}

	// Checks through game object list
	for(std::vector<GhettoParticle *>::iterator it = m_poList.begin(); it != m_poList.end(); ++it)
	{
		GhettoParticle *po = (GhettoParticle *)*it;

		if(po->active)
		{	
			if(po->type == GhettoParticle::PO_DETECTOR)
			{
				po->vel += pos - playerPos;
				po->pos += po->vel * (float)dt;
				std::cout << "active detector\n";
				for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
				{
					if (currentRoom->specialTiles[special].TileName == "Wall" ||
						currentRoom->specialTiles[special].TileName == "CellDoorClosed" ||
						currentRoom->specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
						currentRoom->specialTiles[special].TileName == "PrisonDoorRightClosed")
					{
						/*if (po->pos == currentRoom->specialTiles[special].TileID)
						{
							po->active = false;
							return false;
						}*/
					}
				}

				if(po->pos == playerPos)
				{
					po->active = false;
					return true;
				}
			}
		}
	}
}

void Guards::Chasing(int worldWidth, int worldHeight, int tileSize, double dt)
{
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
		targetPos.Set(targetPos.x + tileSize, targetPos.y);
	}

	else if (destination.x < pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f)
	{
		targetPos.Set(targetPos.x - tileSize, targetPos.y);
	}

	else if (destination.y > pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
		targetPos.Set(targetPos.x, targetPos.y + tileSize);
	}

	else if (destination.y < pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
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
		/*if (checkNextTile(Vector2(targetPos.x + tileSize, targetPos.y)))
		{
			targetPos.Set(targetPos.x + tileSize, targetPos.y);
		}
		else
		{
			if(destination.y > pos.y)
			{
				targetPos.Set(targetPos.x, targetPos.y + tileSize);
			}

			else
			{
				targetPos.Set(targetPos.x, targetPos.y - tileSize);
			}
		}*/
		targetPos.Set(targetPos.x + tileSize, targetPos.y);
	}

	else if (destination.x < pos.x && Math::FAbs(destination.x - pos.x) > size.x * 0.2f)
	{
		targetPos.Set(targetPos.x - tileSize, targetPos.y);
	}

	else if (destination.y > pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
		targetPos.Set(targetPos.x, targetPos.y + tileSize);
	}

	else if (destination.y < pos.y && Math::FAbs(destination.y - pos.y) > size.y * 0.2f)
	{
		pos.x = destination.x;
		targetPos.x = pos.x;
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

	//this->tileBasedMovement(worldWidth, worldHeight, tileSize, dt);
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

	std::cout << targetedLocation << std::endl;

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