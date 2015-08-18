#pragma once
#include "Item.h"

class CClothing : public CItem
{
public:
	enum CLOTHING_TYPE
	{
		CLOTHING_PRISONER = 0,
		CLOTHING_GUARD,
		NUM_CLOTHING,
	};

private:
	//type of clothes
	CLOTHING_TYPE clothingType;

public:
	CClothing(void);
	~CClothing(void);

	//set type of clothes
	void setClothingType(CLOTHING_TYPE type);
	//get the type of clothes player is using
	CClothing::CLOTHING_TYPE getClothingType(void);
};

