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
#ifndef GAME_SERVER_ENTITIES_CPUSHABLE_H
#define GAME_SERVER_ENTITIES_CPUSHABLE_H

#include "CBreakable.h"

// func_pushable (it's also func_breakable, so don't collide with those flags)
#define SF_PUSH_BREAKABLE		128

class CPushable : public CBreakable
{
public:
	DECLARE_CLASS( CPushable, CBreakable );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	Precache( void ) override;
	void	Touch( CBaseEntity *pOther ) override;
	void	Move( CBaseEntity *pMover, int push );
	void	KeyValue( KeyValueData *pkvd ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void	StopSound( void );
	//	virtual void	SetActivator( CBaseEntity *pActivator ) { m_pPusher = pActivator; }

	virtual int	ObjectCaps() const override { return ( CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION ) | FCAP_CONTINUOUS_USE; }

	inline float MaxSpeed( void ) { return m_maxSpeed; }

	// breakables use an overridden takedamage
	virtual void OnTakeDamage( const CTakeDamageInfo& info ) override;

	static const char* const m_soundNames[ 3 ];
	int		m_lastSound;	// no need to save/restore, just keeps the same sound from playing twice in a row
	float	m_maxSpeed;
	float	m_soundTime;
};

#endif //GAME_SERVER_ENTITIES_CPUSHABLE_H