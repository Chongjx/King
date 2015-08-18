#pragma once
#include "Item.h"

class CWeapon : public CItem
{
public:
	enum WEAPON_TYPE
	{
		WEAPON_DUMBBELL,
		WEAPON_BATON,
		WEAPON_TASER,
		NUM_WEAPON,
	};

private:
	//type of weapon
	WEAPON_TYPE weaponType;
	//range of weapon, in tiles
	int weaponRange;
public:
	CWeapon(void);
	~CWeapon(void);

	//set type of weapon
	void setWeaponType(WEAPON_TYPE type);
	//get type of weapon
	WEAPON_TYPE getWeaponType(void);

	//set range of weapon e.g 1 tile, 2 tiles, etc
	void setWeaponRange(int range);
	//get range of weapon to check collision
	int getWeaponRange(void);
};

