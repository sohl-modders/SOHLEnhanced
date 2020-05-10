/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#include "extdll.h"
#include "util.h"

#include "TempEntity.h"

void BeamPoints( 
	const Vector& origin, const Vector& vecStartPos, const Vector& vecEndPos, 
	const int iSpriteModel, const int iFrame, const float flFrameRate, const float flLife, const int iWidth, const int iNoise,
	const Vector& vecColor, const float flBrightness, const int iSpeed )
{
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, origin );
		WRITE_BYTE( TE_BEAMPOINTS );
		WRITE_COORD_VECTOR( vecStartPos );
		WRITE_COORD_VECTOR( vecEndPos );
		WRITE_SHORT( iSpriteModel );
		WRITE_BYTE( iFrame );
		WRITE_BYTE( ( int ) flFrameRate );
		WRITE_BYTE( ( int ) ( flLife * 10.0 ) );
		WRITE_BYTE( iWidth );
		WRITE_BYTE( iNoise ); 
		WRITE_BYTE( ( int ) vecColor.x );
		WRITE_BYTE( ( int ) vecColor.y );
		WRITE_BYTE( ( int ) vecColor.z );
		WRITE_BYTE( flBrightness );
		WRITE_BYTE( iSpeed );
	MESSAGE_END();
}

void StreakSplash( const Vector &origin, const Vector &direction, int color, int count, int speed, int velocityRange )
{
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, origin );
		WRITE_BYTE( TE_STREAK_SPLASH );
		WRITE_COORD( origin.x );		// origin
		WRITE_COORD( origin.y );
		WRITE_COORD( origin.z );
		WRITE_COORD( direction.x );	// direction
		WRITE_COORD( direction.y );
		WRITE_COORD( direction.z );
		WRITE_BYTE( color );	// Streak color 6
		WRITE_SHORT( count );	// count
		WRITE_SHORT( speed );
		WRITE_SHORT( velocityRange );	// Random velocity modifier
	MESSAGE_END();
}

// ---------------------------------
//
// Mortar
//
// ---------------------------------
void SpriteSpray( const Vector &position, const Vector &direction, int spriteModel, int count )
{
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, position );
	WRITE_BYTE( TE_SPRITE_SPRAY );
	WRITE_COORD( position.x );	// pos
	WRITE_COORD( position.y );
	WRITE_COORD( position.z );
	WRITE_COORD( direction.x );	// dir
	WRITE_COORD( direction.y );
	WRITE_COORD( direction.z );
	WRITE_SHORT( spriteModel );	// model
	WRITE_BYTE( count );			// count
	WRITE_BYTE( 130 );			// speed
	WRITE_BYTE( 80 );			// noise ( client will divide by 100 )
	MESSAGE_END();
}