#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CPathTrack.h"
#include "CFuncTrackTrain.h"

#include "CFuncTrackAuto.h"

LINK_ENTITY_TO_CLASS( func_trackautochange, CFuncTrackAuto );

void CFuncTrackAuto::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CPathTrack *pTarget;

	if( !UseEnabled() )
		return;

	if( m_toggle_state == TS_AT_TOP )
		pTarget = m_trackTop;
	else if( m_toggle_state == TS_AT_BOTTOM )
		pTarget = m_trackBottom;
	else
		pTarget = NULL;

	if( pActivator->ClassnameIs( "func_tracktrain" ) )
	{
		m_code = EvaluateTrain( pTarget );
		// Safe to fire?
		if( m_code == TRAIN_FOLLOWING && m_toggle_state != m_targetState )
		{
			DisableUse();
			if( m_toggle_state == TS_AT_TOP )
				GoDown();
			else
				GoUp();
		}
	}
	else
	{
		if( pTarget )
			pTarget = pTarget->GetNext();
		if( pTarget && m_train->m_ppath != pTarget && ShouldToggle( useType, m_targetState != TS_AT_TOP ) )
		{
			if( m_targetState == TS_AT_TOP )
				m_targetState = TS_AT_BOTTOM;
			else
				m_targetState = TS_AT_TOP;
		}

		UpdateAutoTargets( m_targetState );
	}
}

void CFuncTrackAuto::UpdateAutoTargets( int toggleState )
{
	CPathTrack *pTarget, *pNextTarget;

	if( !m_trackTop || !m_trackBottom )
		return;

	if( m_targetState == TS_AT_TOP )
	{
		pTarget = m_trackTop->GetNext();
		pNextTarget = m_trackBottom->GetNext();
	}
	else
	{
		pTarget = m_trackBottom->GetNext();
		pNextTarget = m_trackTop->GetNext();
	}
	if( pTarget )
	{
		pTarget->GetSpawnFlags().ClearFlags( SF_PATH_DISABLED );
		if( m_code == TRAIN_FOLLOWING && m_train && m_train->GetSpeed() == 0 )
			m_train->Use( this, this, USE_ON, 0 );
	}

	if( pNextTarget )
		pNextTarget->GetSpawnFlags().AddFlags( SF_PATH_DISABLED );

}