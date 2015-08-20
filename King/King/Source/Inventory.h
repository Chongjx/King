#pragma once
#include <vector>
#include "Item.h"

using std::vector;

class CInventory
{
private:
	vector<CItem*> myInventory;

public:
	CInventory(void);
	~CInventory(void);

	//Inventory functions
	bool addItem(CItem* itemToAdd);
	void removeItem(int itemID);
	void swapItem(int item1ID, int item2ID);

	void setSize(int size);
	int getSize(void);
};

