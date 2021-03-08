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


//LRC
int GetStdLightStyle(int iStyle)
{
	switch (iStyle)
	{
		// 0 normal
	case 0: return MAKE_STRING("m");

		// 1 FLICKER (first variety)
	case 1: return MAKE_STRING("mmnmmommommnonmmonqnmmo");

		// 2 SLOW STRONG PULSE
	case 2: return MAKE_STRING("abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");

		// 3 CANDLE (first variety)
	case 3: return MAKE_STRING("mmmmmaaaaammmmmaaaaaabcdefgabcdefg");

		// 4 FAST STROBE
	case 4: return MAKE_STRING("mamamamamama");

		// 5 GENTLE PULSE 1
	case 5: return MAKE_STRING("jklmnopqrstuvwxyzyxwvutsrqponmlkj");

		// 6 FLICKER (second variety)
	case 6: return MAKE_STRING("nmonqnmomnmomomno");

		// 7 CANDLE (second variety)
	case 7: return MAKE_STRING("mmmaaaabcdefgmmmmaaaammmaamm");

		// 8 CANDLE (third variety)
	case 8: return MAKE_STRING("mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");

		// 9 SLOW STROBE (fourth variety)
	case 9: return MAKE_STRING("aaaaaaaazzzzzzzz");

		// 10 FLUORESCENT FLICKER
	case 10: return MAKE_STRING("mmamammmmammamamaaamammma");

		// 11 SLOW PULSE NOT FADE TO BLACK
	case 11: return MAKE_STRING("abcdefghijklmnopqrrqponmlkjihgfedcba");

		// 12 UNDERWATER LIGHT MUTATION
		// this light only distorts the lightmap - no contribution
		// is made to the brightness of affected surfaces
	case 12: return MAKE_STRING("mmnnmmnnnmmnn");

		// 13 OFF (LRC)
	case 13: return MAKE_STRING("a");

		// 14 SLOW FADE IN (LRC)
	case 14: return MAKE_STRING("aabbccddeeffgghhiijjkkllmmmmmmmmmmmmmm");

		// 15 MED FADE IN (LRC)
	case 15: return MAKE_STRING("abcdefghijklmmmmmmmmmmmmmmmmmmmmmmmmmm");

		// 16 FAST FADE IN (LRC)
	case 16: return MAKE_STRING("acegikmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");

		// 17 SLOW FADE OUT (LRC)
	case 17: return MAKE_STRING("llkkjjiihhggffeeddccbbaaaaaaaaaaaaaaaa");

		// 18 MED FADE OUT (LRC)
	case 18: return MAKE_STRING("lkjihgfedcbaaaaaaaaaaaaaaaaaaaaaaaaaaa");

		// 19 FAST FADE OUT (LRC)
	case 19: return MAKE_STRING("kigecaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	default: return MAKE_STRING("m");
	}
}

BEGIN_DATADESC(CLight)
	DEFINE_FIELD(m_iState, FIELD_INTEGER),
	DEFINE_FIELD(m_iszPattern, FIELD_STRING),
	DEFINE_FIELD(m_iszCurrentStyle, FIELD_STRING),
	DEFINE_FIELD(m_iOnStyle, FIELD_INTEGER),
	DEFINE_FIELD(m_iOffStyle, FIELD_INTEGER),
	DEFINE_FIELD(m_iTurnOnStyle, FIELD_INTEGER),
	DEFINE_FIELD(m_iTurnOffStyle, FIELD_INTEGER),
	DEFINE_FIELD(m_iTurnOnTime, FIELD_INTEGER),
	DEFINE_FIELD(m_iTurnOffTime, FIELD_INTEGER),
END_DATADESC()

LINK_ENTITY_TO_CLASS(light, CLight);

//
// shut up spawn functions for new spotlights
//
LINK_ENTITY_TO_CLASS(light_spot, CLight);

//
// Cache user-entity-field values until spawn is called.
//
void CLight::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "m_iOnStyle"))
	{
		m_iOnStyle = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iOffStyle"))
	{
		m_iOffStyle = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOnStyle"))
	{
		m_iTurnOnStyle = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOffStyle"))
	{
		m_iTurnOffStyle = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOnTime"))
	{
		m_iTurnOnTime = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "m_iTurnOffTime"))
	{
		m_iTurnOffTime = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "pitch"))
	{
		Vector vecAngles = GetAbsAngles();
		vecAngles.x = atof(pkvd->szValue);
		SetAbsAngles(vecAngles);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "pattern"))
	{
		m_iszPattern = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "firetarget"))
	{
		pev->target = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else
	{
		CPointEntity::KeyValue(pkvd);
	}
}

