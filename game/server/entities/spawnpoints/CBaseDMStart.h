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
#ifndef GAME_SERVER_ENTITIES_SPAWNPOINTS_CBASEDMSTART_H
#define GAME_SERVER_ENTITIES_SPAWNPOINTS_CBASEDMSTART_H

#include "CBaseSpawnPoint.h"

class CBaseDMStart : public CBaseSpawnPoint
{
public:
	DECLARE_CLASS( CBaseDMStart, CBaseSpawnPoint );

	void KeyValue( KeyValueData* pkvd ) override;
	bool IsTriggered( const CBaseEntity* const pActivator ) const override;

	bool CanUseSpawnPoint( CBasePlayer* const pPlayer ) override;

private:
};

#endif //GAME_SERVER_ENTITIES_SPAWNPOINTS_CBASEDMSTART_H