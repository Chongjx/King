#include "AI.h"

AI::AI(void)
	: destination(0,0)
	, updating(true)
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
	destination.x = Math::RandIntMinMax(0,currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].getNumTilesMapWidth());
	destination.y = Math::RandIntMinMax(0,currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].getNumTilesMapHeight());

	CheckDestination();

	if(CheckDestination() == false)
	{
		SetDestination();
	}
}
	
Vector2 AI::GetDestination(void)
{
	return destination;
}

bool AI::CheckDestination(void)
{
	for (unsigned special = 0; special < currentRoom.specialTiles.size(); ++special)
	{
		if (currentRoom.specialTiles[special].TileName == "A" ||
			currentRoom.specialTiles[special].TileName == "B" ||
			currentRoom.specialTiles[special].TileName == "C"  )
		{
			if(currentRoom.roomLayout[TileMap::TYPE_WAYPOINT].screenMap[destination.y][destination.x] == currentRoom.specialTiles[special].TileID)
			{
				return true;
			}
		}
	}

	return false;
}

void AI::PathFinding(int worldWidth, int worldHeight, int tileSize, double dt)
{
	Vector2 targetedLocation;
	targetedLocation.Set(targetPos.x, worldHeight - targetPos.y - tileSize);
}

void AI::changeAni(StateMachine::STATE unitState)
{
}