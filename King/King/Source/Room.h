#ifndef ROOM_H
#define ROOM_H

#include "TileMap.h"
#include <string>

using std::string;

struct SpecialTiles
{
	string TileName;
	int TileID;
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
	vector<TileMap> roomLayout;
	vector<SpecialTiles> specialTiles;

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