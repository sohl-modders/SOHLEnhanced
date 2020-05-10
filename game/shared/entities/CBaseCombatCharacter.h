/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef GAME_SHARED_ENTITIES_CBASECOMBATCHARACTER_H
#define GAME_SHARED_ENTITIES_CBASECOMBATCHARACTER_H

enum TimeBasedDamage
{
	itbd_Paralyze		= 0,
	itbd_NerveGas		= 1,
	itbd_Poison			= 2,
	itbd_Radiation		= 3,
	itbd_DrownRecover	= 4,
	itbd_Acid			= 5,
	itbd_SlowBurn		= 6,
	itbd_SlowFreeze		= 7,
	CDMG_TIMEBASED		= 8,
};

/**
*	Generic character.
*/
class CBaseCombatCharacter : public CBaseToggle
{
public:
	DECLARE_CLASS( CBaseCombatCharacter, CBaseToggle );
	DECLARE_DATADESC();

protected:
	/**
	*	What types of damage has monster (player) taken.
	*/
	int					m_bitsDamageType;

	/**
	*	Time based damage counters, decr. 1 per 2 seconds.
	*/
	byte				m_rgbTimeBasedDamage[ CDMG_TIMEBASED ];

	/**
	*	How much damage did monster (player) last take.
	*/
	int					m_lastDamageAmount;

public:

	virtual float GiveHealth( float flHealth, int bitsDamageType ) override;

	virtual	Vector GetGunPosition() = 0;

	virtual bool ShouldFadeOnDeath() const;

	virtual void BarnacleVictimBitten( CBaseEntity* pBarnacle ) = 0;
	virtual void BarnacleVictimReleased() = 0;
};

#endif //GAME_SHARED_ENTITIES_CBASECOMBATCHARACTER_H
