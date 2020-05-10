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
#ifndef GAME_SERVER_ENTITIES_SPAWNPOINTS_CBASESPAWNPOINT_H
#define GAME_SERVER_ENTITIES_SPAWNPOINTS_CBASESPAWNPOINT_H

class CBasePlayer;

class CBaseSpawnPoint : public CPointEntity
{
public:
	DECLARE_CLASS( CBaseSpawnPoint, CPointEntity );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData* pkvd ) override;

	/**
	*	Checks whether the given player can use this spawn point.
	*	@param pPlayer Player trying to spawn here.
	*	@return true if the player is allowed to use this spawn point, false otherwise.
	*/
	virtual bool CanUseSpawnPoint( CBasePlayer* const pPlayer ) { return m_bEnabled; }

	/**
	*	Called whenever a player spawns at this spawn point.
	*	@param pPlayer Player that spawned here.
	*/
	virtual void PlayerSpawned( CBasePlayer* const pPlayer );

private:
	bool m_bEnabled = true;
};

/**
*	Checks if the spot is clear of players.
*	@param pPlayer Player that is trying to spawn.
*	@param pSpawnPoint Spawn point.
*	@return true if the point is valid, false otherwise.
*/
bool IsSpawnPointValid( CBasePlayer* pPlayer, CBaseSpawnPoint* pSpawnPoint );

/**
*	Returns the entity to spawn at.
*
*	USES AND SETS GLOBAL g_pLastSpawn
*	@param pPlayer Player that is trying to spawn.
*	@return Spawn point.
*/
CBaseEntity* EntSelectSpawnPoint( CBasePlayer* pPlayer );

#endif //GAME_SERVER_ENTITIES_SPAWNPOINTS_CBASESPAWNPOINT_H