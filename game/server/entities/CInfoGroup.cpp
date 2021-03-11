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
* Worldcraft entity: info_group
*
* targetname- name
* target-     alias entity to affect
* other values are handled in a multi_manager-like way.
**********************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

BEGIN_DATADESC(CInfoGroup)
	DEFINE_FIELD(m_cMembers, FIELD_INTEGER),
	DEFINE_ARRAY(m_iszMemberName, FIELD_STRING, MAX_MULTI_TARGETS_IT),
	DEFINE_ARRAY(m_iszMemberValue, FIELD_STRING, MAX_MULTI_TARGETS_IT),
	DEFINE_FIELD(m_iszDefaultMember, FIELD_STRING),
END_DATADESC()

LINK_ENTITY_TO_CLASS(info_group, CInfoGroup);

void CInfoGroup::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "defaultmember"))
	{
		m_iszDefaultMember = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	// this assumes that additional fields are targetnames and their values are delay values.
	else if (m_cMembers < MAX_MULTI_TARGETS_IT)
	{
		char tmp[128];
		UTIL_StripToken(pkvd->szKeyName, tmp);
		m_iszMemberName[m_cMembers] = ALLOC_STRING(tmp);
		m_iszMemberValue[m_cMembers] = ALLOC_STRING(pkvd->szValue);
		m_cMembers++;
		pkvd->fHandled = true;
	}
	else
	{
		ALERT(at_error, "Too many members for info_group %s (limit is %d)\n",
			GetTargetname(), MAX_MULTI_TARGETS_IT);
	}
}

void CInfoGroup::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	CBaseEntity* pTarget = UTIL_FindEntityByTargetname(nullptr, GetTarget());

	if (pTarget && pTarget->IsAlias())
	{
		if (GetSpawnFlags().Any(SF_GROUP_DEBUG))
			ALERT(at_console, "DEBUG: info_group %s changes the contents of %s \"%s\"\n",
				GetTargetname(), GetClassname(), GetTargetname());
		((CBaseAlias*)pTarget)->ChangeValue(this);
	}
	else if (pev->target)
	{
		ALERT(at_console, "info_group \"%s\": alias \"%s\" was not found or not an alias!",
			GetTargetname(), GetTarget());
	}
}

int CInfoGroup::GetMember(const char* szMemberName)
{
	if (!szMemberName)
	{
		ALERT(at_console, "info_group: GetMember called with null szMemberName!?\n");
		return NULL;
	}

	for (int i = 0; i < m_cMembers; i++)
	{
		if (FStrEq(szMemberName, STRING(m_iszMemberName[i])))
		{
			//ALERT(at_console,"getMember: found member\n");
			return m_iszMemberValue[i];
		}
	}

	if (m_iszDefaultMember)
	{
		static char szBuffer[128];
		strcpy(szBuffer, STRING(m_iszDefaultMember));
		strcat(szBuffer, szMemberName);
		return MAKE_STRING(szBuffer);
		// this is a messy way to do it... but currently, only one
		// GetMember gets performed at a time, so it works.
	}

	ALERT(at_console, "info_group \"%s\" has no member called \"%s\".\n",
		GetTarget(), szMemberName);
	//	ALERT(at_console,"getMember: fail\n");
	return NULL;
}