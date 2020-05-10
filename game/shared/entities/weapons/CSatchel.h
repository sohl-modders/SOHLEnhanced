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
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CSATCHEL_H
#define GAME_SHARED_ENTITIES_WEAPONS_CSATCHEL_H

enum satchel_e
{
	SATCHEL_IDLE1 = 0,
	SATCHEL_FIDGET1,
	SATCHEL_DRAW,
	SATCHEL_DROP
};

enum satchel_radio_e
{
	SATCHEL_RADIO_IDLE1 = 0,
	SATCHEL_RADIO_FIDGET1,
	SATCHEL_RADIO_DRAW,
	SATCHEL_RADIO_FIRE,
	SATCHEL_RADIO_HOLSTER
};


class CSatchel : public CBasePlayerWeapon
{
public:
	enum class ChargeState
	{
		/**
		*	No satchels deployed.
		*/
		NONE = 0,

		/**
		*	At least one satchel deployed.
		*/
		DEPLOYED,

		/**
		*	Triggered satchels, reloading.
		*/
		TRIGGERED
	};

public:
	DECLARE_CLASS( CSatchel, CBasePlayerWeapon );
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	CSatchel();

	void Spawn( void ) override;
	void Precache( void ) override;
	bool AddToPlayer( CBasePlayer *pPlayer ) override;
	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	bool AddDuplicate( CBasePlayerWeapon *pOriginal ) override;
	bool CanDeploy() const override;
	bool Deploy() override;
	bool IsUseable() override;

	void Holster() override;
	void WeaponIdle( void ) override;
	void Throw( void );

	void GetWeaponData( weapon_data_t& data ) override
	{
		BaseClass::GetWeaponData( data );

		data.iuser1 = static_cast<int>( m_chargeReady );
	}

	void SetWeaponData( const weapon_data_t& data ) override
	{
		BaseClass::SetWeaponData( data );

		m_chargeReady = static_cast<ChargeState>( data.iuser1 );
	}

private:
	ChargeState m_chargeReady = ChargeState::NONE;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CSATCHEL_H