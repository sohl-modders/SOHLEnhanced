/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef GAME_SERVER_ENTITIES_NPCS_SCRIPTED_CCINEAI_H
#define GAME_SERVER_ENTITIES_NPCS_SCRIPTED_CCINEAI_H

#include "CCineMonster.h"

// when a monster finishes an AI scripted sequence, we can choose
// a schedule to place them in. These defines are the aliases to
// resolve worldcraft input to real schedules (sjb)
#define SCRIPT_FINISHSCHED_DEFAULT	0
#define SCRIPT_FINISHSCHED_AMBUSH	1

class CCineAI : public CCineMonster
{
public:
	DECLARE_CLASS( CCineAI, CCineMonster );

	bool StartSequence( CBaseMonster *pTarget, int iszSeq, const bool completeOnEmpty ) override;
	void PossessEntity() override;
	bool FCanOverrideState() const override;
	virtual void FixScriptMonsterSchedule( CBaseMonster *pMonster ) override;
};

#endif //GAME_SERVER_ENTITIES_NPCS_SCRIPTED_CCINEAI_H