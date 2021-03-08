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
* Worldcraft entity: multi_alias
*
* targetname- name
* other values are handled in a multi_manager-like way.
**********************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerChangeAlias.h"

LINK_ENTITY_TO_CLASS(trigger_changealias, CTriggerChangeAlias);

void CTriggerChangeAlias::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	CBaseEntity* pTarget = UTIL_FindEntityByTargetname(nullptr, GetTarget());

	if (pTarget && pTarget->IsAlias())
	{
		CBaseEntity* pValue;

		if (FStrEq(GetNetName(), "*locus"))
		{
			pValue = pActivator;
		}
		else if (GetSpawnFlags().Any(SF_CHANGEALIAS_RESOLVE))
		{
			pValue = UTIL_FollowReference(nullptr, GetNetName());
		}

		if (pValue)
			((CBaseAlias*)pTarget)->ChangeValue(pValue);
		else
			((CBaseAlias*)pTarget)->ChangeValue(pev->netname);
	}
	else
	{
		ALERT(at_error, "trigger_changealias %s: alias \"%s\" was not found or not an alias!", 
			GetTargetname(), GetTarget());
	}
}