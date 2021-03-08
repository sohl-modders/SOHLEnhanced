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

/*********************
* Worldcraft entity: info_alias
*
* targetname- alias name
* target-     alias destination while ON
* netname-    alias destination while OFF
**********************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CInfoAlias.h"

LINK_ENTITY_TO_CLASS(info_alias, CInfoAlias);

void CInfoAlias::Spawn()
{
	if (GetSpawnFlags().Any(SF_ALIAS_OFF))
		SetMessage(GetNetName());
	else
		SetMessage(GetTarget());
}

void CInfoAlias::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (GetSpawnFlags().Any(SF_ALIAS_OFF))
	{
		if (GetSpawnFlags().Any(SF_ALIAS_DEBUG))
			ALERT(at_console, "DEBUG: info_alias %s turns on\n",
				GetTargetname());

		GetSpawnFlags() &= ~SF_ALIAS_OFF;
		pev->noise = pev->target;
	}
	else
	{
		if (GetSpawnFlags().Any(SF_ALIAS_DEBUG))
			ALERT(at_console, "DEBUG: info_alias %s turns off\n",
				GetTargetname());

		GetSpawnFlags() |= SF_ALIAS_OFF;
		pev->noise = pev->netname;
	}

	UTIL_AddToAliasList(this);
}

CBaseEntity* CInfoAlias::FollowAlias(CBaseEntity* pFrom)
{
	return UTIL_FindEntityByTargetname(pFrom, STRING(pev->message));
}

void CInfoAlias::ChangeValue(int iszValue)
{
	pev->noise = iszValue;
	UTIL_AddToAliasList(this);
}

void CInfoAlias::FlushChanges()
{
	pev->message = pev->noise;
	if (pev->spawnflags & SF_ALIAS_DEBUG)
		ALERT(at_console, "DEBUG: info_alias %s now refers to \"%s\"\n",
			GetTargetname(), GetMessage());
}