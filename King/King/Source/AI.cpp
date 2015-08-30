#include "AI.h"

AI::AI(void)
	: destination(0,0)
	, updating(true)
	, waypoint("A")
{
}

AI::~AI(void)
{
}

void AI::Init()
{
}

void AI::Update(double dt)
{
	Character::Update(dt);
}

void AI::SetUpdate(bool updating)
{
	this->updating = updating;
}

bool AI::GetUpdate(void)
{
	return updating;
}

void AI::SetDestination(void)
{
	destination.x = (float)Math::RandIntMinMax(0, currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getNumTilesMapWidth() - 1) * currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getTileSize();
	destination.y = currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getScreenHeight() - (float)Math::RandIntMinMax(0, currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getNumTilesMapHeight() - 1) * currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getTileSize();
}

Vector2 AI::GetDestination(void)
{
	return destination;
}

bool AI::CheckDestination(void)
{
	for (unsigned special = 0; special < currentRoom->specialTiles.size(); ++special)
	{
		if (currentRoom->specialTiles[special].TileName == waypoint)
		{
			if(currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].screenMap[currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getNumTilesHeight() - destination.y * (1.f / currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getTileSize())][(int)(destination.x * (1.f / currentRoom->roomLayout[TileMap::TYPE_WAYPOINT].getTileSize()))] == currentRoom->specialTiles[special].TileID)
			{
				return true;
			}
		}
	}

	return false;
}

void AI::Patrolling(int worldWidth, int worldHeight, int tileSize, double dt)
{
}

void AI::changeAni(StateMachine::STATE unitState)
{
}