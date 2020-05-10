#include "extdll.h"
#include "util.h"
#include "gamerules/GameRules.h"
#include "cbase.h"
#include "ServerInterface.h"

#include "CFireAndDie.h"

#include "CChangeLevel.h"

LINK_ENTITY_TO_CLASS( info_landmark, CPointEntity );

extern DLL_GLOBAL bool		g_fGameOver;

FILE_GLOBAL char st_szNextMap[ cchMapNameMost ];
FILE_GLOBAL char st_szNextSpot[ cchMapNameMost ];

int BuildChangeList( LEVELLIST *pLevelList, int maxList )
{
	return CChangeLevel::ChangeList( pLevelList, maxList );
}

// Global Savedata for changelevel trigger
BEGIN_DATADESC( CChangeLevel )
	DEFINE_ARRAY( m_szMapName, FIELD_CHARACTER, cchMapNameMost ),
	DEFINE_ARRAY( m_szLandmarkName, FIELD_CHARACTER, cchMapNameMost ),
	DEFINE_FIELD( m_changeTarget, FIELD_STRING ),
	DEFINE_FIELD( m_changeTargetDelay, FIELD_FLOAT ),

	DEFINE_USEFUNC( UseChangeLevel ),
	DEFINE_THINKFUNC( ExecuteChangeLevel ),
	DEFINE_TOUCHFUNC( TouchChangeLevel ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_changelevel, CChangeLevel );

void CChangeLevel::Spawn( void )
{
	if( FStrEq( m_szMapName, "" ) )
		ALERT( at_console, "a trigger_changelevel doesn't have a map" );

	if( FStrEq( m_szLandmarkName, "" ) )
		ALERT( at_console, "trigger_changelevel to %s doesn't have a landmark", m_szMapName );

	if( HasTargetname() )
	{
		SetUse( &CChangeLevel::UseChangeLevel );
	}
	InitTrigger();
	if( !GetSpawnFlags().Any( SF_CHANGELEVEL_USEONLY ) )
		SetTouch( &CChangeLevel::TouchChangeLevel );
	//	ALERT( at_console, "TRANSITION: %s (%s)\n", m_szMapName, m_szLandmarkName );
}

//
// Cache user-entity-field values until spawn is called.
//
void CChangeLevel::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "map" ) )
	{
		if( strlen( pkvd->szValue ) >= cchMapNameMost )
			ALERT( at_error, "Map name '%s' too long (32 chars)\n", pkvd->szValue );
		strcpy( m_szMapName, pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "landmark" ) )
	{
		if( strlen( pkvd->szValue ) >= cchMapNameMost )
			ALERT( at_error, "Landmark name '%s' too long (32 chars)\n", pkvd->szValue );
		strcpy( m_szLandmarkName, pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "changetarget" ) )
	{
		m_changeTarget = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "changedelay" ) )
	{
		m_changeTargetDelay = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseTrigger::KeyValue( pkvd );
}

//=========================================================
// CChangeLevel :: Use - allows level transitions to be 
// triggered by buttons, etc.
//
//=========================================================
void CChangeLevel::UseChangeLevel( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	ChangeLevelNow( pActivator );
}

void CChangeLevel::ExecuteChangeLevel( void )
{
	MESSAGE_BEGIN( MSG_ALL, SVC_CDTRACK );
	WRITE_BYTE( 3 );
	WRITE_BYTE( 3 );
	MESSAGE_END();

	MESSAGE_BEGIN( MSG_ALL, SVC_INTERMISSION );
	MESSAGE_END();
}

//
// GLOBALS ASSUMED SET:  st_szNextMap
//
void CChangeLevel::TouchChangeLevel( CBaseEntity *pOther )
{
	if( !pOther->IsPlayer() )
		return;

	ChangeLevelNow( pOther );
}

