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
#include "entities/CSoundEnt.h"
#include "CCorpse.h"
#include "effects/CMessage.h"

#include "client.h"
#include "gamerules/GameRules.h"

#include "nodes/Nodes.h"
#include "Decals.h"

#include "CMap.h"

extern CGraph WorldGraph;
extern CSoundEnt* pSoundEnt;
extern CBaseEntity* g_pLastSpawn;

extern DLL_GLOBAL bool g_fGameOver;
extern DLL_GLOBAL bool gDisplayTitle;

extern void W_Precache();

//=======================
// CWorld
//
// This spawns first when each level begins.
//=======================

BEGIN_DATADESC(CWorld)
	DEFINE_FIELD(m_iszMapScript, FIELD_STRING),
	DEFINE_FIELD(m_iszGMR, FIELD_STRING),
END_DATADESC()

LINK_ENTITY_TO_CLASS(worldspawn, CWorld);

CWorld* CWorld::m_pInstance = nullptr;

void CWorld::OnCreate()
{
	BaseClass::OnCreate();

	//There should be only 1 worldspawn instance.
	ASSERT(!m_pInstance);

	m_pInstance = this;

	//Due to how save/restore works, we can't just move all of this stuff over to CMap.
	//Only data that must be available before any entities are created should be moved over to CMap. - Solokiller
}

void CWorld::OnDestroy()
{
	//Destroy it here so custom gamerules aren't kept too long. - Solokiller
	if (g_pGameRules)
	{
		delete g_pGameRules;
		g_pGameRules = nullptr;
	}

	//Should be the only place where this is called. - Solokiller
	//TODO: move to after entities are destroyed - Solokiller
	CMap::DestroyInstance();

	ASSERT(m_pInstance);

	m_pInstance = nullptr;

	BaseClass::OnDestroy();

	//TODO: this might not be the best place to put the pool clear call - Solokiller
	g_StringPool.Clear();
}

void CWorld::Spawn()
{
	LoadGMR();

	g_fGameOver = false;
	Precache();
}

void CWorld::Precache()
{
	m_pAssistLink = NULL;
	//m_pFirstAlias = NULL;
	g_pLastSpawn = NULL;

#if 1
	CVAR_SET_STRING("sv_gravity", "800"); // 67ft/sec
	CVAR_SET_STRING("sv_stepsize", "18");
#else
	CVAR_SET_STRING("sv_gravity", "384"); // 32ft/sec
	CVAR_SET_STRING("sv_stepsize", "24");
#endif

	CVAR_SET_STRING("room_type", "0");// clear DSP

	// Set up game rules
	if (g_pGameRules)
	{
		delete g_pGameRules;
		g_pGameRules = nullptr;
	}

	InstallGameRules();

	//!!!UNDONE why is there so much Spawn code in the Precache function? I'll just keep it here 

	///!!!LATER - do we want a sound ent in deathmatch? (sjb)
	//pSoundEnt = CBaseEntity::Create( "soundent", g_vecZero, g_vecZero, edict() );
	g_pSoundEnt = static_cast<CSoundEnt*>(UTIL_CreateNamedEntity("soundent"));

	if (g_pSoundEnt)
		g_pSoundEnt->Spawn();
	else
		ALERT(at_console, "**COULD NOT CREATE SOUNDENT**\n");

	InitBodyQue();

	// init sentence group playback stuff from sentences.txt.
	// ok to call this multiple times, calls after first are ignored.

	g_Sentences.Initialize();

	// init texture type array from materials.txt

	//TODO: if custom materials were to be added, init them here. - Solokiller
	//TEXTURETYPE_Init();


	// the area based ambient sounds MUST be the first precache_sounds

	// player precaches     
	W_Precache();									// get weapon precaches

	ClientPrecache();

	// sounds used from C physics code
	PRECACHE_SOUND("common/null.wav");				// clears sound channels

	PrecacheSound("items/suitchargeok1.wav");//!!! temporary sound for respawning weapons.
	PrecacheSound("items/gunpickup2.wav");// player picks up a gun.

	PrecacheSound("common/bodydrop3.wav");// dead bodies hitting the ground (animation events)
	PrecacheSound("common/bodydrop4.wav");

	PrecacheModel("models/hgibs.mdl");
	PrecacheModel("models/agibs.mdl");

	PrecacheSound("weapons/ric1.wav");
	PrecacheSound("weapons/ric2.wav");
	PrecacheSound("weapons/ric3.wav");
	PrecacheSound("weapons/ric4.wav");
	PrecacheSound("weapons/ric5.wav");

	// 0 normal
	for (size_t i = 0; i <= 13; i++)
	{
		LIGHT_STYLE(i, (char*)STRING(GetStdLightStyle(i)));
	}

	// styles 32-62 are assigned by the light program for switchable lights

	// 63 testing
	LIGHT_STYLE(63, "a");

	for (size_t i = 0; i < gDecalsSize; i++)
		gDecals[i].index = DECAL_INDEX(gDecals[i].name);

	// init the WorldGraph.
	WorldGraph.InitGraph();

	// make sure the .NOD file is newer than the .BSP file.
	if (!WorldGraph.CheckNODFile(STRING(gpGlobals->mapname)))
	{
		// NOD file is not present, or is older than the BSP file.
		WorldGraph.AllocNodes();
	}
	else
	{
		// Load the node graph for this level
		if (!WorldGraph.FLoadGraph(STRING(gpGlobals->mapname)))
		{
			// couldn't load, so alloc and prepare to build a graph.
			ALERT(at_console, "*Error opening .NOD file\n");
			WorldGraph.AllocNodes();
		}
		else
		{
			ALERT(at_console, "\n*Graph Loaded!\n");
		}
	}

	if (GetSpeed() > 0)
		CVAR_SET_FLOAT("sv_zmax", GetSpeed());
	else
		CVAR_SET_FLOAT("sv_zmax", 4096);

	if (HasNetName())
	{
		ALERT(at_aiconsole, "Chapter title: %s\n", GetNetName());
		CBaseEntity* pEntity = Create("env_message", g_vecZero, g_vecZero, NULL);
		if (pEntity)
		{
			pEntity->SetThink(&CBaseEntity::SUB_CallUseToggle);
			pEntity->SetMessage(GetNetName());
			ClearNetName();
			pEntity->SetNextThink(0.3);
			pEntity->GetSpawnFlags().Set(SF_MESSAGE_ONCE);
		}
	}

	if (GetSpawnFlags().Any(SF_WORLD_DARK))
		CVAR_SET_FLOAT("v_dark", 1.0);
	else
		CVAR_SET_FLOAT("v_dark", 0.0);

	if (GetSpawnFlags().Any(SF_WORLD_TITLE))
		gDisplayTitle = true;		// display the game title if this key is set
	else
		gDisplayTitle = false;

	if (GetSpawnFlags().Any(SF_WORLD_FORCETEAM))
	{
		CVAR_SET_FLOAT("mp_defaultteam", 1);
	}
	else
	{
		CVAR_SET_FLOAT("mp_defaultteam", 0);
	}
}