void CLight::SetStyle(int iszPattern)
{
	if (m_iStyle < 32) // if it's using a global style, don't change it
		return;

	m_iszCurrentStyle = iszPattern;

	//	ALERT(at_console, "SetStyle %d \"%s\"\n", m_iStyle, (char *)STRING( iszPattern ));
	LIGHT_STYLE(m_iStyle, (char*)STRING(iszPattern));
}

void CLight::SetCorrectStyle()
{
	if (m_iStyle >= 32)
	{
		switch (m_iState)
		{
		case STATE_ON:
			if (m_iszPattern) // custom styles have priority over standard ones
				SetStyle(m_iszPattern);
			else if (m_iOnStyle)
				SetStyle(GetStdLightStyle(m_iOnStyle));
			else
				SetStyle(MAKE_STRING("m"));
			break;
		case STATE_OFF:
			if (m_iOffStyle)
				SetStyle(GetStdLightStyle(m_iOffStyle));
			else
				SetStyle(MAKE_STRING("a"));
			break;
		case STATE_TURN_ON:
			if (m_iTurnOnStyle)
				SetStyle(GetStdLightStyle(m_iTurnOnStyle));
			else
				SetStyle(MAKE_STRING("a"));
			break;
		case STATE_TURN_OFF:
			if (m_iTurnOffStyle)
				SetStyle(GetStdLightStyle(m_iTurnOffStyle));
			else
				SetStyle(MAKE_STRING("m"));
			break;
		}
	}
	else
	{
		m_iszCurrentStyle = GetStdLightStyle(m_iStyle);
	}
}

void CLight::Think()
{
	switch (GetState())
	{
		case STATE_TURN_ON:
			m_iState = STATE_ON;
			FireTargets(STRING(pev->target), this, this, USE_ON, 0);
		break;
		case STATE_TURN_OFF:
			m_iState = STATE_OFF;
			FireTargets(STRING(pev->target), this, this, USE_OFF, 0);
		break;
	}
	
	SetCorrectStyle();
}

void CLight::Spawn()
{
	if (!HasTargetname())
	{
		// inert light
		UTIL_RemoveNow(this);
		return;
	}
	
	LIGHT_STYLE(m_iStyle, "a");
	GetSpawnFlags().AddFlags(SF_LIGHT_START_OFF);
	
	if (GetSpawnFlags().Any(SF_LIGHT_START_OFF))
		m_iState = STATE_OFF;
	else
		m_iState = STATE_ON;

	SetCorrectStyle();
}

void CLight::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (m_iStyle >= 32)
	{
		if (!ShouldToggle(useType))
			return;

		switch (GetState())
		{
		case STATE_ON:
		case STATE_TURN_ON:
			if (m_iTurnOffTime)
			{
				m_iState = STATE_TURN_OFF;
				AbsoluteNextThink(m_iTurnOffTime);
			}
			else
				m_iState = STATE_OFF;
			break;
		case STATE_OFF:
		case STATE_TURN_OFF:
			if (m_iTurnOnTime)
			{
				m_iState = STATE_TURN_ON;
				AbsoluteNextThink(m_iTurnOnTime);
			}
			else
				m_iState = STATE_ON;
			break;
		}
		
		SetCorrectStyle();
	}
}