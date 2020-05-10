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
#ifndef GAME_SERVER_CBASEDELAY_H
#define GAME_SERVER_CBASEDELAY_H

//
// generic Delay entity.
//
class CBaseDelay : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseDelay, CBaseEntity );
	DECLARE_DATADESC();

	float		m_flDelay;
	string_t	m_iszKillTarget;

	EHANDLE m_hOriginalCaller;

	virtual void	KeyValue( KeyValueData* pkvd ) override;
	// common member functions
	void SUB_UseTargets( CBaseEntity *pActivator, USE_TYPE useType, float value );
	void DelayThink( void );
};

#endif //GAME_SERVER_CBASEDELAY_H