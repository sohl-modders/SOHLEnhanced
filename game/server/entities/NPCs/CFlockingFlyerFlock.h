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
#ifndef GAME_SERVER_ENTITIES_NPCS_CFLOCKINGFLYERFLOCK_H
#define GAME_SERVER_ENTITIES_NPCS_CFLOCKINGFLYERFLOCK_H

#define		AFLOCK_MAX_RECRUIT_RADIUS	1024
#define		AFLOCK_FLY_SPEED			125
#define		AFLOCK_TURN_RATE			75
#define		AFLOCK_ACCELERATE			10
#define		AFLOCK_CHECK_DIST			192
#define		AFLOCK_TOO_CLOSE			100
#define		AFLOCK_TOO_FAR				256

class CFlockingFlyerFlock : public CBaseMonster
{
public:
	DECLARE_CLASS( CFlockingFlyerFlock, CBaseMonster );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	void KeyValue( KeyValueData *pkvd ) override;
	void SpawnFlock( void );

	// Sounds are shared by the flock
	static  void PrecacheFlockSounds( void );

	int		m_cFlockSize;
	float	m_flFlockRadius;
};

#endif //GAME_SERVER_ENTITIES_NPCS_CFLOCKINGFLYERFLOCK_H