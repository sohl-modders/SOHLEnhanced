/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CRPG_H
#define GAME_SHARED_ENTITIES_WEAPONS_CRPG_H

class CLaserSpot;

enum rpg_e
{
	RPG_IDLE = 0,
	RPG_FIDGET,
	RPG_RELOAD,		// to reload
	RPG_FIRE2,		// to empty
	RPG_HOLSTER1,	// loaded
	RPG_DRAW1,		// loaded
	RPG_HOLSTER2,	// unloaded
	RPG_DRAW_UL,	// unloaded
	RPG_IDLE_UL,	// unloaded idle
	RPG_FIDGET_UL,	// unloaded fidget
};

class CRpg : public CBasePlayerWeapon
{
public:
	DECLARE_CLASS( CRpg, CBasePlayerWeapon );
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CRpg();

	void Spawn( void ) override;
	void Precache( void ) override;
	void Reload( void ) override;
	bool AddToPlayer( CBasePlayer *pPlayer ) override;

	bool Deploy() override;
	bool CanHolster() override;
	void Holster() override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	void WeaponIdle( void ) override;

	void UpdateSpot( void );
	bool ShouldWeaponIdle() override { return true; }

#ifdef CLIENT_DLL
	TEMPENTITY* m_pSpot;
#else
	CLaserSpot *m_pSpot;
#endif
	bool m_bSpotActive;
	int m_cActiveRockets;// how many missiles in flight from this launcher right now?

private:
	unsigned short m_usRpg;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CRPG_H