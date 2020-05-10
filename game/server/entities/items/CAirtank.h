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
#ifndef GAME_SERVER_ENTITIES_ITEMS_CAIRTANK_H
#define GAME_SERVER_ENTITIES_ITEMS_CAIRTANK_H

class CAirtank : public CGrenade
{
public:
	/**
	*	The default amount of time, in seconds, that players will be given to breathe when they pick up this tank.
	*/
	static const float DEFAULT_AIR_TIME;

	/**
	*	The default base recharge time. The actual time is base + air time, to keep it from recharging before the player has run out.
	*/
	static const float DEFAULT_BASE_RECHARGE_TIME;

public:
	DECLARE_CLASS( CAirtank, CGrenade );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData* pkvd ) override;
	void Spawn( void ) override;
	void Precache( void ) override;
	void TankThink( void );
	void TankTouch( CBaseEntity *pOther );
	int	 BloodColor() const override { return DONT_BLEED; }
	void Killed( const CTakeDamageInfo& info, GibAction gibAction ) override;

private:
	bool m_bState;
	bool m_bDisableAccumRecharge = false;

	float m_flAirTimeToGive = DEFAULT_AIR_TIME;
	float m_flBaseRechargeTime = DEFAULT_BASE_RECHARGE_TIME;
};

#endif //GAME_SERVER_ENTITIES_ITEMS_CAIRTANK_H