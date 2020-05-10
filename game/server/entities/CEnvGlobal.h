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
#ifndef GAME_SERVER_ENTITIES_CENVGLOBAL_H
#define GAME_SERVER_ENTITIES_CENVGLOBAL_H

#define SF_GLOBAL_SET			1	// Set global state to initial state on spawn

class CEnvGlobal : public CPointEntity
{
public:
	enum class TriggerMode
	{
		OFF		= 0,
		ON		= 1,
		DEAD	= 2,
		TOGGLE	= 3
	};

public:
	DECLARE_CLASS( CEnvGlobal, CPointEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	string_t	m_globalstate;
	TriggerMode m_triggermode;
	int			m_initialstate;
};

#endif //GAME_SERVER_ENTITIES_CENVGLOBAL_H