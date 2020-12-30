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
#include "ServerInterface.h"

#include "Decals.h"

#include "cbase.h"
#include "Weapons.h"

extern DLL_GLOBAL Vector		g_vecAttackDir;

void SetObjectCollisionBox(entvars_t* pev);

// give health
float CBaseEntity::GiveHealth(float flHealth, int bitsDamageType)
{
	if (GetTakeDamageMode() == DAMAGE_NO)
		return 0;

	// heal
	if (GetHealth() >= GetMaxHealth())
		return 0;

	const float flOldHealth = GetHealth();

	float flNewHealth = GetHealth() + flHealth;

	//TODO: if the entity's health drops below 1, kill it. - Solokiller

	if (flNewHealth > GetMaxHealth())
		flNewHealth = GetMaxHealth();

	SetHealth(flNewHealth);

	return GetHealth() - flOldHealth;
}

// inflict damage on this entity.  bitsDamageType indicates type of damage inflicted, ie: DMG_CRUSH

void CBaseEntity::OnTakeDamage(const CTakeDamageInfo& info)
{
	if (GetTakeDamageMode() == DAMAGE_NO)
		return;

	auto pInflictor = info.GetInflictor();

	// UNDONE: some entity types may be immune or resistant to some bitsDamageType

	// if Attacker == Inflictor, the attack was a melee or other instant-hit attack.
	// (that is, no actual entity projectile was involved in the attack so use the shooter's origin). 
	// Otherwise, an actual missile was involved.
	Vector vecTemp = pInflictor->GetAbsOrigin() - (VecBModelOrigin(this));

	// this global is still used for glass and other non-monster killables, along with decals.
	g_vecAttackDir = vecTemp.Normalize();

	// save damage based on the target's armor level

	// figure momentum add (don't let hurt brushes or other triggers move player)
	if ((!FNullEnt(pInflictor)) && (GetMoveType() == MOVETYPE_WALK || GetMoveType() == MOVETYPE_STEP) && (info.GetAttacker()->GetSolidType() != SOLID_TRIGGER))
	{
		Vector vecDir = GetAbsOrigin() - (pInflictor->GetAbsMin() + pInflictor->GetAbsMax()) * 0.5;
		vecDir = vecDir.Normalize();

		//TODO: use human hull here? - Solokiller
		float flForce = info.GetDamage() * ((32 * 32 * 72.0) / (GetBounds().x * GetBounds().y * GetBounds().z)) * 5;

		if (flForce > 1000.0)
			flForce = 1000.0;
		SetAbsVelocity(GetAbsVelocity() + vecDir * flForce);
	}

	// do the damage
	SetHealth(GetHealth() - info.GetDamage());
	if (GetHealth() <= 0)
	{
		Killed(info, GIB_NORMAL);
		return;
	}

	return;
}

void CBaseEntity::Killed(const CTakeDamageInfo& info, GibAction gibAction)
{
	SetTakeDamageMode(DAMAGE_NO);
	SetDeadFlag(DEAD_DEAD);
	UTIL_Remove(this);
}


CBaseEntity* CBaseEntity::GetNextTarget()
{
	if (!HasTarget())
		return nullptr;

	return UTIL_FindEntityByTargetname(nullptr, GetTarget());
}

bool CBaseEntity::Save(CSave& save)
{
	if (save.WriteEntVars("ENTVARS", pev))
	{
		const DataMap_t* pInstanceDataMap = GetDataMap();
		const DataMap_t* pDataMap = pInstanceDataMap;

		bool bResult = true;

		while (pDataMap)
		{
			bResult = save.WriteFields(pDataMap->pszClassName, this, *pInstanceDataMap, pDataMap->pTypeDesc, pDataMap->uiNumDescriptors);

			if (!bResult)
				return false;

			pDataMap = pDataMap->pParent;
		}

		if (bResult)
		{
			save.WriteString("classificationOverride", EntityClassifications().GetClassificationName(GetClassificationOverride()).c_str());
		}

		return bResult;
	}

	return false;
}

