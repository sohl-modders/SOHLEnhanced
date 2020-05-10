#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerRandom.h"

BEGIN_DATADESC( CTriggerRandom )
	DEFINE_FIELD( m_iTargetCount, FIELD_INTEGER ),
	DEFINE_FIELD( m_iTargetsFiredCount, FIELD_INTEGER ),
	DEFINE_FIELD( m_flMinTime, FIELD_FLOAT ),
	DEFINE_FIELD( m_flMaxTime, FIELD_FLOAT ),
	DEFINE_ARRAY( m_iszTargets, FIELD_STRING, CTriggerRandom::MAX_TARGETS ),
	DEFINE_ARRAY( m_bFiredTargets, FIELD_BOOLEAN, CTriggerRandom::MAX_TARGETS ),
	DEFINE_USEFUNC( RandomUse ),
	DEFINE_USEFUNC( ToggleUse ),
	DEFINE_THINKFUNC( RandomThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_random, CTriggerRandom );

void CTriggerRandom::KeyValue( KeyValueData* pkvd )
{
	if( FStrEq( "targetCount", pkvd->szKeyName ) )
	{
		m_iTargetCount = min<int>( MAX_TARGETS, max( 0L, strtol( pkvd->szValue, nullptr, 10 ) ) );

		pkvd->fHandled = true;
	}
	else if( FStrEq( "minTime", pkvd->szKeyName ) )
	{
		m_flMinTime = strtod( pkvd->szValue, nullptr );

		pkvd->fHandled = true;
	}
	else if( FStrEq( "maxTime", pkvd->szKeyName ) )
	{
		m_flMaxTime = strtod( pkvd->szValue, nullptr );

		pkvd->fHandled = true;
	}
	else if( strncmp( "target", pkvd->szKeyName, 6 ) == 0 )
	{
		const char* const pszIndex = pkvd->szKeyName + 6;
		char* pszEnd;

		unsigned int uiTarget = strtoul( pszIndex, &pszEnd, 10 );

		if( pszIndex != pszEnd )
		{
			//Make zero based.
			--uiTarget;

			if( uiTarget < MAX_TARGETS )
			{
				m_iszTargets[ uiTarget ] = ALLOC_STRING( pkvd->szValue );
			}

			pkvd->fHandled = true;
		}

		//Not a target key, might be intended for the base class.
	}

	if( !pkvd->fHandled )
		BaseClass::KeyValue( pkvd );
}

void CTriggerRandom::Spawn()
{
	if( IsTimed() )
	{
		SetUse( &CTriggerRandom::ToggleUse );
		SetThink( &CTriggerRandom::RandomThink );

		if( ShouldStartOn() )
		{
			SetNextThink( gpGlobals->time + NextTimeDelay() );
		}
		else
		{
			//In case we're being spawned multiple times, cleans up think time if spawnflags changed.
			SetNextThink( 0 );
		}
	}
	else
	{
		SetUse( &CTriggerRandom::RandomUse );
		//In case we're being spawned multiple times, cleans up timed think if spawnflags changed.
		SetThink( nullptr );
	}
}

void CTriggerRandom::RandomUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue )
{
	FireRandomTarget( pActivator, IsUnique() );
}

void CTriggerRandom::ToggleUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue )
{
	bool bToggle = false;

	switch( useType )
	{
	case USE_OFF:
		bToggle = GetNextThink() != 0;
		break;

	case USE_ON:
		bToggle = GetNextThink() == 0;
		break;

	case USE_SET:
		break;

	case USE_TOGGLE:
		bToggle = true;
		break;
	}

	if( bToggle )
	{
		if( GetNextThink() != 0 )
			SetNextThink( 0 );
		else
			SetNextThink( gpGlobals->time + NextTimeDelay() );
	}
}

void CTriggerRandom::RandomThink()
{
	FireRandomTarget( this, IsUnique() );

	if( !ShouldTriggerOnce() )
		SetNextThink( gpGlobals->time + NextTimeDelay() );
}

size_t CTriggerRandom::SelectTarget( const bool bUnique ) const
{
	if( m_iTargetCount <= 0 )
		return INVALID_INDEX;

	//Sanity check: prevents us from doing the loop for no reason.
	if( m_iTargetsFiredCount >= m_iTargetCount && !IsReusable() )
		return INVALID_INDEX;

	int iTotal = 0;

	size_t uiTargetIndex = INVALID_INDEX;

	//Enumerate all entities with the given name and assign it to uiTargetIndex if a random check succeeds.
	for( size_t uiIndex = 0; uiIndex < static_cast<size_t>( m_iTargetCount ); ++uiIndex )
	{
		if( bUnique && m_bFiredTargets[ uiIndex ] )
			continue;

		//Only count targets that haven't been triggered so we've always got a valid target (first valid target is set since < 1 is always true).
		++iTotal;

		if( UTIL_RandomLong( 0, iTotal - 1 ) < 1 )
			uiTargetIndex = uiIndex;
	}

	return uiTargetIndex;
}

bool CTriggerRandom::FireRandomTarget( CBaseEntity* pActivator, const bool bUnique )
{
	const auto uiTarget = SelectTarget( bUnique );

	if( uiTarget == INVALID_INDEX )
		return false;

	if( bUnique )
		m_bFiredTargets[ uiTarget ] = true;

	//Fire main target.
	SUB_UseTargets( pActivator, USE_TOGGLE, 0 );

	FireTargets( STRING( m_iszTargets[ uiTarget ] ), pActivator, this, USE_TOGGLE, 0 );

	++m_iTargetsFiredCount;

	if( m_iTargetsFiredCount >= m_iTargetCount )
	{
		if( IsReusable() )
		{
			m_iTargetsFiredCount = 0;
			memset( m_bFiredTargets, 0, sizeof( m_bFiredTargets ) );
		}
	}

	return true;
}
