#include "ItemClothing.h"


CClothing::CClothing(void)
	: clothingType(CLOTHING_PRISONER)
{
}


CClothing::~CClothing(void)
{
}

void CClothing::setClothingType(CLOTHING_TYPE type)
{
	this->clothingType = type;
}

CClothing::CLOTHING_TYPE CClothing::getClothingType(void)
{
	return this->clothingType;
}
