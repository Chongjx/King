#include "ItemMisc.h"


CMisc::CMisc(void)
{
}


CMisc::~CMisc(void)
{
}

void CMisc::setMiscType(MISC_TYPE type)
{
	this->miscType = type;
}
CMisc::MISC_TYPE CMisc::getMiscType(void)
{
	return this->miscType;
}