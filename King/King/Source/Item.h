#pragma once
#include <iostream>
#include <string>
#include "GameObject2D.h"
#include "Vector2.h"

using std::string;

class CItem : public GameObject2D
{
public:
	//type of item
	enum ITEM_TYPE
	{
		ITEM_WEAPON = 0,
		ITEM_CLOTHNG,
		ITEM_MISC,
		NUM_ITEM,
	};

	enum ITEM_STATUS
	{
		//Active status
		ITEM_ONGROUND = 0,
		ITEM_ININVENTORY,
		//Inactive status
		ITEM_INACTIVE,
		NUM_STATUS,
	};
private:
	//status of item
	ITEM_STATUS itemStatus;
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
	//Location ID of item
	int locationID;

public:
	CItem(void);
	~CItem(void);

	//Initialise item
	void Init(ITEM_STATUS status, ITEM_TYPE type, Vector2 position, int id, string name, string description, Mesh* mesh, int location);
	//Update item
	void Update();

	//set item status
	void setItemStatus(ITEM_STATUS status);
	//get status of item
	CItem::ITEM_STATUS getItemStatus(void);

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
	
	//set location of item
	void setLocationID(int location);
	//get location of item
	int getLocationID(void);
};

