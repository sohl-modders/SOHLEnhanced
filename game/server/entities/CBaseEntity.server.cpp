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
#include "CServerGameInterface.h"
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
	}
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
	ThinkCorrection();
	
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

void CBaseEntity::SetObjectCollisionBox()
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

void CBaseEntity::MakeDormant()
{
	GetFlags() |= FL_DORMANT;

	// Don't touch
	SetSolidType(SOLID_NOT);
	// Don't move
	SetMoveType(MOVETYPE_NONE);
	// Don't draw
	GetEffects() |= EF_NODRAW;
	// Don't think
	DontThink();
	// Relink
	SetAbsOrigin(GetAbsOrigin());
}

bool CBaseEntity::IsDormant() const
{
	return GetFlags().Any(FL_DORMANT);
	//return FBitSet( pev->flags, FL_DORMANT );
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

bool CBaseEntity::ShouldToggle(USE_TYPE useType) const
{
	if (useType != USE_TOGGLE && useType != USE_SET)
	{
		switch(GetState())
		{
		case STATE_ON:
		case STATE_TURN_ON:
			if (useType == USE_ON) 
				return false;
			break;
		case STATE_OFF:
		case STATE_TURN_OFF:
			if (useType == USE_OFF) 
				return false;
			break;
		default:
			return false;
		}
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
	byte* data = LOAD_FILE_FOR_ME(pszModelName, nullptr);
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
	if (!pszModelName || !*pszModelName) {
		ALERT(at_console, "Warning: modelname not specified\n");
		return g_sModelIndexNullModel; //set null model
	}

	//no need to precacahe brush
	if (pszModelName[0] == '*') return 0;

	//verify file exists
	byte* data = LOAD_FILE_FOR_ME(pszModelName, nullptr);
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
	byte* data = LOAD_FILE_FOR_ME(path, nullptr);
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
	byte* data = LOAD_FILE_FOR_ME((char*)psz, nullptr);
	if (data) {
		FREE_FILE(data);
		return g_engfuncs.pfnPrecacheEvent(type, psz);
	}

	ALERT(at_console, "Warning: event \"%s\" not found!\n", psz);
	return g_engfuncs.pfnPrecacheEvent(type, "events/null.sc");
}

void CBaseEntity::Activate()
{
	if (GetDesiredFlags() & LF_ASSISTLIST)
	{
		UTIL_AddToAssistList(this);
	}

	if (GetDesiredFlags() & LF_ALIASLIST)
	{
//		UTIL_AddToAliasList((CBaseAlias*)this); //TODO
	}

	if (m_activated) 
		return;
	
	m_activated = true;
	
	SetParent(m_MoveWith);
	PostSpawn();
}

//g-cont. upgrade system to xashParentSystem 0.3 beta
void CBaseEntity::SetParent(int m_iNewParent, int m_iAttachment)
{
	if (!m_iNewParent) //unlink entity from chain
	{
		ResetParent();
		return;
	}

	CBaseEntity* pParent;

	if (!m_iAttachment) //try to extract aiment from name
	{
		char* name = (char*)STRING(m_iNewParent);
		for (char* c = name; *c; c++)
		{
			if (*c == '.')
			{
				m_iAttachment = atoi(c + 1);
				name[strlen(name) - 2] = 0;
				pParent = UTIL_FindEntityByTargetname(nullptr, name);
				SetParent(pParent, m_iAttachment);
				return;
			}
		}
	}

	pParent = UTIL_FindEntityByTargetname(nullptr, STRING(m_iNewParent));
	SetParent(pParent, m_iAttachment);//check pointer to valid later
}

void CBaseEntity::SetParent(CBaseEntity* pParent, int m_iAttachment)
{
	m_pMoveWith = pParent;

	if (!m_pMoveWith)
	{
		ALERT(at_console, "Missing movewith entity %s\n", STRING(m_MoveWith));
		return;
	}

	if(pev->targetname)
		ALERT(at_console,"Init: %s %s moves with %s\n", STRING(pev->classname), STRING(pev->targetname), STRING(m_MoveWith));
	else
		ALERT(at_console,"Init: %s moves with %s\n", STRING(pev->classname), STRING(m_MoveWith));

		//check for himself parent
	if (m_pMoveWith == this)
	{
		ALERT(at_console, "%s has himself parent!\n", STRING(pev->classname));
		return;
	}

	CBaseEntity* pSibling = m_pMoveWith->m_pChildMoveWith;
	while (pSibling) // check that this entity isn't already in the list of children
	{
		if (pSibling == this) break;
		pSibling = pSibling->m_pSiblingMoveWith;
	}
	
	if (!pSibling) // if movewith is being set up for the first time...
	{
		m_pSiblingMoveWith = m_pMoveWith->m_pChildMoveWith; // may be null: that's fine by me.
		m_pMoveWith->m_pChildMoveWith = this;

		if (m_iAttachment)//parent has attcahment
		{
			if (m_iLFlags & LF_POINTENTITY || pev->flags & FL_MONSTER)
			{
				pev->skin = ENTINDEX(m_pMoveWith->edict());
				pev->body = m_iAttachment;
				pev->aiment = m_pMoveWith->edict();
				pev->movetype = MOVETYPE_FOLLOW;
			}
			else //error
			{
				ALERT(at_console, "%s not following with aiment %d!(yet)\n", STRING(pev->classname), m_iAttachment);
			}
			return;
		}
		else//appllayed to origin
		{
			if (pev->movetype == MOVETYPE_NONE)
			{
				if (pev->solid == SOLID_BSP)
					pev->movetype = MOVETYPE_PUSH;
				else pev->movetype = MOVETYPE_NOCLIP; // or _FLY, perhaps?
				SetBits(m_iLFlags, LF_MOVENONE); //member movetype
			}

			if (m_pMoveWith->pev->movetype == MOVETYPE_WALK)//parent is walking monster?
			{
				SetBits(m_iLFlags, LF_POSTORG);//copy pos from parent every frame
				pev->solid = SOLID_NOT;//set non solid
			}
			m_vecParentOrigin = m_pMoveWith->pev->origin;
			m_vecParentAngles = m_pMoveWith->pev->angles;
		}

		// was the parent shifted at spawn-time?
		if (m_pMoveWith->m_vecSpawnOffset != g_vecZero)
		{
			UTIL_AssignOrigin(this, pev->origin + m_pMoveWith->m_vecSpawnOffset);
			m_vecSpawnOffset = m_vecSpawnOffset + m_pMoveWith->m_vecSpawnOffset;
		}

		m_vecOffsetOrigin = pev->origin - m_vecParentOrigin;
		m_vecOffsetAngles = pev->angles - m_vecParentAngles;

		if ((m_pMoveWith->m_iLFlags & LF_ANGULAR && m_vecOffsetOrigin != g_vecZero) || m_pMoveWith->m_iLFlags & LF_POINTENTITY)
		{
			SetBits(m_iLFlags, LF_POSTORG);//magic stuff
			//GetPInfo( this );
		}

		if (g_Server.IsActive())
		{
			pev->velocity = pev->velocity + m_pMoveWith->pev->velocity;
			pev->avelocity = pev->avelocity + m_pMoveWith->pev->avelocity;
		}
	}
}

void CBaseEntity::ResetParent()
{
	CBaseEntity* pTemp;

	if (m_iLFlags & LF_MOVENONE)//this entity was static e.g. func_wall
	{
		ClearBits(m_iLFlags, LF_MOVENONE);
		pev->movetype = MOVETYPE_NONE;
	}

	if (!CWorld::GetInstance())
	{
		ALERT(at_console, "ResetParent has no AssistList!\n");
		return;
	}

	//LRC - remove this from the AssistList.
	for (pTemp = CWorld::GetInstance(); pTemp->m_pAssistLink != nullptr; pTemp = pTemp->m_pAssistLink)
	{
		if (this == pTemp->m_pAssistLink)
		{
			//			ALERT(at_console,"REMOVE: %s removed from the Assist List.\n", STRING(pev->classname));
			pTemp->m_pAssistLink = this->m_pAssistLink;
			this->m_pAssistLink = nullptr;
			break;
		}
	}

	//LRC
	if (m_pMoveWith)
	{
		// if I'm moving with another entity, take me out of the list. (otherwise things crash!)
		pTemp = m_pMoveWith->m_pChildMoveWith;
		if (pTemp == this)
		{
			m_pMoveWith->m_pChildMoveWith = this->m_pSiblingMoveWith;
		}
		else
		{
			while (pTemp->m_pSiblingMoveWith)
			{
				if (pTemp->m_pSiblingMoveWith == this)
				{
					pTemp->m_pSiblingMoveWith = this->m_pSiblingMoveWith;
					break;
				}
				pTemp = pTemp->m_pSiblingMoveWith;
			}

		}

		ALERT(at_console,"REMOVE: %s removed from the %s ChildMoveWith list.\n", 
			STRING(pev->classname), STRING(m_pMoveWith->pev->targetname));
	}

	if (m_pChildMoveWith)
	{
		CBaseEntity* pCur = m_pChildMoveWith;
		while (pCur != nullptr)
		{
			CBaseEntity* pNext = pCur->m_pSiblingMoveWith;
			UTIL_SetMoveWithVelocity(pCur, g_vecZero, 100);
			UTIL_SetMoveWithAvelocity(pCur, g_vecZero, 100);
			pCur->m_pMoveWith = nullptr;
			pCur->m_pSiblingMoveWith = nullptr;
			pCur = pNext;
		}
	}
}

void CBaseEntity::ClearPointers()
{
	m_pChildMoveWith = nullptr;
	m_pSiblingMoveWith = nullptr;
	m_pAssistLink = nullptr;
}

void CBaseEntity::DontThink()
{
	m_fNextThink = 0;
	if (m_pMoveWith == nullptr && m_pChildMoveWith == nullptr)
	{
		pev->nextthink = 0;
		m_fPevNextThink = 0;
	}

	ALERT(at_console, "DontThink for %s\n", GetTargetname());
}

void CBaseEntity::SetEternalThink()
{
	if (pev->movetype == MOVETYPE_PUSH)
	{
		pev->nextthink = pev->ltime + 1E6;
		m_fPevNextThink = pev->nextthink;
	}

	for (CBaseEntity* pChild = m_pChildMoveWith; pChild != nullptr; pChild = pChild->m_pSiblingMoveWith)
		pChild->SetEternalThink();
}

void CBaseEntity::SetNextThink(const float delay, const bool correctSpeed)
{
	// now monsters use this method, too.
	if (m_pMoveWith || m_pChildMoveWith || pev->flags & FL_MONSTER)
	{
		// use the Assist system, so that thinking doesn't mess up movement.
		if (pev->movetype == MOVETYPE_PUSH)
			m_fNextThink = pev->ltime + delay;
		else
			m_fNextThink = gpGlobals->time + delay;
		
		SetEternalThink();
		
		UTIL_MarkForAssist(this, correctSpeed);

		ALERT(at_console, "SetAssistedThink for %s: %f\n", GetTargetname(), m_fNextThink);
	}
	else
	{
		if (pev->movetype == MOVETYPE_PUSH)
			pev->nextthink = pev->ltime + delay;
		else
			pev->nextthink = gpGlobals->time + delay;

		m_fPevNextThink = m_fNextThink = pev->nextthink;

		if(pev->classname)
			ALERT(at_console, "SetNormThink for %s: %f\n", GetTargetname(), m_fNextThink);
	}
}

void CBaseEntity::AbsoluteNextThink(const float time, const bool correctSpeed)
{
	if (m_pMoveWith || m_pChildMoveWith)
	{
		m_fNextThink = time;
		SetEternalThink();
		UTIL_MarkForAssist(this, correctSpeed);
	}
	else
	{
		pev->nextthink = time;
		m_fPevNextThink = m_fNextThink = pev->nextthink;
	}
}

void CBaseEntity::ThinkCorrection()
{
	if (pev->nextthink != m_fPevNextThink)
	{
		ALERT(at_console, "StoredThink corrected for %s \"%s\": %f -> %f\n", 
			GetClassname(), GetTargetname(), m_fNextThink, 
			m_fNextThink + pev->nextthink - m_fPevNextThink);
		m_fNextThink += pev->nextthink - m_fPevNextThink;
		m_fPevNextThink = pev->nextthink;
	}
}

// NOTE: szName must be a pointer to constant memory, e.g. "monster_class" because the entity
// will keep a pointer to it after this call.
CBaseEntity* CBaseEntity::Create(const char* const pszName, const Vector& vecOrigin, const Vector& vecAngles, edict_t* pentOwner, const bool bSpawnEntity)
{
	if (!pszName)
	{
		ALERT(at_console, "Create() - No item name!\n");
		UTIL_LogPrintf("Create() - No item name!\n");
		return nullptr;
	}
	
	edict_t* pent = CREATE_NAMED_ENTITY(MAKE_STRING(pszName));
	if (FNullEnt(pent))
	{
		ALERT(at_console, "NULL Ent in Create! (%s)\n", pszName);
		UTIL_LogPrintf("NULL Ent in Create! \"%s\"\n", STRING(pszName));
		return nullptr;
	}
	
	CBaseEntity* pEntity = Instance(pent);
	pEntity->SetOwner(pentOwner);
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