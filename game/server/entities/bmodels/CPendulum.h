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
#ifndef GAME_SERVER_ENTITIES_BMODELS_CPENDULUM_H
#define GAME_SERVER_ENTITIES_BMODELS_CPENDULUM_H

#define		SF_PENDULUM_SWING		2	// spawnflag that makes a pendulum a rope swing.

class CPendulum : public CBaseEntity
{
public:
	DECLARE_CLASS( CPendulum, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;
	void	Swing( void );
	void	PendulumUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	Stop( void );
	void	Touch( CBaseEntity *pOther ) override;
	void	RopeTouch( CBaseEntity *pOther );// this touch func makes the pendulum a rope
	virtual int	ObjectCaps() const override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	void	Blocked( CBaseEntity *pOther ) override;

	float	m_accel;			// Acceleration
	float	m_distance;			// 
	float	m_time;
	float	m_damp;
	float	m_maxSpeed;
	float	m_dampSpeed;
	Vector	m_center;
	Vector	m_start;
};

#endif //GAME_SERVER_ENTITIES_BMODELS_CPENDULUM_H