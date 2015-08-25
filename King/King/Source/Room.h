#ifndef ROOM_H
#define ROOM_H

#include "TileMap.h"
#include <string>

using std::string;

enum DOOR_TYPE
{
	CELLDOOR_CLOSE,
	CELLDOOR_OPEN,
	PRISONDOOR_LEFT_CLOSE,
	PRISONDOOR_RIGHT_CLOSE,
	PRISONDOOR_LEFT_OPEN,
	PRISONDOOR_RIGHT_OPEN,
	MAX_DOOR,
};

struct SpecialTiles
{
	string TileName;
	int TileID;
};

struct Door
{
	// true for open, false for close
	bool status;
	Vector2 pos;
};

struct Room
{
public:
	Room(void)
	{
		ID = -1;
		roomLayout.resize(0);
		specialTiles.clear();
	}

	~Room(void)
	{
		roomLayout.clear();
		specialTiles.clear();
	}

	int ID;
	string name;
	vector<TileMap> roomLayout;
	vector<SpecialTiles> specialTiles;
	vector<Door> doors;

	bool locateDoors(void)
	{
		if (roomLayout.size() >= TileMap::MAX_TYPE)
		{
			string doorTypes[MAX_DOOR] =
			{
				"CellDoorClosed",
				"CellDoorOpened",
				"PrisonDoorLeftClosed",
				"PrisonDoorRightClosed",
				"PrisonDoorLeftOpened",
				"PrisonDoorRightOpened",
			};

			for(int row = 0; row < roomLayout[TileMap::TYPE_COLLISION].getNumTilesMapHeight(); ++row)
			{
				for(int col = 0; col < roomLayout[TileMap::TYPE_COLLISION].getNumTilesMapWidth(); ++col)
				{
					for (unsigned special = 0; special < this->specialTiles.size(); ++special)
					{
						for (int doors = 0; doors < MAX_DOOR; ++doors)
						{
							if (this->specialTiles[special].TileName == doorTypes[doors])
							{
								if (roomLayout[TileMap::TYPE_COLLISION].screenMap[row][col] == specialTiles[special].TileID)
								{
									Door tempDoor;
									tempDoor.status = false;
									tempDoor.pos.Set(col, row);
									this->doors.push_back(tempDoor);
								}	
							}
						}
					}
				}
			}

			return true;
		}

		return false;
	};

	void rearrange(void)
	{
		for (unsigned iter = 1; iter < roomLayout.size(); ++iter)
		{
			for (unsigned index = 0; index < roomLayout.size() - iter; ++index)
			{
				if (roomLayout[index].getMapType() > roomLayout[index + 1].getMapType())
				{
					TileMap tempMap = roomLayout[index];
					roomLayout[index] = roomLayout[index + 1];
					roomLayout[index + 1] = tempMap;
				}
			}
		}
	};
};

#endif