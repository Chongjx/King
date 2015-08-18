#pragma once
#include <iostream>
#include <string>
#include "GameObject2D.h"
#include "Vector2.h"

using std::string;

class CItem : public GameObject2D
{
public:
	enum ITEM_TYPE
	{
		ITEM_WEAPON = 0,
		ITEM_CLOTHNG,
		ITEM_MISC,
		NUM_ITEM,
	};

private:
	//type of item
	ITEM_TYPE itemType;
	//position of item
	Vector2 itemPos;
	//id of item
	int itemID;
	//name of item
	string itemName;
	//description of item
	string itemDescription;
	//does the player currently have the item
	bool playerAcquired;

public:
	CItem(void);
	~CItem(void);

	//set type of item
	void setItemType(ITEM_TYPE type);
	//get type of item
	CItem::ITEM_TYPE getItemType(void);

	//set position of item
	void setItemPos(Vector2 position);
	Vector2 getItemPos(void);

	//set id of item
	void setItemID(int id);
	//get id of item
	int getItemID(void);

	//set name of item
	void setItemName(string name);
	//get name of item
	string getItemName(void);

	//set description of item
	void setItemDescription(string description);
	//get description of item
	string getItemDescription(void);

	//set if player has the item
	void setPlayerAcquired(bool acquired);
	//check if player has the item
	bool getPlayerAcquired(void);
};

