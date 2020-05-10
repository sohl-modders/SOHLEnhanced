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
#ifndef GAME_SERVER_CGLOBALSTATE_H
#define GAME_SERVER_CGLOBALSTATE_H

enum GLOBALESTATE
{
	GLOBAL_OFF		= 0,
	GLOBAL_ON		= 1,
	GLOBAL_DEAD		= 2
};

/**
*	Converts a GLOBALESTATE value to string.
*/
const char* GLOBALESTATEToString( const GLOBALESTATE state );

//TODO: use constants - Solokiller
struct globalentity_t
{
	DECLARE_CLASS_NOBASE( globalentity_t );
	DECLARE_DATADESC_FINAL();

	char			name[ 64 ];
	char			levelName[ cchMapNameMost ];
	GLOBALESTATE	state;
	globalentity_t	*pNext;
};

class CGlobalState
{
public:
	DECLARE_CLASS_NOBASE( CGlobalState );
	DECLARE_DATADESC_FINAL();

	CGlobalState();
	void			Reset( void );
	void			ClearStates( void );
	void			EntityAdd( string_t globalname, string_t mapName, GLOBALESTATE state );
	void			EntitySetState( string_t globalname, GLOBALESTATE state );
	void			EntityUpdate( string_t globalname, string_t mapname );
	const globalentity_t	*EntityFromTable( string_t globalname );
	GLOBALESTATE	EntityGetState( string_t globalname );
	int				EntityInTable( string_t globalname ) { return ( Find( globalname ) != NULL ) ? 1 : 0; }
	bool			Save( CSave &save );
	bool			Restore( CRestore &restore );

	//#ifdef _DEBUG
	void			DumpGlobals( void );
	//#endif

private:
	globalentity_t	*Find( string_t globalname );
	globalentity_t	*m_pList;
	int				m_listCount;
};

extern CGlobalState gGlobalState;

#endif //GAME_SERVER_CGLOBALSTATE_H