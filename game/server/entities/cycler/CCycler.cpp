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
/*

===== h_cycler.cpp ========================================================

  The Halflife Cycler Monsters

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CCycler.h"

BEGIN_DATADESC(	CCycler )
	DEFINE_FIELD( m_animate, FIELD_INTEGER ),
END_DATADESC()

// Cycler member functions

void CCycler :: GenericCyclerSpawn(const char* szModel, Vector vecMin, Vector vecMax)
{
	if (!szModel || !*szModel)
	{
		ALERT(at_error, "cycler at %.0f %.0f %0.f missing modelname", GetAbsOrigin().x, GetAbsOrigin().y, GetAbsOrigin().z );
		UTIL_RemoveNow( this );
		return;
	}

	//TODO: needed? Only the "cycler" entity ever calls this method anyway. - Solokiller
	SetClassname( "cycler" );
	PRECACHE_MODEL( szModel );
	SetModel( szModel );

	CCycler::Spawn( );

	SetSize( vecMin, vecMax );
}

//
// CyclerPain , changes sequences when shot
//
//void CCycler :: Pain( float flDamage )
void CCycler::OnTakeDamage( const CTakeDamageInfo& info )
{
	if( m_animate )
	{
		SetSequence( GetSequence() + 1 );

		ResetSequenceInfo();

		if( m_flFrameRate == 0.0 )
		{
			SetSequence( 0 );
			ResetSequenceInfo();
		}
		SetFrame( 0 );
	}
	else
	{
		SetFrameRate( 1.0 );
		StudioFrameAdvance( 0.1 );
		SetFrameRate( 0 );
		ALERT( at_console, "sequence: %d, frame %.0f\n", GetSequence(), GetFrame() );
	}
}

void CCycler :: Spawn( )
{
	InitBoneControllers();
	SetSolidType( SOLID_SLIDEBOX );
	SetMoveType( MOVETYPE_NONE );
	SetTakeDamageMode( DAMAGE_YES );
	GetEffects().ClearAll();
	SetHealth( 80000 );// no cycler should die
	SetYawSpeed( 5 );
	SetIdealYaw( GetAbsAngles().y );
	ChangeYaw( 360 );
	
	m_flFrameRate		= 75;
	m_flGroundSpeed		= 0;

	SetNextThink( GetNextThink() + 1.0 );

	ResetSequenceInfo( );

	if( GetSequence() != 0 || GetFrame() != 0)
	{
		m_animate = 0;
		SetFrameRate( 0 );
	}
	else
	{
		m_animate = 1;
	}
}

//
// cycler think
//
void CCycler :: Think( void )
{
	SetNextThink( gpGlobals->time + 0.1 );

	if (m_animate)
	{
		StudioFrameAdvance ( );
	}
	if (m_fSequenceFinished && !m_fSequenceLoops)
	{
		// ResetSequenceInfo();
		// hack to avoid reloading model every frame
		SetAnimTime( gpGlobals->time );
		SetFrameRate( 1.0 );
		m_fSequenceFinished = false;
		m_flLastEventCheck = gpGlobals->time;
		SetFrame( 0 );
		if (!m_animate)
			SetFrameRate( 0.0 );	// FIX: don't reset framerate
	}
}

//
// CyclerUse - starts a rotation trend
//
void CCycler :: Use ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	m_animate = !m_animate;
	if (m_animate)
		SetFrameRate( 1.0 );
	else
		SetFrameRate( 0.0 );
}