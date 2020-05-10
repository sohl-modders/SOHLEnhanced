#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CSaveRestoreBuffer.h"

namespace engine
{
const int g_SaveRestoreSizes[ FIELD_TYPECOUNT ] =
{
	sizeof( float ),		// FIELD_FLOAT
	sizeof( int ),		// FIELD_STRING
	sizeof( int ),		// FIELD_ENTITY
	sizeof( int ),		// FIELD_CLASSPTR
	sizeof( int ),		// FIELD_EHANDLE
	sizeof( int ),		// FIELD_entvars_t
	sizeof( int ),		// FIELD_EDICT
	sizeof( float ) * 3,	// FIELD_VECTOR
	sizeof( float ) * 3,	// FIELD_POSITION_VECTOR
	sizeof( int * ),		// FIELD_POINTER
	sizeof( int ),		// FIELD_INTEGER
#ifdef GNUC
	sizeof( int * ) * 2,		// FIELD_FUNCTION
#else
	sizeof( int * ),		// FIELD_FUNCTION	
#endif
	sizeof( qboolean ),		// FIELD_BOOLEAN
	sizeof( short ),		// FIELD_SHORT
	sizeof( char ),		// FIELD_CHARACTER
	sizeof( float ),		// FIELD_TIME
	sizeof( int ),		// FIELD_MODELNAME
	sizeof( int ),		// FIELD_SOUNDNAME
};

//Verify that all types and no more than that are represented here - Solokiller
static_assert( ARRAYSIZE( g_SaveRestoreSizes ) == FIELD_TYPECOUNT, "SaveRestore sizes data is incorrect!" );

const TYPEDESCRIPTION gEntvarsDescription[] =
{
	ENGINE_DEFINE_ENTITY_FIELD( classname, FIELD_STRING ),
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( globalname, FIELD_STRING ),

	ENGINE_DEFINE_ENTITY_FIELD( origin, FIELD_POSITION_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( oldorigin, FIELD_POSITION_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( velocity, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( basevelocity, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( movedir, FIELD_VECTOR ),

	ENGINE_DEFINE_ENTITY_FIELD( angles, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( avelocity, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( punchangle, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( v_angle, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( fixangle, FIELD_FLOAT ),	//TODO: fixangle is an int. Why is this float? - Solokiller
	ENGINE_DEFINE_ENTITY_FIELD( idealpitch, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( pitch_speed, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( ideal_yaw, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( yaw_speed, FIELD_FLOAT ),

	ENGINE_DEFINE_ENTITY_FIELD( modelindex, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( model, FIELD_MODELNAME ),

	ENGINE_DEFINE_ENTITY_FIELD( viewmodel, FIELD_MODELNAME ),
	ENGINE_DEFINE_ENTITY_FIELD( weaponmodel, FIELD_MODELNAME ),

	ENGINE_DEFINE_ENTITY_FIELD( absmin, FIELD_POSITION_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( absmax, FIELD_POSITION_VECTOR ),
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( mins, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( maxs, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( size, FIELD_VECTOR ),

	ENGINE_DEFINE_ENTITY_FIELD( ltime, FIELD_TIME ),
	ENGINE_DEFINE_ENTITY_FIELD( nextthink, FIELD_TIME ),

	ENGINE_DEFINE_ENTITY_FIELD( solid, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( movetype, FIELD_INTEGER ),

	ENGINE_DEFINE_ENTITY_FIELD( skin, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( body, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( effects, FIELD_INTEGER ),

	ENGINE_DEFINE_ENTITY_FIELD( gravity, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( friction, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( light_level, FIELD_FLOAT ),

	ENGINE_DEFINE_ENTITY_FIELD( frame, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( scale, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( sequence, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( animtime, FIELD_TIME ),
	ENGINE_DEFINE_ENTITY_FIELD( framerate, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( controller, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( blending, FIELD_INTEGER ),

	ENGINE_DEFINE_ENTITY_FIELD( rendermode, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( renderamt, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( rendercolor, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( renderfx, FIELD_INTEGER ),

	ENGINE_DEFINE_ENTITY_FIELD( health, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( frags, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( weapons, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( takedamage, FIELD_FLOAT ),

	ENGINE_DEFINE_ENTITY_FIELD( deadflag, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( view_ofs, FIELD_VECTOR ),
	ENGINE_DEFINE_ENTITY_FIELD( button, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( impulse, FIELD_INTEGER ),

	ENGINE_DEFINE_ENTITY_FIELD( chain, FIELD_EDICT ),
	ENGINE_DEFINE_ENTITY_FIELD( dmg_inflictor, FIELD_EDICT ),
	ENGINE_DEFINE_ENTITY_FIELD( enemy, FIELD_EDICT ),
	ENGINE_DEFINE_ENTITY_FIELD( aiment, FIELD_EDICT ),
	ENGINE_DEFINE_ENTITY_FIELD( owner, FIELD_EDICT ),
	ENGINE_DEFINE_ENTITY_FIELD( groundentity, FIELD_EDICT ),

	ENGINE_DEFINE_ENTITY_FIELD( spawnflags, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( flags, FIELD_FLOAT ),

	ENGINE_DEFINE_ENTITY_FIELD( colormap, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( team, FIELD_INTEGER ),

	ENGINE_DEFINE_ENTITY_FIELD( max_health, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( teleport_time, FIELD_TIME ),
	ENGINE_DEFINE_ENTITY_FIELD( armortype, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( armorvalue, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( waterlevel, FIELD_INTEGER ),
	ENGINE_DEFINE_ENTITY_FIELD( watertype, FIELD_INTEGER ),

	// Having these fields be local to the individual levels makes it easier to test those levels individually.
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( target, FIELD_STRING ),
	ENGINE_DEFINE_ENTITY_GLOBAL_FIELD( targetname, FIELD_STRING ),
	ENGINE_DEFINE_ENTITY_FIELD( netname, FIELD_STRING ),
	ENGINE_DEFINE_ENTITY_FIELD( message, FIELD_STRING ),

	ENGINE_DEFINE_ENTITY_FIELD( dmg_take, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( dmg_save, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( dmg, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( dmgtime, FIELD_TIME ),

	ENGINE_DEFINE_ENTITY_FIELD( noise, FIELD_SOUNDNAME ),
	ENGINE_DEFINE_ENTITY_FIELD( noise1, FIELD_SOUNDNAME ),
	ENGINE_DEFINE_ENTITY_FIELD( noise2, FIELD_SOUNDNAME ),
	ENGINE_DEFINE_ENTITY_FIELD( noise3, FIELD_SOUNDNAME ),
	ENGINE_DEFINE_ENTITY_FIELD( speed, FIELD_FLOAT ),
	ENGINE_DEFINE_ENTITY_FIELD( air_finished, FIELD_TIME ),
	ENGINE_DEFINE_ENTITY_FIELD( pain_finished, FIELD_TIME ),
	ENGINE_DEFINE_ENTITY_FIELD( radsuit_finished, FIELD_TIME ),
};

const size_t gEntvarsCount = ARRAYSIZE( gEntvarsDescription );

// Base class includes common SAVERESTOREDATA pointer, and manages the entity table
CSaveRestoreBuffer::CSaveRestoreBuffer( void )
{
	m_pdata = nullptr;
}

CSaveRestoreBuffer::CSaveRestoreBuffer( SAVERESTOREDATA *pdata )
{
	m_pdata = pdata;
}

CSaveRestoreBuffer::~CSaveRestoreBuffer( void )
{
}

int	CSaveRestoreBuffer::EntityIndex( CBaseEntity *pEntity )
{
	if( pEntity == NULL )
		return -1;
	return EntityIndex( pEntity->pev );
}


int	CSaveRestoreBuffer::EntityIndex( entvars_t *pevLookup )
{
	if( pevLookup == NULL )
		return -1;
	return EntityIndex( ENT( pevLookup ) );
}

int	CSaveRestoreBuffer::EntityIndex( EOFFSET eoLookup )
{
	return EntityIndex( ENT( eoLookup ) );
}

int	CSaveRestoreBuffer::EntityIndex( edict_t *pentLookup )
{
	if( !m_pdata || pentLookup == NULL )
		return -1;

	int i;
	ENTITYTABLE *pTable;

	for( i = 0; i < m_pdata->tableCount; i++ )
	{
		pTable = m_pdata->pTable + i;
		if( pTable->pent == pentLookup )
			return i;
	}
	return -1;
}

edict_t *CSaveRestoreBuffer::EntityFromIndex( int entityIndex )
{
	if( !m_pdata || entityIndex < 0 )
		return NULL;

	int i;
	ENTITYTABLE *pTable;

	for( i = 0; i < m_pdata->tableCount; i++ )
	{
		pTable = m_pdata->pTable + i;
		if( pTable->id == entityIndex )
			return pTable->pent;
	}
	return NULL;
}

int	CSaveRestoreBuffer::EntityFlagsSet( int entityIndex, int flags )
{
	if( !m_pdata || entityIndex < 0 )
		return 0;
	if( entityIndex > m_pdata->tableCount )
		return 0;

	m_pdata->pTable[ entityIndex ].flags |= flags;

	return m_pdata->pTable[ entityIndex ].flags;
}

unsigned short CSaveRestoreBuffer::TokenHash( const char *pszToken )
{
	unsigned short	hash = ( unsigned short ) ( HashString( pszToken ) % ( unsigned ) m_pdata->tokenCount );

#if _DEBUG
	static int tokensparsed = 0;
	tokensparsed++;
	if( !m_pdata->tokenCount || !m_pdata->pTokens )
		ALERT( at_error, "No token table array in TokenHash()!" );
#endif

	for( int i = 0; i<m_pdata->tokenCount; i++ )
	{
#if _DEBUG
		static bool beentheredonethat = false;
		if( i > 50 && !beentheredonethat )
		{
			beentheredonethat = true;
			ALERT( at_error, "CSaveRestoreBuffer :: TokenHash() is getting too full!" );
		}
#endif

		int	index = hash + i;
		if( index >= m_pdata->tokenCount )
			index -= m_pdata->tokenCount;

		if( !m_pdata->pTokens[ index ] || strcmp( pszToken, m_pdata->pTokens[ index ] ) == 0 )
		{
			m_pdata->pTokens[ index ] = ( char * ) pszToken;
			return index;
		}
	}

	// Token hash table full!!! 
	// [Consider doing overflow table(s) after the main table & limiting linear hash table search]
	ALERT( at_error, "CSaveRestoreBuffer :: TokenHash() is COMPLETELY FULL!" );
	return 0;
}

void CSaveRestoreBuffer::BufferRewind( int size )
{
	if( !m_pdata )
		return;

	if( m_pdata->size < size )
		size = m_pdata->size;

	m_pdata->pCurrentData -= size;
	m_pdata->size -= size;
}

unsigned int CSaveRestoreBuffer::HashString( const char *pszToken )
{
	unsigned int	hash = 0;

	while( *pszToken )
		hash = _rotr( hash, 4 ) ^ *pszToken++;

	return hash;
}
}
