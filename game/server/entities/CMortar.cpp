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
#include "Weapons.h"

#include "CMortar.h"

BEGIN_DATADESC( CMortar )
	DEFINE_THINKFUNC( MortarExplode ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( monster_mortar, CMortar );

void CMortar::Spawn()
{
	SetMoveType( MOVETYPE_NONE );
	SetSolidType( SOLID_NOT );

	SetDamage( 200 );

	SetThink( &CMortar::MortarExplode );
	SetNextThink( 0 );

	Precache();
}

void CMortar::Precache()
{
	m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
}

void CMortar::MortarExplode( void )
{
#if 1
	// mortar beam
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
	WRITE_BYTE( TE_BEAMPOINTS );
	WRITE_COORD( GetAbsOrigin().x );
	WRITE_COORD( GetAbsOrigin().y );
	WRITE_COORD( GetAbsOrigin().z );
	WRITE_COORD( GetAbsOrigin().x );
	WRITE_COORD( GetAbsOrigin().y );
	WRITE_COORD( GetAbsOrigin().z + 1024 );
	WRITE_SHORT( m_spriteTexture );
	WRITE_BYTE( 0 ); // framerate
	WRITE_BYTE( 0 ); // framerate
	WRITE_BYTE( 1 ); // life
	WRITE_BYTE( 40 );  // width
	WRITE_BYTE( 0 );   // noise
	WRITE_BYTE( 255 );   // r, g, b
	WRITE_BYTE( 160 );   // r, g, b
	WRITE_BYTE( 100 );   // r, g, b
	WRITE_BYTE( 128 );	// brightness
	WRITE_BYTE( 0 );		// speed
	MESSAGE_END();
#endif

#if 0
	// blast circle
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
	WRITE_BYTE( TE_BEAMTORUS );
	WRITE_COORD( GetAbsOrigin().x );
	WRITE_COORD( GetAbsOrigin().y );
	WRITE_COORD( GetAbsOrigin().z + 32 );
	WRITE_COORD( GetAbsOrigin().x );
	WRITE_COORD( GetAbsOrigin().y );
	WRITE_COORD( GetAbsOrigin().z + 32 + GetDamage() * 2 / .2 ); // reach damage radius over .3 seconds
	WRITE_SHORT( m_spriteTexture );
	WRITE_BYTE( 0 ); // startframe
	WRITE_BYTE( 0 ); // framerate
	WRITE_BYTE( 2 ); // life
	WRITE_BYTE( 12 );  // width
	WRITE_BYTE( 0 );   // noise
	WRITE_BYTE( 255 );   // r, g, b
	WRITE_BYTE( 160 );   // r, g, b
	WRITE_BYTE( 100 );   // r, g, b
	WRITE_BYTE( 255 );	// brightness
	WRITE_BYTE( 0 );		// speed
	MESSAGE_END();
#endif

	TraceResult tr;
	UTIL_TraceLine( GetAbsOrigin() + Vector( 0, 0, 1024 ), GetAbsOrigin() - Vector( 0, 0, 1024 ), dont_ignore_monsters, ENT( pev ), &tr );

	Explode( &tr, DMG_BLAST | DMG_MORTAR );
	UTIL_ScreenShake( tr.vecEndPos, 25.0, 150.0, 1.0, 750 );

#if 0
	int pitch = RANDOM_LONG( 95, 124 );
	EMIT_SOUND_DYN( this, CHAN_VOICE, "weapons/mortarhit.wav", 1.0, 0.55, 0, pitch );

	// ForceSound( SNDRADIUS_MP5, bits_SOUND_COMBAT );

	// ExplodeModel( GetAbsOrigin(), 400, g_sModelIndexShrapnel, 30 );

	RadiusDamage( this, GetOwner(), GetDamage(), EntityClassifications().GetNoneId(), DMG_BLAST );

	/*
	if ( RANDOM_FLOAT ( 0 , 1 ) < 0.5 )
	{
	UTIL_DecalTrace( pTrace, DECAL_SCORCH1 );
	}
	else
	{
	UTIL_DecalTrace( pTrace, DECAL_SCORCH2 );
	}
	*/

	SetThink( &CMortar::SUB_Remove );
	SetNextThink( gpGlobals->time + 0.1 );
#endif

}

#if 0
void CMortar::ShootTimed( EVARS *pevOwner, Vector vecStart, float time )
{
	auto pMortar = static_cast<CMortar*>( UTIL_CreateNamedEntity( "monster_mortar" ) );
	pMortar->Spawn();

	TraceResult tr;
	UTIL_TraceLine( vecStart, vecStart + Vector( 0, 0, -1 ) * 4096, ignore_monsters, ENT( pMortar->pev ), &tr );

	pMortar->SetNextThink( gpGlobals->time + time );

	pMortar->SetAbsOrigin( tr.vecEndPos );
}
#endif