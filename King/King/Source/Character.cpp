#include "Character.h"

Character::Character()
	: pos(0,0)
	, vel(0,0)
	, dir(0,0)
	, targetPos(0, 0)
	, tiles(1)
	, walkSpeed(400.f)
	, runSpeed (800.f)
	, MAX_WALK_SPEED(25.0)
	, MAX_RUN_SPEED(20.0)
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
	this->targetPos = pos;
	this->dir = dir;
	this->vel = vel;

	*(this->sprite) = *(sa);
}

void Character::Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa)
{
	this->pos = pos;
	this->targetPos = pos;
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
bool Character::moveTo(void)
{
	return true;
}

// Movement
void Character::moveUp(bool walk, double dt)
{
	this->dir.Set(0, 1);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		vel.y = walkSpeed;
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

	vel.x = 0;
	pos.y += vel.y;
}

void Character::moveDown(bool walk, double dt)
{
	this->dir.Set(0, -1);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		vel.y = -walkSpeed;
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

	vel.x = 0;
	pos.y += vel.y;
}

void Character::moveLeft(bool walk, double dt)
{
	this->dir.Set(-1, 0);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		vel.x = -walkSpeed;
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

	vel.y = 0;
	pos.x += vel.x;
}

void Character::moveRight(bool walk, double dt)
{
	this->dir.Set(1, 0);
	// walk
	if (walk)
	{
		this->changeAni(StateMachine::WALK_STATE);
		vel.x = walkSpeed;
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

	vel.y = 0;
	pos.x += vel.x;
}

void Character::tileBasedMovement(int worldWidth, int worldHeight, int tileSize, double dt)
{
	// move right
	static bool movable = true;
	movable = true;

	if (targetPos.x > pos.x)
	{
		Vector2 targetedLocation;
		targetedLocation.Set(targetPos.x, worldHeight - targetPos.y - tileSize);

		for (unsigned special = 0; special < currentRoom.specialTiles.size(); ++special)
		{
			if (currentRoom.specialTiles[special].TileName == "Wall" /*|| 
				/*currentRoom.specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorRightClosed"*/)
			{
				int nextTile = currentRoom.roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];

				if (nextTile == currentRoom.specialTiles[special].TileID)
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
			this->changeAni(StateMachine::IDLE_STATE);
		}
	}

	// move left
	else if (targetPos.x < pos.x)
	{
		Vector2 targetedLocation;
		targetedLocation.Set(targetPos.x, worldHeight - targetPos.y - tileSize);

		for (unsigned special = 0; special < currentRoom.specialTiles.size(); ++special)
		{
			if (currentRoom.specialTiles[special].TileName == "Wall" /*|| 
				/*currentRoom.specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorRightClosed"*/)
			{
				int nextTile = currentRoom.roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];

				if (nextTile == currentRoom.specialTiles[special].TileID)
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
			this->changeAni(StateMachine::IDLE_STATE);
		}
	}

	// move up
	if (targetPos.y > pos.y)
	{
		Vector2 targetedLocation;
		targetedLocation.Set(targetPos.x, worldHeight - targetPos.y - tileSize);

		for (unsigned special = 0; special < currentRoom.specialTiles.size(); ++special)
		{
			if (currentRoom.specialTiles[special].TileName == "Wall" /*|| 
				/*currentRoom.specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorRightClosed"*/)
			{
				int nextTile = currentRoom.roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];

				if (nextTile == currentRoom.specialTiles[special].TileID)
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
			this->changeAni(StateMachine::IDLE_STATE);
		}
	}

	// move down
	else if (targetPos.y < pos.y)
	{
		Vector2 targetedLocation;
		targetedLocation.Set(targetPos.x, worldHeight - targetPos.y - tileSize);

		for (unsigned special = 0; special < currentRoom.specialTiles.size(); ++special)
		{
			if (currentRoom.specialTiles[special].TileName == "Wall" /*|| 
				/*currentRoom.specialTiles[special].TileName == "CellDoorClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorLeftClosed" ||
				currentRoom.specialTiles[special].TileName == "PrisonDoorRightClosed"*/)
			{
				int nextTile = currentRoom.roomLayout[TileMap::TYPE_COLLISION].screenMap[(int)targetedLocation.y / tileSize][(int)targetedLocation.x / tileSize];

				if (nextTile == currentRoom.specialTiles[special].TileID)
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
			this->changeAni(StateMachine::IDLE_STATE);
		}
	}

	tileBasedOffset();
}

void Character::tileBasedOffset(void)
{
	if (Math::FAbs(targetPos.x - pos.x) < size.x * 0.2f)
	{
		pos.x = targetPos.x;
	}

	if (Math::FAbs(targetPos.y - pos.y) < size.y * 0.2f)
	{
		pos.y = targetPos.y;
	}

	if (pos == targetPos)
	{
		this->changeAni(StateMachine::IDLE_STATE);
	}
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

	if (targetPos.x > pos.x)
	{
		dir.Set(1, 0);
	}

	// move left
	else if (targetPos.x < pos.x)
	{
		dir.Set(-1, 0);
	}

	// move up
	else if (targetPos.y > pos.y)
	{
		dir.Set(0, 1);
	}

	// move down
	else if (targetPos.y < pos.y)
	{
		dir.Set(0, -1);
	}
}

void Character::setState(StateMachine::STATE unitState)
{
	changeAni(unitState);
}

void Character::setSprite(SpriteAnimation *sa)
{
	*(this->sprite) = *sa;
}

void Character::setAni(SPRITE_ANI currentAni)
{
	this->currentAni = currentAni;
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

void Character::setRoom(Room& room)
{
	this->currentRoom = room;
}

Room Character::getRoom(void)
{
	return this->currentRoom;
}

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

				this->vel.SetZero();
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

bool Character::collideWithDoor(void)
{
	for (unsigned i = 0; i < currentRoom.doors.size(); ++i)
	{
		if (pos == currentRoom.doors[i].pos)
		{
			return true;
		}
	}

	return false;
}