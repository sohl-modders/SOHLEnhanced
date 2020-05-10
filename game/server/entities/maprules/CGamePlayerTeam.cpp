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
#include "gamerules/GameRules.h"
#include "cbase.h"

#include "CGamePlayerTeam.h"

LINK_ENTITY_TO_CLASS( game_player_team, CGamePlayerTeam );

void CGamePlayerTeam::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !CanFireForActivator( pActivator ) )
		return;

	if( pActivator->IsPlayer() )
	{
		const char *pszTargetTeam = TargetTeamName( GetTarget() );
		if( pszTargetTeam )
		{
			CBasePlayer *pPlayer = ( CBasePlayer * ) pActivator;
			g_pGameRules->ChangePlayerTeam( pPlayer, pszTargetTeam, ShouldKillPlayer(), ShouldGibPlayer() );
		}
	}

	if( RemoveOnFire() )
	{
		UTIL_Remove( this );
	}
}

const char *CGamePlayerTeam::TargetTeamName( const char *pszTargetName )
{
	CBaseEntity *pTeamEntity = NULL;

	while( ( pTeamEntity = UTIL_FindEntityByTargetname( pTeamEntity, pszTargetName ) ) != NULL )
	{
		if( pTeamEntity->ClassnameIs( "game_team_master" ) )
			return pTeamEntity->TeamID();
	}

	return NULL;
}