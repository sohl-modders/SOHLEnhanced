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
#include "cbase.h"
#include "CBasePlayer.h"

#include "CGamePlayerEquip.h"

LINK_ENTITY_TO_CLASS( game_player_equip, CGamePlayerEquip );

void CGamePlayerEquip::KeyValue( KeyValueData *pkvd )
{
	CRulePointEntity::KeyValue( pkvd );

	if( !pkvd->fHandled )
	{
		for( int i = 0; i < MAX_EQUIP; i++ )
		{
			if( !m_weaponNames[ i ] )
			{
				char tmp[ 128 ];

				UTIL_StripToken( pkvd->szKeyName, tmp );

				m_weaponNames[ i ] = ALLOC_STRING( tmp );
				m_weaponCount[ i ] = atoi( pkvd->szValue );
				m_weaponCount[ i ] = max( 1, m_weaponCount[ i ] );
				pkvd->fHandled = true;
				break;
			}
		}
	}
}

void CGamePlayerEquip::Touch( CBaseEntity *pOther )
{
	if( !CanFireForActivator( pOther ) )
		return;

	if( UseOnly() )
		return;

	EquipPlayer( pOther );
}

void CGamePlayerEquip::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	EquipPlayer( pActivator );
}

void CGamePlayerEquip::EquipPlayer( CBaseEntity *pEntity )
{
	CBasePlayer *pPlayer = NULL;

	if( pEntity->IsPlayer() )
	{
		pPlayer = ( CBasePlayer * ) pEntity;
	}

	if( !pPlayer )
		return;

	for( int i = 0; i < MAX_EQUIP; i++ )
	{
		if( !m_weaponNames[ i ] )
			break;
		for( int j = 0; j < m_weaponCount[ i ]; j++ )
		{
			pPlayer->GiveNamedItem( STRING( m_weaponNames[ i ] ) );
		}
	}
}