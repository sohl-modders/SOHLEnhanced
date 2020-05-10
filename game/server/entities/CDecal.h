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
#ifndef GAME_SERVER_CDECAL_H
#define GAME_SERVER_CDECAL_H

#define SF_DECAL_NOTINDEATHMATCH		2048

class CDecal : public CBaseEntity
{
public:
	DECLARE_CLASS( CDecal, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;
	void	StaticDecal( void );
	void	TriggerDecal( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
};

#endif //GAME_SERVER_CDECAL_H