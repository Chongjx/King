#pragma once
#include "Item.h"

class CMisc : public CItem
{
public:
	enum MISC_TYPE
	{
		//For accessing places
		MISC_FORK,
		MISC_CELLKEY,
		MISC_ACCESSCARD,
		//For player's field of view at night
		MISC_MATCHES,
		MISC_TORCHLIGHT,
		//For player's map
		MISC_NOTES,
		NUM_MISC,
	};
private:
	MISC_TYPE miscType;
public:
	CMisc(void);
	~CMisc(void);

	void setMiscType(MISC_TYPE type);
	CMisc::MISC_TYPE getMiscType(void);
};