void CChangeLevel::ChangeLevelNow( CBaseEntity *pActivator )
{
	ASSERT( !FStrEq( m_szMapName, "" ) );

	// Don't work in deathmatch
	if( g_pGameRules->IsDeathmatch() )
		return;

	// Some people are firing these multiple times in a frame, disable
	if( gpGlobals->time == GetDamageTime() )
		return;

	SetDamageTime( gpGlobals->time );


	CBaseEntity *pPlayer = CBaseEntity::Instance( g_engfuncs.pfnPEntityOfEntIndex( 1 ) );
	if( !InTransitionVolume( pPlayer, m_szLandmarkName ) )
	{
		ALERT( at_aiconsole, "Player isn't in the transition volume %s, aborting\n", m_szLandmarkName );
		return;
	}

	// Create an entity to fire the changetarget
	if( m_changeTarget )
	{
		auto pFireAndDie = static_cast<CFireAndDie*>( UTIL_CreateNamedEntity( "fireanddie" ) );
		if( pFireAndDie )
		{
			// Set target and delay
			pFireAndDie->SetTarget( m_changeTarget );
			pFireAndDie->m_flDelay = m_changeTargetDelay;
			pFireAndDie->SetAbsOrigin( pPlayer->GetAbsOrigin() );
			// Call spawn
			DispatchSpawn( pFireAndDie->edict() );
		}
	}
	// This object will get removed in the call to CHANGE_LEVEL, copy the params into "safe" memory
	strcpy( st_szNextMap, m_szMapName );

	m_hActivator = pActivator;
	SUB_UseTargets( pActivator, USE_TOGGLE, 0 );
	st_szNextSpot[ 0 ] = 0;	// Init landmark to NULL

							// look for a landmark entity		
	CBaseEntity* pLandmark = FindLandmark( m_szLandmarkName );
	if( !FNullEnt( pLandmark ) )
	{
		strcpy( st_szNextSpot, m_szLandmarkName );
		gpGlobals->vecLandmarkOffset = pLandmark->GetAbsOrigin();
	}
	//	ALERT( at_console, "Level touches %d levels\n", ChangeList( levels, 16 ) );
	ALERT( at_console, "CHANGE LEVEL: %s %s\n", st_szNextMap, st_szNextSpot );
	CHANGE_LEVEL( st_szNextMap, st_szNextSpot );
}

CBaseEntity* CChangeLevel::FindLandmark( const char* const pszLandmarkName )
{
	CBaseEntity* pLandmark = nullptr;

	while( ( pLandmark = UTIL_FindEntityByTargetname( pLandmark, pszLandmarkName ) ) != nullptr )
	{
		// Found the landmark
		if( pLandmark->ClassnameIs( "info_landmark" ) )
			return pLandmark;
	}
	ALERT( at_error, "Can't find landmark %s\n", pszLandmarkName );
	return nullptr;
}

// We can only ever move 512 entities across a transition
#define MAX_ENTITY 512

