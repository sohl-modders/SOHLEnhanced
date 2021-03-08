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
#include "cbase.h"
#include "CBasePlayer.h"

#include "entities/lights/CLight.h"
#include "entities/lights/CLightFader.h"
#include "CTriggerLightstyle.h"

BEGIN_DATADESC(CTriggerLightstyle)
	DEFINE_FIELD(m_iszPattern, FIELD_STRING),
	DEFINE_FIELD(m_iFade, FIELD_INTEGER),
	DEFINE_FIELD(m_iWait, FIELD_INTEGER),
	DEFINE_USEFUNC(Use),
END_DATADESC()

LINK_ENTITY_TO_CLASS(trigger_lightstyle, CTriggerLightstyle);

void CTriggerLightstyle::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "pattern"))
	{
		m_iszPattern = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iFade"))
	{
		m_iFade = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iWait"))
	{
		m_iWait = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else
		BaseClass::KeyValue(pkvd);
}

void CTriggerLightstyle::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!HasTarget())
		return;

	//ALERT( at_console, "Lightstyle change for: (%s)\n", GetTarget() );

	CBaseEntity* pTarget = nullptr;
	while ((pTarget = UTIL_FindEntityByTargetname(pTarget,GetTarget())) != nullptr)
	{
		int iszPattern;
		if (m_iszPattern)
			iszPattern = m_iszPattern;
		else
			iszPattern = GetStdLightStyle(m_iStyle);

		// not a light entity?
		if (!pTarget->ClassnameIs("light") && 
			!pTarget->ClassnameIs("light_spot") && 
			!pTarget->ClassnameIs("light_environment"))
		{
			if (pTarget->m_iStyle >= 32)
				LIGHT_STYLE(pTarget->m_iStyle, (char*)STRING(iszPattern));
		}
		else
		{
			CLight* pLight = (CLight*)pTarget;

			if (m_iFade)
			{
				//				ALERT(at_console, "Making fader ent, step 1/%d = %f\n", m_iFade, 1/m_iFade);
				CLightFader* pFader = GetClassPtr((CLightFader*)nullptr);
				pFader->m_pLight = pLight;
				pFader->m_cFrom = ((char*)STRING(pLight->GetStyle()))[0];
				pFader->m_cTo = ((char*)STRING(iszPattern))[0];
				pFader->m_iszPattern = iszPattern;
				pFader->m_fEndTime = gpGlobals->time + m_iFade;
				pFader->m_fStep = ((float)1) / m_iFade;
				pFader->m_iWait = m_iWait;
				pFader->SetThink(&CLightFader::FadeThink);
				pFader->SetNextThink(0.1);
			}
			else
			{
				pLight->SetStyle(iszPattern);
				if (m_iWait != -1)
				{
					CLightFader* pFader = GetClassPtr((CLightFader*)nullptr);
					pFader->m_pLight = pLight;
					pFader->SetThink(&CLightFader::WaitThink);
					pFader->SetNextThink(m_iWait);
				}
			}
		}
	}
}
