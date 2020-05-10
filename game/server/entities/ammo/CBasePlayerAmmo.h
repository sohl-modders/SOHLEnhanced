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
#ifndef GAME_SERVER_ENTITIES_AMMO_CBASEPLAYERAMMO_H
#define GAME_SERVER_ENTITIES_AMMO_CBASEPLAYERAMMO_H

class CBasePlayerAmmo : public CBaseEntity
{
public:
	DECLARE_CLASS( CBasePlayerAmmo, CBaseEntity );
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	virtual void Spawn() override;
	void DefaultTouch( CBaseEntity* pOther ); // default weapon touch
	virtual bool AddAmmo( CBaseEntity* pOther ) { return true; }

	CBaseEntity* Respawn() override;
	void Materialize();
};

#define DEFAULT_AMMO_PICKUP_SOUND "items/9mmclip1.wav"

/**
*	Gives ammo to the given player.
*	@param pGiver Entity that is giving the ammo.
*	@param pPlayer Player to give ammo to. If this is not a player, no ammo is given.
*	@param iAmount Amount of ammo to give. Amount smaller than or equal to 0 are ignored.
*	@param pszAmmoName Name of the ammo type to give ammo of.
*	@param pszPickupSound Sound to play on pickup. Defaults to DEFAULT_AMMO_PICKUP_SOUND. If null, no sound is played.
*	@return true if ammo was given, false otherwise.
*/
bool UTIL_GiveAmmoToPlayer( CBaseEntity* pGiver, CBaseEntity* pPlayer,
							const int iAmount, const char* const pszAmmoName,
							const char* const pszPickupSound = DEFAULT_AMMO_PICKUP_SOUND );

#endif //GAME_SERVER_ENTITIES_AMMO_CBASEPLAYERAMMO_H