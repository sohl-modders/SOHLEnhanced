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

#include "CLight.h"
#include "CLightFading.h"

BEGIN_DATADESC(CLightFading)
	DEFINE_FIELD(lightMode, FIELD_FLOAT),
	DEFINE_FIELD(lightUpdateTime, FIELD_FLOAT),
END_DATADESC()

LINK_ENTITY_TO_CLASS(light_fading, CLightFading);

void CLightFading::Spawn()
{
	CLight::Spawn();

	if (pev->spawnflags & SF_LIGHT_START_OFF)
	{
		lightMode = Light_Off;
		lightIntensity[0] = 'a';
	}

	else
	{
		lightMode = Light_On;
		lightIntensity[0] = 'z';
	}

	lightIntensity[1] = '\0';

	LIGHT_STYLE(GetStyle(), lightIntensity);

	DontThink();
}

void CLightFading::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "lightFrequency"))
	{
		lightUpdateTime = 1.0f / (atof(pkvd->szValue));
		pkvd->fHandled = TRUE;
	}

	else
	{
		CLight::KeyValue(pkvd);
	}
}

void CLightFading::Use(CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value)
{
	lightMode = !lightMode;

	SetNextThink(0.001f);
}

void CLightFading::Think()
{
	if (lightMode == Light_On && lightIntensity[0] < 'z')
	{
		lightIntensity[0]++;
	}
	else if (lightMode == Light_Off && lightIntensity[0] > 'a')
	{
		lightIntensity[0]--;
	}

	LIGHT_STYLE(GetStyle(), lightIntensity);

	SetNextThink(lightUpdateTime);
}