bool CBaseEntity::Restore(CRestore& restore)
{
	bool bResult = restore.ReadEntVars("ENTVARS", pev);

	if (bResult)
	{
		const DataMap_t* pInstanceDataMap = GetDataMap();
		const DataMap_t* pDataMap = pInstanceDataMap;

		while (pDataMap)
		{
			bResult = restore.ReadFields(pDataMap->pszClassName, this, *pInstanceDataMap, pDataMap->pTypeDesc, pDataMap->uiNumDescriptors);

			if (!bResult)
				break;

			pDataMap = pDataMap->pParent;
		}
	}

	if (bResult)
	{
		SetClassificationOverride(EntityClassifications().GetClassificationId(restore.ReadNamedString("classificationOverride")));
	}

	if (GetModelIndex() != 0 && HasModel())
	{
		Vector mins = GetRelMin();	// Set model is about to destroy these
		Vector maxs = GetRelMax();


		PrecacheModel(GetModelName());
		SetModel(GetModelName());
		SetSize(mins, maxs);	// Reset them
	}

	return bResult;
}

void CBaseEntity::SetObjectCollisionBox(void)
{
	::SetObjectCollisionBox(pev);
}

bool CBaseEntity::Intersects(const CBaseEntity* const pOther) const
{
	if (pOther->GetAbsMin().x > GetAbsMax().x ||
		pOther->GetAbsMin().y > GetAbsMax().y ||
		pOther->GetAbsMin().z > GetAbsMax().z ||
		pOther->GetAbsMax().x < GetAbsMin().x ||
		pOther->GetAbsMax().y < GetAbsMin().y ||
		pOther->GetAbsMax().z < GetAbsMin().z)
		return false;
	return true;
}

void CBaseEntity::MakeDormant(void)
{
	GetFlags() |= FL_DORMANT;

	// Don't touch
	SetSolidType(SOLID_NOT);
	// Don't move
	SetMoveType(MOVETYPE_NONE);
	// Don't draw
	GetEffects() |= EF_NODRAW;
	// Don't think
	SetNextThink(0);
	// Relink
	SetAbsOrigin(GetAbsOrigin());
}

bool CBaseEntity::IsDormant() const
{
	return GetFlags().Any(FL_DORMANT);
}

bool CBaseEntity::IsInWorld() const
{
	// position 
	if (GetAbsOrigin().x >= WORLD_BOUNDARY) return false;
	if (GetAbsOrigin().y >= WORLD_BOUNDARY) return false;
	if (GetAbsOrigin().z >= WORLD_BOUNDARY) return false;
	if (GetAbsOrigin().x <= -WORLD_BOUNDARY) return false;
	if (GetAbsOrigin().y <= -WORLD_BOUNDARY) return false;
	if (GetAbsOrigin().z <= -WORLD_BOUNDARY) return false;
	// speed
	if (GetAbsVelocity().x >= MAX_VELOCITY) return false;
	if (GetAbsVelocity().y >= MAX_VELOCITY) return false;
	if (GetAbsVelocity().z >= MAX_VELOCITY) return false;
	if (GetAbsVelocity().x <= -MAX_VELOCITY) return false;
	if (GetAbsVelocity().y <= -MAX_VELOCITY) return false;
	if (GetAbsVelocity().z <= -MAX_VELOCITY) return false;

	return true;
}

bool CBaseEntity::ShouldToggle(USE_TYPE useType, const bool currentState) const
{
	if (useType != USE_TOGGLE && useType != USE_SET)
	{
		if ((currentState && useType == USE_ON) || (!currentState && useType == USE_OFF))
			return false;
	}
	return true;
}

int	CBaseEntity::DamageDecal(int bitsDamageType) const
{
	if (GetRenderMode() == kRenderTransAlpha)
		return -1;

	if (GetRenderMode() != kRenderNormal)
		return DECAL_BPROOF1;

	return DECAL_GUNSHOT1 + RANDOM_LONG(0, 4);
}

void CBaseEntity::SetModel(const char* pszModelName)
{
	if (!FStringNull(pev->model)) //LRC
		pszModelName = (const char*)STRING(pev->model);

	if (!pszModelName || !(*pszModelName)) {
		g_engfuncs.pfnSetModel(edict(), "models/null.mdl");
		return;
	}

	//is this brush model?
	if (pszModelName[0] == '*') {
		g_engfuncs.pfnSetModel(edict(), pszModelName);
		return;
	}

	//verify file exists
	byte* data = LOAD_FILE_FOR_ME(pszModelName, NULL);
	if (data) {
		FREE_FILE(data);
		g_engfuncs.pfnSetModel(edict(), pszModelName);
		return;
	}

	char* ext = COM_FileExtension(pszModelName);
	if (FStrEq(ext, "mdl")) {
		//this is model
		g_engfuncs.pfnSetModel(edict(), "models/error.mdl");
	}
	else if (FStrEq(ext, "spr")) {
		//this is sprite
		g_engfuncs.pfnSetModel(edict(), "sprites/error.spr");
	}
	else {
		//set null model
		g_engfuncs.pfnSetModel(edict(), "models/null.mdl");
	}
}

