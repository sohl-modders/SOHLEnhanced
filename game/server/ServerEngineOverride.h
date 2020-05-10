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
#ifndef GAME_SERVER_SERVERENGINEOVERRIDE_H
#define GAME_SERVER_SERVERENGINEOVERRIDE_H

/**
*	@file
*	Functions that override engine functions in g_engfuncs
*/

namespace engine
{
void InitOverrides();

/**
*	Implements model replacement for model precaching.
*	@see enginefuncs_t::pfnPrecacheModel
*/
int PrecacheModel( const char* pszModelName );

/**
*	Implements model replacement for model setting.
*	@see enginefuncs_t::pfnSetModel
*/
void SetModel( edict_t* pEdict, const char* pszModelName );
}

#endif //GAME_SERVER_SERVERENGINEOVERRIDE_H
