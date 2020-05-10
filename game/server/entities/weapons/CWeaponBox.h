#ifndef GAME_SERVER_ENTITIES_WEAPONS_CWEAPONBOX_H
#define GAME_SERVER_ENTITIES_WEAPONS_CWEAPONBOX_H

//=========================================================
// CWeaponBox - a single entity that can store weapons
// and ammo. 
//=========================================================
class CWeaponBox : public CBaseEntity
{
public:
	DECLARE_CLASS( CWeaponBox, CBaseEntity );
	DECLARE_DATADESC();

	void Precache() override;
	void Spawn() override;
	void Touch( CBaseEntity *pOther ) override;
	void KeyValue( KeyValueData *pkvd ) override;

private:
	bool IsEmpty() const;
	int  GiveAmmo( int iCount, char *szName, int iMax, int *pIndex = NULL );
	void SetObjectCollisionBox() override;

public:
	void Kill();

	bool HasWeapon( CBasePlayerWeapon *pCheckItem ) const;
	bool PackWeapon( CBasePlayerWeapon *pWeapon );
	bool PackAmmo( int iszName, int iCount );

	CBasePlayerWeapon* m_rgpPlayerItems[ MAX_WEAPON_SLOTS ];// one slot for each 

	int m_rgiszAmmo[ CAmmoTypes::MAX_AMMO_TYPES ];// ammo names
	int	m_rgAmmo[ CAmmoTypes::MAX_AMMO_TYPES ];// ammo quantities

	int m_cAmmoTypes;// how many ammo types packed into this box (if packed by a level designer)
};

#endif //GAME_SERVER_ENTITIES_WEAPONS_CWEAPONBOX_H