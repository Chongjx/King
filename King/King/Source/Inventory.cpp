#include "Inventory.h"


CInventory::CInventory(void)
{
}


CInventory::~CInventory(void)
{
}

bool CInventory::addItem(CItem* itemToAdd)
{
	if (myInventory.size() < myInventory.max_size())
	{
		itemToAdd->setItemStatus(CItem::ITEM_ININVENTORY);
		myInventory.push_back(itemToAdd);
		return true;
	}
	else
	{
		return false;
	}
	return false;
}
void CInventory::removeItem(int indexToRemove)
{
	myInventory.erase(myInventory.begin() + indexToRemove);	
}

//Item1 and Item2 Index will be obtained from the button position of the inventory
void CInventory::swapItem(int item1Index, int item2Index)
{	
	std::iter_swap(myInventory.at(item1Index), myInventory.at(item2Index));
}

vector<CItem*> CInventory::getVecOfItems(void)
{
	return this->myInventory;
}