//
// Just to ignore the "wad" field.
//
void CWorld::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "skyname"))
	{
		// Sent over net now.
		CVAR_SET_STRING("sv_skyname", pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "sounds"))
	{
		gpGlobals->cdAudioTrack = atoi(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "WaveHeight"))
	{
		// Sent over net now.
		SetScale(atof(pkvd->szValue) * (1.0 / 8.0));
		pkvd->fHandled = true;
		CVAR_SET_FLOAT("sv_wateramp", GetScale());
	}
	else if (FStrEq(pkvd->szKeyName, "MaxRange"))
	{
		SetSpeed(atof(pkvd->szValue));
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "chaptertitle"))
	{
		SetNetName(ALLOC_STRING(pkvd->szValue));
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "startdark"))
	{
		// UNDONE: This is a gross hack!!! The CVAR is NOT sent over the client/sever link
		// but it will work for single player
		int flag = atoi(pkvd->szValue);
		pkvd->fHandled = true;
		if (flag)
			GetSpawnFlags() |= SF_WORLD_DARK;
	}
	else if (FStrEq(pkvd->szKeyName, "newunit"))
	{
		// Single player only.  Clear save directory if set
		if (atoi(pkvd->szValue))
			CVAR_SET_FLOAT("sv_newunit", 1);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "gametitle"))
	{
		if (atoi(pkvd->szValue))
			GetSpawnFlags() |= SF_WORLD_TITLE;

		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "mapteams"))
	{
		pev->team = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "defaultteam"))
	{
		if (atoi(pkvd->szValue))
		{
			GetSpawnFlags() |= SF_WORLD_FORCETEAM;
		}
		pkvd->fHandled = true;
	}
	/*else if (FStrEq(pkvd->szKeyName, "startsuit"))
	{
		g_startSuit = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}*/
	else if (FStrEq(pkvd->szKeyName, "allowmonsters"))
	{
		CVAR_SET_FLOAT("mp_allowmonsters", atof(pkvd->szValue));
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "primary_hud_color"))
	{
		Color color;

		UTIL_StringToColor(color, 3, pkvd->szValue);

		CMap::GetInstance()->SetPrimaryHudColor(color);

		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "empty_item_color"))
	{
		Color color;

		UTIL_StringToColor(color, 3, pkvd->szValue);

		CMap::GetInstance()->SetEmptyItemHudColor(color);

		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "ammo_bar_color"))
	{
		Color color;

		UTIL_StringToColor(color, 3, pkvd->szValue);

		CMap::GetInstance()->SetAmmoBarHudColor(color);

		pkvd->fHandled = true;
	}
	else if (FStrEq(pkvd->szKeyName, "global_model_replacement"))
	{
		m_iszGMR = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = true;
	}
	else
		CBaseEntity::KeyValue(pkvd);
}

bool CWorld::Save(CSave& save)
{
	if (!BaseClass::Save(save))
		return false;

	return CMap::GetInstance()->Save(save);
}

bool CWorld::Restore(CRestore& restore)
{
	if (!BaseClass::Restore(restore))
		return false;

	if (!CMap::GetInstance()->Restore(restore))
		return false;

	LoadGMR();

	return true;
}

void CWorld::LoadGMR()
{
	if (!FStringNull(m_iszGMR))
		CMap::GetInstance()->LoadGlobalModelReplacement(STRING(m_iszGMR));
}