int CBaseEntity::PrecacheModel(const char* pszModelName)
{
	if (!FStringNull(pev->model)) //LRC
		pszModelName = (const char*)STRING(pev->model);
	
	if (!pszModelName || !*pszModelName) {
		ALERT(at_console, "Warning: modelname not specified\n");
		return g_sModelIndexNullModel; //set null model
	}

	//no need to precacahe brush
	if (pszModelName[0] == '*') return 0;

	//verify file exists
	byte* data = LOAD_FILE_FOR_ME(pszModelName, NULL);
	if (data)
	{
		FREE_FILE(data);
		return g_engfuncs.pfnPrecacheModel(pszModelName);
	}

	char* ext = COM_FileExtension(pszModelName);
	if (FStrEq(ext, "mdl"))
	{
		//this is model
		ALERT(at_console, "Warning: model \"%s\" not found!\n", pszModelName);
		return g_sModelIndexErrorModel;
	}

	if (FStrEq(ext, "spr"))
	{
		//this is sprite
		ALERT(at_console, "Warning: sprite \"%s\" not found!\n", pszModelName);
		return g_sModelIndexErrorSprite;
	}

	//unknown format
	ALERT(at_console, "Warning: invalid name \"%s\"!\n", pszModelName);
	return g_sModelIndexNullModel; //set null model
}

int CBaseEntity::PrecacheSound(const char* pszSoundName)
{
	if (!pszSoundName || !*pszSoundName)
		return g_sSoundIndexNullSound; //set null sound

	//NOTE: Engine function as predicted for sound folder
	//But LOAD_FILE_FOR_ME don't known about this. Set it manualy

	char path[256];
	const char* sound = pszSoundName;		//sounds from model events can contains a symbol '*'.
	//remove this for sucessfully loading a sound	
	if (sound[0] == '*') sound++;	//only for fake path, engine needs this prefix!
	sprintf(path, "sound/%s", sound);

	//verify file exists
	byte* data = LOAD_FILE_FOR_ME(path, NULL);
	if (data)
	{
		FREE_FILE(data);
		return g_engfuncs.pfnPrecacheSound(pszSoundName);
	}

	char* ext = COM_FileExtension(pszSoundName);
	if (FStrEq(ext, "wav"))
	{
		//this is sound
		ALERT(at_console, "Warning: sound \"%s\" not found!\n", pszSoundName);
		return g_sSoundIndexNullSound; //set null sound
	}
	else
	{
		//unknown format
		ALERT(at_console, "Warning: invalid name \"%s\"!\n", pszSoundName);
		return g_sSoundIndexNullSound; //set null sound
	}
}

unsigned short CBaseEntity::PrecacheEvent(int type, const char* psz)
{
	byte* data = LOAD_FILE_FOR_ME((char*)psz, NULL);
	if (data) {
		FREE_FILE(data);
		return g_engfuncs.pfnPrecacheEvent(type, psz);
	}

	ALERT(at_console, "Warning: event \"%s\" not found!\n", psz);
	return g_engfuncs.pfnPrecacheEvent(type, "events/null.sc");
}

// NOTE: szName must be a pointer to constant memory, e.g. "monster_class" because the entity
// will keep a pointer to it after this call.
CBaseEntity* CBaseEntity::Create(const char* const pszName, const Vector& vecOrigin, const Vector& vecAngles, edict_t* pentOwner, const bool bSpawnEntity)
{
	//TODO: alloc for custom ents - Solokiller
	edict_t* pent = CREATE_NAMED_ENTITY(MAKE_STRING(pszName));
	if (FNullEnt(pent))
	{
		ALERT(at_console, "NULL Ent in Create!\n");
		return nullptr;
	}
	CBaseEntity* pEntity = Instance(pent);
	pEntity->pev->owner = pentOwner;
	pEntity->SetAbsOrigin(vecOrigin);
	pEntity->SetAbsAngles(vecAngles);

	if (bSpawnEntity)
	{
		//This didn't use to handle self removing entities. - Solokiller
		if (-1 == DispatchSpawn(pEntity->edict()))
			return nullptr;
	}

	return pEntity;
}