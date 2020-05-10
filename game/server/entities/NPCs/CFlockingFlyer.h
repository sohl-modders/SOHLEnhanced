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
#ifndef GAME_SERVER_ENTITIES_NPCS_CFLOCKINGFLYER_H
#define GAME_SERVER_ENTITIES_NPCS_CFLOCKINGFLYER_H

//TODO: this entity has copy pasted squad monster code in it. Refactor - Solokiller
class CFlockingFlyer : public CBaseMonster
{
public:
	DECLARE_CLASS( CFlockingFlyer, CBaseMonster );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	void SpawnCommonCode( void );
	void IdleThink( void );
	void BoidAdvanceFrame( void );
	void FormFlock( void );
	void Start( void );
	void FlockLeaderThink( void );
	void FlockFollowerThink( void );
	void FallHack( void );
	void MakeSound( void );
	void AlertFlock( void );
	void SpreadFlock( void );
	void SpreadFlock2( void );
	void Killed( const CTakeDamageInfo& info, GibAction gibAction ) override;
	void Poop( void );
	bool FPathBlocked();
	//void KeyValue( KeyValueData *pkvd );

	bool IsLeader() const { return m_pSquadLeader == this; }
	bool InSquad() const { return m_pSquadLeader != nullptr; }
	int	SquadCount( void );
	void SquadRemove( CFlockingFlyer *pRemove );
	void SquadUnlink( void );
	void SquadAdd( CFlockingFlyer *pAdd );
	void SquadDisband( void );

	CFlockingFlyer *m_pSquadLeader;
	CFlockingFlyer *m_pSquadNext;
	bool	m_fTurning;// is this boid turning?
	bool	m_fCourseAdjust;// followers set this flag true to override flocking while they avoid something
	bool	m_fPathBlocked;// true if there is an obstacle ahead
	Vector	m_vecReferencePoint;// last place we saw leader
	Vector	m_vecAdjustedVelocity;// adjusted velocity (used when fCourseAdjust is true)
	float	m_flGoalSpeed;
	float	m_flLastBlockedTime;
	float	m_flFakeBlockedTime;
	float	m_flAlertTime;
	float	m_flFlockNextSoundTime;
};

#endif //GAME_SERVER_ENTITIES_NPCS_CFLOCKINGFLYER_H