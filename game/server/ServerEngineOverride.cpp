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
#include "extdll.h"
#include "util.h"

#include "CMap.h"

#include "ServerEngineOverride.h"

namespace engine
{
void InitOverrides()
{
	g_engfuncs.pfnPrecacheModel		= &engine::PrecacheModel;
	g_engfuncs.pfnSetModel			= &engine::SetModel;
}

int PrecacheModel( const char* pszModelName )
{
	auto pMap = CMap::GetInstance()->GetGlobalModelReplacement();

	const char* pszNewName = pMap ? pMap->LookupFile( pszModelName ) : pszModelName;

	return g_hlenginefuncs.pfnPrecacheModel( pszNewName );
}

void SetModel( edict_t* pEdict, const char* pszModelName )
{
	auto pMap = CMap::GetInstance()->GetGlobalModelReplacement();

	const char* pszNewName = pMap ? pMap->LookupFile( pszModelName ) : pszModelName;

	g_hlenginefuncs.pfnSetModel( pEdict, pszNewName );
}
}
