#ifndef ROOM_H
#define ROOM_H

#include "TileMap.h"

struct Room
{
public:
	Room(void)
	{
		ID = -1;
		roomLayout.resize(0);
	}

	~Room(void)
	{
		roomLayout.clear();
	}

	int ID;
	vector<TileMap> roomLayout;
};

#endif