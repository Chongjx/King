#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using std::vector;
using std::ifstream;
using std::fstream;
using std::string;
using std::getline;
using std::istringstream;

class TileMap
{
public:
	enum MAP_TYPE
	{
		TYPE_BACKGROUND,
		TYPE_COLLISION,
		TYPE_VISUAL,
		TYPE_WAYPOINT,
		MAX_TYPE,
	};

	TileMap(void);
	~TileMap(void);

	void Init(const int ID, const int screenWidth, const int screenHeight, const int mapWidth, const int mapHeight, const int mapOffsetX = 0, const int mapOffsetY = 0, const int mapFineOffsetX = 0, const int mapFineOffsetY = 0,  const bool enableXScrolling = false, const bool enableYScrolling = false, float scrollSpeed = 0, const int tileSize = 32);
	void InitDynamic(int ID, int screenWidth, int screenHeight, int numTilesWidth, int numTilesHeight, int mapOffsetX = 0, int mapOffsetY = 0, int mapFineOffsetX = 0, int mapFineOffsetY = 0, bool enableXscrolling = false, bool enableYScrolling = false, float scrollSpeed = 0, int tileSize = 32);
	bool LoadMap(const string mapName);

	void setBackgroundID(int ID);

	int getID(void);
	int getBackgroundID(void);
	int getScreenWidth(void);
	int getScreenHeight(void);
	int getNumTilesWidth(void);
	int getNumTilesHeight(void);
	int getMapWidth(void);
	int getMapHeight(void);
	int getNumTilesMapWidth(void);
	int getNumTilesMapHeight(void);
	int getTileSize(void);

	int getMapOffsetX(void);
	int getMapOffsetY(void);
	int getTileOffsetX(void);
	int getTileOffsetY(void);
	int getMapFineOffsetX(void);
	int getMapFineOffsetY(void);
	bool getMapXScroll(void);
	bool getMapYScroll(void);
	float getScrollSpeed(void);
	MAP_TYPE getMapType(void);

	void setMapOffsetX(int mapOffsetX);
	void setMapOffsetY(int mapOffsetY);
	void setXScroll(bool enabled);
	void setYScroll(bool enabled);
	void setScrollSpeed(float scrollSpeed);
	void setMapType(MAP_TYPE mapType);

	void Update(void);

	vector<vector<int>> screenMap;
private:
	int ID;
	int backgroundID;
	int screenWidth;
	int screenHeight;
	int numTilesWidth;
	int numTilesHeight;
	int mapWidth;
	int mapHeight;
	int numTilesMapWidth;
	int numTilesMapHeight;
	int tileSize;

	int mapOffsetX;
	int mapOffsetY;
	int tileOffsetX;
	int tileOffsetY;
	int mapFineOffsetX;
	int mapFineOffsetY;
	
	bool xScrolling;
	bool yScrolling;
	float scrollSpeed;

	MAP_TYPE mapType;

	bool loadFile(const string mapName);
};

#endif