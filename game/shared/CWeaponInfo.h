#ifndef GAME_SHARED_CWEAPONINFO_H
#define GAME_SHARED_CWEAPONINFO_H

#include "entities/weapons/CAmmoTypes.h"

class CWeaponHUDInfo;

/**
*	Stores a single weapon's information.
*	Avoid changing this information after the info has been loaded, since it's used on both the client and server side.
*/
class CWeaponInfo final
{
public:
	CWeaponInfo() = default;
	~CWeaponInfo();

	/**
	*	Handles keyvalues loaded in from files.
	*	@param pszKey Key.
	*	@param pszValue Value.
	*	@return true if the key was handled, false otherwise.
	*/
	bool KeyValue( const char* const pszKey, const char* const pszValue );

	/**
	*	Handles flags loaded in from files.
	*	@param pszFlag Flag to set.
	*	@return true if the flag was set, false otherwise.
	*/
	bool Flag( const char* const pszFlag );

	/**
	*	@return The entity's class name.
	*/
	const char* GetWeaponName() const { return m_pszWeaponName; }

	/**
	*	Sets the weapon name. Avoid changing weapon names.
	*/
	void SetWeaponName( const char* const pszWeaponName );

	/**
	*	@return The weapon ID.
	*	@see WeaponId
	*/
	int GetID() const { return m_iId; }

	/**
	*	Sets the weapon ID. Avoid changing weapon IDs.
	*/
	void SetID( const int iID )
	{
		m_iId = iID;
	}

	/**
	*	@return The bucket to place the weapon in.
	*	@see MAX_WEAPON_SLOTS
	*/
	int GetBucket() const { return m_iBucket; }

	/**
	*	Sets the bucket.
	*/
	void SetBucket( const int iBucket )
	{
		m_iBucket = iBucket;
	}

	/**
	*	@return The position in the bucket to place the weapon in.
	*	@see MAX_WEAPONS
	*/
	int GetPosition() const { return m_iPosition; }

	/**
	*	Sets the position.
	*/
	void SetPosition( const int iPosition )
	{
		m_iPosition = iPosition;
	}

	/**
	*	@return The primary ammo type.
	*/
	const CAmmoType* GetPrimaryAmmo() const { return m_pPrimaryAmmo; }

	/**
	*	@return The primary ammo type name.
	*/
	const char* GetPrimaryAmmoName() const { return m_pPrimaryAmmo ? m_pPrimaryAmmo->GetName() : nullptr; }

	/**
	*	@return The secondary ammo type.
	*/
	const CAmmoType* GetSecondaryAmmo() const { return m_pSecondaryAmmo; }

	/**
	*	@return The secondary ammo type name.
	*/
	const char* GetSecondaryAmmoName() const { return m_pSecondaryAmmo ? m_pSecondaryAmmo->GetName() : nullptr; }

	/**
	*	@return The maximum number of bullets in the primary ammo magazine.
	*/
	int GetMaxMagazine() const { return m_iMaxMagazine; }

	/**
	*	@return The default amount of ammo to store in the weapon.
	*/
	int GetDefaultAmmo() const { return m_iDefaultAmmo; }

	/**
	*	This value used to determine this weapon's importance in autoselection.
	*	@return The weapon's weight.
	*/
	int GetWeight() const { return m_iWeight; }

	/**
	*	@return Item flags.
	*	@see ItemInfoFlag
	*/
	int GetFlags() const { return m_iFlags; }

	/**
	*	@return The HUD info instance, if any.
	*/
	const CWeaponHUDInfo* GetHUDInfo() const { return m_pHUDInfo; }

	/**
	*	Sets the HUD info instance.
	*/
	void SetHUDInfo( const CWeaponHUDInfo* pHUDInfo )
	{
		m_pHUDInfo = pHUDInfo;
	}

private:
	const char* m_pszWeaponName = "";

	int m_iId = 0;

	int m_iBucket = 0;
	int m_iPosition = 0;

	CAmmoType* m_pPrimaryAmmo = nullptr;
	CAmmoType* m_pSecondaryAmmo = nullptr;

	int m_iMaxMagazine = WEAPON_NOCLIP;

	int m_iDefaultAmmo = 0;

	int m_iWeight = 0;

	int m_iFlags = 0;

	const CWeaponHUDInfo* m_pHUDInfo = nullptr;

private:
	CWeaponInfo( const CWeaponInfo& ) = delete;
	CWeaponInfo& operator=( const CWeaponInfo& ) = delete;
};

#endif //GAME_SHARED_CWEAPONINFO_H