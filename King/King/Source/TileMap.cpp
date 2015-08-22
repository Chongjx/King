#include "TileMap.h"

TileMap::TileMap(void) :
screenWidth(0),
	screenHeight(0),
	numTilesHeight(0),
	numTilesWidth(0),
	mapWidth(0),
	mapHeight(0),
	numTilesMapWidth(0),
	numTilesMapHeight(0),
	tileSize(0),
	mapOffsetX(0),
	mapOffsetY(0),
	tileOffsetX(0),
	tileOffsetY(0),
	mapFineOffsetX(0),
	mapFineOffsetY(0),
	xScrolling(0),
	yScrolling(0),
	backgroundID(0),
	ID(0)
{
	screenMap.clear();
}

TileMap::~TileMap(void)
{
	screenMap.clear();
}

void TileMap::Init(const int ID, const int screenWidth, const int screenHeight, const int mapWidth, const int mapHeight, const int mapOffsetX, const int mapOffsetY, const int mapFineOffsetX, const int mapFineOffsetY, const bool enableXScrolling, const bool enableYScrolling, const int tileSize)
{
	this->ID = ID;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
	this->mapOffsetX = mapOffsetX;
	this->mapOffsetY = mapOffsetY;
	this->mapFineOffsetX = mapFineOffsetX;
	this->mapFineOffsetY = mapFineOffsetY;
	this->xScrolling = enableXScrolling;
	this->yScrolling = enableYScrolling;
	this->tileSize = tileSize;

	this->numTilesWidth = (int)(screenWidth * (1.f/ tileSize));
	this->numTilesHeight = (int)(screenHeight * (1.f/tileSize));

	this->numTilesMapWidth = (int) (mapWidth / tileSize);
	this->numTilesMapHeight = (int) (mapHeight / tileSize);

	screenMap.resize(numTilesMapHeight);

	for(int i = 0; i < numTilesMapHeight; ++i)
	{
		screenMap[i].resize(numTilesMapWidth);
	}

	this->backgroundID = 0;
	this->mapType = TYPE_VISUAL;
}

void TileMap::InitDynamic(int ID, int screenWidth, int screenHeight, int numTilesWidth, int numTilesHeight, int mapOffsetX, int mapOffsetY, bool enableXscrolling, bool enableYScrolling, int tileSize)
{
	this->ID = ID;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->numTilesWidth = numTilesWidth;
	this->numTilesHeight = numTilesHeight;
	this->mapOffsetX = mapOffsetX;
	this->mapOffsetY = mapOffsetY;
	this->mapFineOffsetX = mapFineOffsetX;
	this->mapFineOffsetY = mapFineOffsetY;
	this->xScrolling = enableXScrolling;
	this->yScrolling = enableYScrolling;
	this->tileSize = tileSize;

	this->numTilesWidth = (int)(screenWidth * (1.f/ tileSize));
	this->numTilesHeight = (int)(screenHeight * (1.f/tileSize));

	this->numTilesMapWidth = (int) (mapWidth / tileSize);
	this->numTilesMapHeight = (int) (mapHeight / tileSize);

	screenMap.resize(numTilesMapHeight);

	for(int i = 0; i < numTilesMapHeight; ++i)
	{
		screenMap[i].resize(numTilesMapWidth);
	}

	this->backgroundID = 0;
	this->mapType = TYPE_VISUAL;
}

bool TileMap::LoadMap(const string mapName)
{
	if (loadFile(mapName))
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool TileMap::loadFile(const string mapName)
{
	int lineCounter = 0;
	int maxNumColumn = 0;

	ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;

		while(file.good())
		{
			string lineOfText = "";
			getline(file, lineOfText);

			// if this line is not a comment line, then process it
			if(!(lineOfText.find("//*") == NULL) && lineOfText != "")
			{
				string token;
				istringstream iss(lineOfText);
				int columnCounter = 0;

				while (getline(iss, token, ',') && columnCounter < numTilesMapWidth)
				{
					screenMap[lineCounter][columnCounter++] = atoi(token.c_str());
				}
				lineCounter++;
			}
		}
	}
	return true;
}

void TileMap::setBackgroundID(int ID)
{
	this->backgroundID = ID;
}

int TileMap::getID(void)
{
	return this->ID;
}

int TileMap::getBackgroundID(void)
{
	return this->backgroundID;
}

int TileMap::getScreenWidth(void)
{
	return this->screenWidth;
}

int TileMap::getScreenHeight(void)
{
	return this->screenHeight;
}

int TileMap::getNumTilesWidth(void)
{
	return this->numTilesWidth;
}

int TileMap::getNumTilesHeight(void)
{
	return this->numTilesHeight;
}

int TileMap::getMapWidth(void)
{
	return this->mapWidth;
}

int TileMap::getMapHeight(void)
{
	return this->mapHeight;
}

int TileMap::getNumTilesMapWidth(void)
{
	return this->numTilesMapWidth;
}

int TileMap::getNumTilesMapHeight(void)
{
	return this->numTilesMapHeight;
}

int TileMap::getTileSize(void)
{
	return this->tileSize;
}

int TileMap::getMapOffsetX(void)
{
	return this->mapOffsetX;
}

int TileMap::getMapOffsetY(void)
{
	return this->mapOffsetY;
}

int TileMap::getTileOffsetX(void)
{
	return this->tileOffsetX;
}

int TileMap::getTileOffsetY(void)
{
	return this->tileOffsetY;
}

int TileMap::getMapFineOffsetX(void)
{
	return this->mapFineOffsetX;
}

int TileMap::getMapFineOffsetY(void)
{
	return this->mapFineOffsetY;
}

TileMap::MAP_TYPE TileMap::getMapType(void)
{
	return this->mapType;
}

void TileMap::setMapOffsetX(int mapOffsetX)
{
	this->mapOffsetX = mapOffsetX;
}

void TileMap::setMapOffsetY(int mapOffsetY)
{
	this->mapOffsetY = mapOffsetY;
}

void TileMap::setXScroll(bool enabled)
{
	this->xScrolling = enabled;
}

void TileMap::setYScroll(bool enabled)
{
	this->yScrolling = enabled;
}

void TileMap::setMapType(TileMap::MAP_TYPE mapType)
{
	this->mapType = mapType;
}

void TileMap::Update(void)
{
	// fine scrolling for x-axis
	if (xScrolling)
	{
		this->tileOffsetX = (int) (this->mapOffsetX / this->tileSize);

		if (this->tileOffsetX + this->numTilesWidth > this->numTilesMapWidth)
		{
			this->tileOffsetX = this->numTilesMapWidth - this->numTilesWidth;
		}

		this->mapFineOffsetX = this->mapOffsetX % this->tileSize;
	}
	
	// fine scrolling for y-axis
	if (yScrolling)
	{
		this->tileOffsetY = (int) (this->mapOffsetY / this->tileSize);
		if (this->tileOffsetY + this->numTilesHeight > this->numTilesMapHeight)
		{
			this->tileOffsetY = this->numTilesMapHeight - this->numTilesHeight;
		}

	this->mapFineOffsetY = this->mapOffsetY % this->tileSize;
	}
}