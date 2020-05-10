#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "CPlayerHudColors.h"

//TODO: really need to overhaul save/restore to support colors at the lowest level - Solokiller
BEGIN_DATADESC( CPlayerHudColors )
	DEFINE_FIELD( m_HudColors.m_PrimaryColor, FIELD_INTEGER ),
	DEFINE_FIELD( m_HudColors.m_EmptyItemColor, FIELD_INTEGER ),
	DEFINE_FIELD( m_HudColors.m_AmmoBarColor, FIELD_INTEGER ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( player_hudcolors, CPlayerHudColors );

void CPlayerHudColors::KeyValue( KeyValueData* pkvd )
{
	if( FStrEq( pkvd->szKeyName, "primary_hud_color" ) )
	{
		UTIL_StringToColor( m_HudColors.m_PrimaryColor, 3, pkvd->szValue );

		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "empty_item_color" ) )
	{
		UTIL_StringToColor( m_HudColors.m_EmptyItemColor, 3, pkvd->szValue );

		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "ammo_bar_color" ) )
	{
		UTIL_StringToColor( m_HudColors.m_AmmoBarColor, 3, pkvd->szValue );

		pkvd->fHandled = true;
	}
	else
		BaseClass::KeyValue( pkvd );
}

void CPlayerHudColors::Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue )
{
	if( GetSpawnFlags().Any( SF_PLR_HUDCOLORS_ALLPLAYERS ) )
	{
		CBasePlayer* pPlayer;

		for( int iPlayer = 1; iPlayer <= gpGlobals->maxClients; ++iPlayer )
		{
			pPlayer = UTIL_PlayerByIndex( iPlayer );

			if( !pPlayer || !pPlayer->IsConnected() )
				continue;

			if( GetSpawnFlags().Any( SF_PLR_HUDCOLORS_RESET ) )
				pPlayer->ResetHudColors();
			else
			{
				pPlayer->SetHudColors( m_HudColors );
			}
		}
	}
	else
	{
		if( !pActivator || !pActivator->IsPlayer() )
		{
			Alert( at_warning, "player_hudcolors(%s): activator was null or not a player!\n", GetTargetname() );
			return;
		}

		auto pPlayer = static_cast<CBasePlayer*>( pActivator );

		if( GetSpawnFlags().Any( SF_PLR_HUDCOLORS_RESET ) )
			pPlayer->ResetHudColors();
		else
		{
			pPlayer->SetHudColors( m_HudColors );
		}
	}
}
