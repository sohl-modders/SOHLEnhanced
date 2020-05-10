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
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CHANDGRENADE_H
#define GAME_SHARED_ENTITIES_WEAPONS_CHANDGRENADE_H

enum handgrenade_e
{
	HANDGRENADE_IDLE = 0,
	HANDGRENADE_FIDGET,
	HANDGRENADE_PINPULL,
	HANDGRENADE_THROW1,	// toss
	HANDGRENADE_THROW2,	// medium
	HANDGRENADE_THROW3,	// hard
	HANDGRENADE_HOLSTER,
	HANDGRENADE_DRAW
};

class CHandGrenade : public CBasePlayerWeapon
{
public:
	DECLARE_CLASS( CHandGrenade, CBasePlayerWeapon );

	CHandGrenade();

	void Spawn( void ) override;
	void Precache( void ) override;

	void PrimaryAttack( void ) override;
	bool Deploy() override;
	bool CanHolster() override;
	void Holster() override;
	void WeaponIdle( void ) override;

	void GetWeaponData( weapon_data_t& data ) override
	{
		BaseClass::GetWeaponData( data );

		data.fuser2 = m_flStartThrow;
		data.fuser3 = m_flReleaseThrow;
	}

	void SetWeaponData( const weapon_data_t& data ) override
	{
		BaseClass::SetWeaponData( data );

		m_flStartThrow = data.fuser2;
		m_flReleaseThrow = data.fuser3;
	}

private:
	float m_flStartThrow;
	float m_flReleaseThrow;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CHANDGRENADE_H