// This has grown into a complicated beast
// Can we make this more elegant?
// This builds the list of all transitions on this level and which entities are in their PVS's and can / should
// be moved across.
int CChangeLevel::ChangeList( LEVELLIST *pLevelList, int maxList )
{
	int count = 0;

	CBaseEntity* pChangelevel = nullptr;

	// Find all of the possible level changes on this BSP
	while( ( pChangelevel = UTIL_FindEntityByClassname( pChangelevel, "trigger_changelevel" ) ) != nullptr )
	{
		CBaseEntity* pLandmark;

		CChangeLevel* pTrigger = static_cast<CChangeLevel*>( pChangelevel );
		// Find the corresponding landmark
		pLandmark = FindLandmark( pTrigger->m_szLandmarkName );
		if( pLandmark )
		{
			// Build a list of unique transitions
			if( AddTransitionToList( pLevelList, count, pTrigger->m_szMapName, pTrigger->m_szLandmarkName, pLandmark ) )
			{
				++count;
				if( count >= maxList )		// FULL!!
					break;
			}
		}
	}

	if( count == 0 )
		return 0;

	if( gpGlobals->pSaveData && ( ( SAVERESTOREDATA * ) gpGlobals->pSaveData )->pTable )
	{
		CSave saveHelper( ( SAVERESTOREDATA * ) gpGlobals->pSaveData );

		for( int i = 0; i < count; i++ )
		{
			int j, entityCount = 0;
			CBaseEntity *pEntList[ MAX_ENTITY ];
			int			 entityFlags[ MAX_ENTITY ];

			// Follow the linked list of entities in the PVS of the transition landmark
			edict_t *pent = UTIL_EntitiesInPVS( pLevelList[ i ].pentLandmark );

			// Build a list of valid entities in this linked list (we're going to use pent->v.chain again)
			while( !FNullEnt( pent ) )
			{
				CBaseEntity *pEntity = CBaseEntity::Instance( pent );
				if( pEntity )
				{
					//					ALERT( at_console, "Trying %s\n", pEntity->GetClassname() );
					int caps = pEntity->ObjectCaps();
					if( !( caps & FCAP_DONT_SAVE ) )
					{
						int flags = 0;

						// If this entity can be moved or is global, mark it
						if( caps & FCAP_ACROSS_TRANSITION )
							flags |= EntTableFlag::MOVEABLE;
						if( pEntity->HasGlobalName() && !pEntity->IsDormant() )
							flags |= EntTableFlag::GLOBAL;
						if( flags )
						{
							pEntList[ entityCount ] = pEntity;
							entityFlags[ entityCount ] = flags;
							entityCount++;
							if( entityCount > MAX_ENTITY )
								ALERT( at_error, "Too many entities across a transition!" );
						}
						//						else
						//							ALERT( at_console, "Failed %s\n", pEntity->GetClassname() );
					}
					//					else
					//						ALERT( at_console, "DON'T SAVE %s\n", pEntity->GetClassname() );
				}
				pent = pent->v.chain;
			}

			for( j = 0; j < entityCount; j++ )
			{
				// Check to make sure the entity isn't screened out by a trigger_transition
				if( entityFlags[ j ] && InTransitionVolume( pEntList[ j ], pLevelList[ i ].landmarkName ) )
				{
					// Mark entity table with 1<<i
					int index = saveHelper.EntityIndex( pEntList[ j ] );
					// Flag it with the level number
					saveHelper.EntityFlagsSet( index, entityFlags[ j ] | ( 1 << i ) );
				}
				//				else
				//					ALERT( at_console, "Screened out %s\n", pEntList[j]->GetClassname() );

			}
		}
	}

	return count;
}

// Add a transition to the list, but ignore duplicates 
// (a designer may have placed multiple trigger_changelevels with the same landmark)
int CChangeLevel::AddTransitionToList( LEVELLIST *pLevelList, int listCount, const char *pMapName, const char *pLandmarkName, CBaseEntity* pLandmark )
{
	int i;

	if( !pLevelList || !pMapName || !pLandmarkName || !pLandmark )
		return 0;

	for( i = 0; i < listCount; i++ )
	{
		if( pLevelList[ i ].pentLandmark == pLandmark->edict() && strcmp( pLevelList[ i ].mapName, pMapName ) == 0 )
			return 0;
	}
	strcpy( pLevelList[ listCount ].mapName, pMapName );
	strcpy( pLevelList[ listCount ].landmarkName, pLandmarkName );
	pLevelList[ listCount ].pentLandmark = pLandmark->edict();
	pLevelList[ listCount ].vecLandmarkOrigin = pLandmark->GetAbsOrigin();

	return 1;
}

bool CChangeLevel::InTransitionVolume( CBaseEntity *pEntity, char *pVolumeName )
{
	if( pEntity->ObjectCaps() & FCAP_FORCE_TRANSITION )
		return true;

	// If you're following another entity, follow it through the transition (weapons follow the player)
	if( pEntity->GetMoveType() == MOVETYPE_FOLLOW )
	{
		auto pAimEnt = pEntity->GetAimEntity();
		if( pAimEnt )
			pEntity = pAimEnt;
	}

	bool inVolume = true;	// Unless we find a trigger_transition, everything is in the volume

	CBaseEntity* pVolume = nullptr;
	while( ( pVolume = UTIL_FindEntityByTargetname( pVolume, pVolumeName ) ) != nullptr )
	{
		if( pVolume->ClassnameIs( "trigger_transition" ) )
		{
			if( pVolume->Intersects( pEntity ) )	// It touches one, it's in the volume
				return true;
			else
				inVolume = false;	// Found a trigger_transition, but I don't intersect it -- if I don't find another, don't go!
		}
	}

	return inVolume;
}