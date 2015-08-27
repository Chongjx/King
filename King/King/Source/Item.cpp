#include "Item.h"


CItem::CItem(void)
	: itemType(ITEM_WEAPON)
	, itemPos(0, 0)
	, itemID(0)
	, itemName("NULL")
	, itemDescription("Description")
{
}


CItem::~CItem(void)
{
}

void CItem::Init(ITEM_STATUS status, ITEM_TYPE type, Vector2 position, int id, string name, string description, Mesh* mesh, int location)
{
	this->itemStatus = status;
	this->itemType = type;
	this->itemPos = position;
	this->itemID = id;
	this->itemName = name;
	this->itemDescription = description;
	this->mesh = mesh;
	this->locationID = location;
}
void CItem::Update()
{
	if	(itemStatus == CItem::ITEM_ONGROUND)
	{
		//if mouse click and inventory is not full
		//itemStatus = CItem::ITEM_ININVENTORY
	}
	else if (itemStatus == CItem::ITEM_ININVENTORY)
	{
		//if mouse click n drag out of inventory box UI
		//itemStatus = CItem::ITEM_ONGROUND
	}
	else if (itemStatus == CItem::ITEM_INACTIVE)
	{
		//delete item
	}
}

void CItem::setItemStatus(ITEM_STATUS status)
{
	this->itemStatus = status;
}
CItem::ITEM_STATUS CItem::getItemStatus(void)
{
	return this->itemStatus;
}

void CItem::setItemType(ITEM_TYPE type)
{
	this->itemType = type;
}
CItem::ITEM_TYPE CItem::getItemType(void)
{
	return this->itemType;
}

void CItem::setItemPos(Vector2 position)
{
	this->itemPos.x = position.x;
	this->itemPos.y = position.y;
}
Vector2 CItem::getItemPos(void)
{
	return this->itemPos;
}

void CItem::setItemID(int id)
{
	this->itemID = id;
}
int CItem::getItemID(void)
{
	return this->itemID;
}

void CItem::setItemName(string name)
{
	this->itemName = name;
}
string CItem::getItemName(void)
{
	return this->itemName;
}

void CItem::setItemDescription(string description)
{
	this->itemDescription = description;
}
string CItem::getItemDescription(void)
{
	return this->itemDescription;
}

void CItem::setLocationID(int location)
{
	this->locationID = location;
}
int CItem::getLocationID(void)
{
	return this->locationID;
}