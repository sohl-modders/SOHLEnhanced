#ifndef GAME_SHARED_CWEAPONHUDINFO_H
#define GAME_SHARED_CWEAPONHUDINFO_H

#include "wrect.h"

struct WeaponHUDSprite
{
	HSPRITE hSprite;
	wrect_t rect;
};

class CWeaponHUDInfo final
{
public:
	CWeaponHUDInfo();
	CWeaponHUDInfo( const CWeaponHUDInfo& other ) = default;
	CWeaponHUDInfo& operator=( const CWeaponHUDInfo& other ) = default;

	const WeaponHUDSprite& GetActive() const { return m_Active; }
	const WeaponHUDSprite& GetInactive() const { return m_Inactive; }
	const WeaponHUDSprite& GetPrimaryAmmo() const { return m_PrimaryAmmo; }
	const WeaponHUDSprite& GetSecondaryAmmo() const { return m_SecondaryAmmo; }
	const WeaponHUDSprite& GetCrosshair() const { return m_Crosshair; }
	const WeaponHUDSprite& GetAutoAim() const { return m_AutoAim; }
	const WeaponHUDSprite& GetZoomedCrosshair() const { return m_ZoomedCrosshair; }
	const WeaponHUDSprite& GetZoomedAutoAim() const { return m_ZoomedAutoAim; }

	bool LoadFromFile( const char* const pszWeaponName );

private:
	WeaponHUDSprite m_Active;
	WeaponHUDSprite m_Inactive;
	WeaponHUDSprite m_PrimaryAmmo;
	WeaponHUDSprite m_SecondaryAmmo;
	WeaponHUDSprite m_Crosshair;
	WeaponHUDSprite m_AutoAim;
	WeaponHUDSprite m_ZoomedCrosshair;
	WeaponHUDSprite m_ZoomedAutoAim;
};

#endif //GAME_SHARED_CWEAPONHUDINFO_H