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
	int ID;
	int transitionRoom;
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