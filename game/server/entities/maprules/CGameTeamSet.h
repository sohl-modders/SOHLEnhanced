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
#ifndef GAME_SERVER_ENTITIES_MAPRULES_CGAMETEAMSET_H
#define GAME_SERVER_ENTITIES_MAPRULES_CGAMETEAMSET_H

#include "CRulePointEntity.h"

#define SF_TEAMSET_FIREONCE			0x0001
#define SF_TEAMSET_CLEARTEAM		0x0002

//
// CGameTeamSet / game_team_set	-- Changes the team of the entity it targets to the activator's team
// Flag: Fire once
// Flag: Clear team				-- Sets the team to "NONE" instead of activator

class CGameTeamSet : public CRulePointEntity
{
public:
	DECLARE_CLASS( CGameTeamSet, CRulePointEntity );

	void		Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	inline bool RemoveOnFire() const { return GetSpawnFlags().Any( SF_TEAMSET_FIREONCE ); }
	inline bool ShouldClearTeam() const { return GetSpawnFlags().Any( SF_TEAMSET_CLEARTEAM ); }

private:
};

#endif //GAME_SERVER_ENTITIES_MAPRULES_CGAMETEAMSET_H