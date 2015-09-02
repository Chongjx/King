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
	void removeItem(int indexToRemove);
	void swapItem(int item1Index, int item2Index);
	void clear(void);

	vector<CItem*> getVecOfItems(void);
};

