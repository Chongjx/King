#include "ItemWeapon.h"


CWeapon::CWeapon(void)
	: weaponRange(0)
{
}


CWeapon::~CWeapon(void)
{
}

void CWeapon::setWeaponType(WEAPON_TYPE type)
{
	this->weaponType = type;
}
CWeapon::WEAPON_TYPE CWeapon::getWeaponType(void)
{
	return this->weaponType;
}

void CWeapon::setWeaponRange(int range)
{
	this->weaponRange = range;
}
int CWeapon::getWeaponRange(void)
{
	return this->weaponRange;
}