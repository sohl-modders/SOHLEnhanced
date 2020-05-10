#include "extdll.h"
#include "util.h"
#include "gamerules/GameRules.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "CStripWeapons.h"

LINK_ENTITY_TO_CLASS( player_weaponstrip, CStripWeapons );

void CStripWeapons::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBasePlayer *pPlayer = NULL;

	if( pActivator && pActivator->IsPlayer() )
	{
		pPlayer = ( CBasePlayer * ) pActivator;
	}
	else if( !g_pGameRules->IsDeathmatch() )
	{
		pPlayer = ( CBasePlayer * ) CBaseEntity::Instance( g_engfuncs.pfnPEntityOfEntIndex( 1 ) );
	}

	if( pPlayer )
		pPlayer->RemoveAllItems( false );
}