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

#include "CLightDynamic.h"

LINK_ENTITY_TO_CLASS(light_glow, CLightDynamic);

void CLightDynamic::Spawn()
{
	Precache();

	SetModel("sprites/null.spr");
	SetSolidType(SOLID_NOT);
	SetMoveType(MOVETYPE_NONE);
	GetSpawnFlags().AddFlags(SF_LIGHTDYNAMIC_START_OFF);

	m_iState = STATE_OFF;
	if (!GetSpawnFlags().Any(SF_LIGHTDYNAMIC_START_OFF))
	{
		m_iState = STATE_ON;
		SetEffects();
	}
}

void CLightDynamic::Precache()
{
	PrecacheModel("sprites/null.spr");
}

void CLightDynamic::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (!ShouldToggle(useType, GetState()))
	{
		if (GetState() >= STATE_ON)
			m_iState = STATE_OFF;
		else
			m_iState = STATE_ON;
		
		SetEffects();
	}
}

void CLightDynamic::SetEffects()
{
	if (GetState() == STATE_ON)
	{
		if (GetFrags() == 2)
			GetEffects() |= EF_BRIGHTLIGHT;
		else if (GetFrags())
			GetEffects() |= EF_DIMLIGHT;

		if (GetSpawnFlags().Any(SF_LIGHTDYNAMIC_FLARE))
			GetEffects() |= EF_LIGHT;
	}
	else
	{
		GetEffects() &= ~(EF_DIMLIGHT | EF_BRIGHTLIGHT | EF_LIGHT);
	}
}
