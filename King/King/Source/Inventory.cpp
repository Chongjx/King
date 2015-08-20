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
		myInventory.push_back(itemToAdd);
		return true;
	}
	else
	{
		return false;
	}
	return false;
}
void CInventory::removeItem(int itemID)
{
	for (std::vector<CItem *>::iterator it = myInventory.begin(); it != myInventory.end(); ++it)
	{
		CItem *item = (CItem *)*it;

		if (item->getItemID() == itemID)
		{
			delete item;
			break;
		}
	}
}

//Item1 and Item2 ID will be obtained from the button position of the inventory
void CInventory::swapItem(int item1ID, int item2ID)
{	
	CItem* tempItem = new CItem;

	for (std::vector<CItem *>::iterator it = myInventory.begin(); it != myInventory.end(); ++it)
	{
		CItem *item1 = (CItem *)*it;
		//if the item in the list is the 1st item to be swapped
		if (item1->getItemID() == item1ID)
		{
			//store item1 in a temporary storage
			tempItem = item1;
			//check through the vector for the 2nd item that is going to be swapped
			for (std::vector<CItem *>::iterator it = myInventory.begin(); it != myInventory.end(); ++it)
			{
				CItem *item2 = (CItem *)*it;
				if (item2->getItemID() == item2ID)
				{
					//Swapping of items in the vector
					item1 = item2;
					item2 = tempItem;
					delete tempItem;
					break;
				}
			}
		}	
	}
}

void CInventory::setSize(int size)
{
	this->myInventory.resize(size);
}
int CInventory::getSize(void)
{
	return this->myInventory.size();
}