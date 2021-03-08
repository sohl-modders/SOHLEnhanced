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
#ifndef GAME_SERVER_CINFOALIAS_H
#define GAME_SERVER_CINFOALIAS_H

#define SF_ALIAS_OFF 1
#define SF_ALIAS_DEBUG 2

class CInfoAlias : public CBaseAlias
{
public:
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	void Spawn() override;
	
	STATE GetState()
	{
		return (pev->spawnflags & SF_ALIAS_OFF) ? STATE_OFF : STATE_ON;
	}

	CBaseEntity* FollowAlias(CBaseEntity* pFrom) override;
	
	void ChangeValue(int iszValue) override;
	void FlushChanges() override;
};

#endif //GAME_SERVER_CINFOALIAS_H