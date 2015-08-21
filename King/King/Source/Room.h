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
};

#endif