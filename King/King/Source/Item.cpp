#include "Item.h"


CItem::CItem(void)
	: itemType(ITEM_WEAPON)
	//, itemPos
	, itemID(0)
	, itemName("Name")
	, itemDescription("Description")
	, playerAcquired(false)
{
}


CItem::~CItem(void)
{
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

void CItem::setPlayerAcquired(bool acquired)
{
	this->playerAcquired = acquired;
}
bool CItem::getPlayerAcquired(void)
{
	return this->playerAcquired;
}