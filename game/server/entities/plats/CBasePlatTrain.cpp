#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CBasePlatTrain.h"

BEGIN_DATADESC( CBasePlatTrain )
	DEFINE_FIELD( m_bMoveSnd, FIELD_CHARACTER ),
	DEFINE_FIELD( m_bStopSnd, FIELD_CHARACTER ),
	DEFINE_FIELD( m_volume, FIELD_FLOAT ),
END_DATADESC()

void CBasePlatTrain::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "lip" ) )
	{
		m_flLip = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "wait" ) )
	{
		m_flWait = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "height" ) )
	{
		m_flHeight = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "rotation" ) )
	{
		m_vecFinalAngle.x = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "movesnd" ) )
	{
		m_bMoveSnd = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "stopsnd" ) )
	{
		m_bStopSnd = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "volume" ) )
	{
		m_volume = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseToggle::KeyValue( pkvd );
}

void CBasePlatTrain::Precache( void )
{
	// set the plat's "in-motion" sound
	switch( m_bMoveSnd )
	{
	case	0:
		pev->noiseMoving = MAKE_STRING( "common/null.wav" );
		break;
	case	1:
		PrecacheSound( "plats/bigmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/bigmove1.wav" );
		break;
	case	2:
		PrecacheSound( "plats/bigmove2.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/bigmove2.wav" );
		break;
	case	3:
		PrecacheSound( "plats/elevmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/elevmove1.wav" );
		break;
	case	4:
		PrecacheSound( "plats/elevmove2.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/elevmove2.wav" );
		break;
	case	5:
		PrecacheSound( "plats/elevmove3.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/elevmove3.wav" );
		break;
	case	6:
		PrecacheSound( "plats/freightmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/freightmove1.wav" );
		break;
	case	7:
		PrecacheSound( "plats/freightmove2.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/freightmove2.wav" );
		break;
	case	8:
		PrecacheSound( "plats/heavymove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/heavymove1.wav" );
		break;
	case	9:
		PrecacheSound( "plats/rackmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/rackmove1.wav" );
		break;
	case	10:
		PrecacheSound( "plats/railmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/railmove1.wav" );
		break;
	case	11:
		PrecacheSound( "plats/squeekmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/squeekmove1.wav" );
		break;
	case	12:
		PrecacheSound( "plats/talkmove1.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/talkmove1.wav" );
		break;
	case	13:
		PrecacheSound( "plats/talkmove2.wav" );
		pev->noiseMoving = MAKE_STRING( "plats/talkmove2.wav" );
		break;
	default:
		pev->noiseMoving = MAKE_STRING( "common/null.wav" );
		break;
	}

	// set the plat's 'reached destination' stop sound
	switch( m_bStopSnd )
	{
	case	0:
		pev->noiseArrived = MAKE_STRING( "common/null.wav" );
		break;
	case	1:
		PrecacheSound( "plats/bigstop1.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/bigstop1.wav" );
		break;
	case	2:
		PrecacheSound( "plats/bigstop2.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/bigstop2.wav" );
		break;
	case	3:
		PrecacheSound( "plats/freightstop1.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/freightstop1.wav" );
		break;
	case	4:
		PrecacheSound( "plats/heavystop2.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/heavystop2.wav" );
		break;
	case	5:
		PrecacheSound( "plats/rackstop1.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/rackstop1.wav" );
		break;
	case	6:
		PrecacheSound( "plats/railstop1.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/railstop1.wav" );
		break;
	case	7:
		PrecacheSound( "plats/squeekstop1.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/squeekstop1.wav" );
		break;
	case	8:
		PrecacheSound( "plats/talkstop1.wav" );
		pev->noiseArrived = MAKE_STRING( "plats/talkstop1.wav" );
		break;

	default:
		pev->noiseArrived = MAKE_STRING( "common/null.wav" );
		break;
	}
}