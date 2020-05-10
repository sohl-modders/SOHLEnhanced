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
//=========================================================
// Hornets
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"entities/NPCs/Monsters.h"
#include	"Weapons.h"
#include	"entities/CSoundEnt.h"
#include	"CHornet.h"
#include	"gamerules/GameRules.h"


int iHornetTrail;
int iHornetPuff;

LINK_ENTITY_TO_CLASS( hornet, CHornet );

//=========================================================
// Save/Restore
//=========================================================
BEGIN_DATADESC(	CHornet )
	DEFINE_FIELD( m_flStopAttack, FIELD_TIME ),
	DEFINE_FIELD( m_iHornetType, FIELD_INTEGER ),
	DEFINE_FIELD( m_flFlySpeed, FIELD_FLOAT ),
	DEFINE_THINKFUNC( StartTrack ),
	DEFINE_THINKFUNC( StartDart ),
	DEFINE_THINKFUNC( TrackTarget ),
	DEFINE_TOUCHFUNC( TrackTouch ),
	DEFINE_TOUCHFUNC( DartTouch ),
	DEFINE_TOUCHFUNC( DieTouch ),
END_DATADESC()

//=========================================================
// don't let hornets gib, ever.
//=========================================================
void CHornet::OnTakeDamage( const CTakeDamageInfo& info )
{
	CTakeDamageInfo newInfo = info;
	// filter these bits a little.

	newInfo.GetMutableDamageTypes() &= ~( DMG_ALWAYSGIB );
	newInfo.GetMutableDamageTypes() |= DMG_NEVERGIB;

	CBaseMonster::OnTakeDamage( newInfo );
}

//=========================================================
//=========================================================
void CHornet :: Spawn( void )
{
	Precache();

	SetMoveType( MOVETYPE_FLY );
	SetSolidType( SOLID_BBOX );
	SetTakeDamageMode( DAMAGE_YES );
	GetFlags() |= FL_MONSTER;
	SetHealth( 1 );// weak!
	
	if ( g_pGameRules->IsMultiplayer() )
	{
		// hornets don't live as long in multiplayer
		m_flStopAttack = gpGlobals->time + 3.5;
	}
	else
	{
		m_flStopAttack	= gpGlobals->time + 5.0;
	}

	m_flFieldOfView = 0.9; // +- 25 degrees

	if ( RANDOM_LONG ( 1, 5 ) <= 2 )
	{
		m_iHornetType = HORNET_TYPE_RED;
		m_flFlySpeed = HORNET_RED_SPEED;
	}
	else
	{
		m_iHornetType = HORNET_TYPE_ORANGE;
		m_flFlySpeed = HORNET_ORANGE_SPEED;
	}

	SetModel( "models/hornet.mdl");
	SetSize( Vector( -4, -4, -4 ), Vector( 4, 4, 4 ) );

	SetTouch( &CHornet::DieTouch );
	SetThink( &CHornet::StartTrack );

	CBaseEntity* pSoundEnt = GetOwner();
	if ( !pSoundEnt )
		pSoundEnt = this;

	if ( !FNullEnt( GetOwner() ) && GetOwner()->GetFlags().Any( FL_CLIENT ) )
	{
		SetDamage( gSkillData.GetPlrDmgHornet() );
	}
	else
	{
		// no real owner, or owner isn't a client. 
		SetDamage( gSkillData.GetMonDmgHornet() );
	}
	
	SetNextThink( gpGlobals->time + 0.1 );
	ResetSequenceInfo( );
}


void CHornet :: Precache()
{
	PRECACHE_MODEL("models/hornet.mdl");

	PRECACHE_SOUND( "agrunt/ag_fire1.wav" );
	PRECACHE_SOUND( "agrunt/ag_fire2.wav" );
	PRECACHE_SOUND( "agrunt/ag_fire3.wav" );

	PRECACHE_SOUND( "hornet/ag_buzz1.wav" );
	PRECACHE_SOUND( "hornet/ag_buzz2.wav" );
	PRECACHE_SOUND( "hornet/ag_buzz3.wav" );

	PRECACHE_SOUND( "hornet/ag_hornethit1.wav" );
	PRECACHE_SOUND( "hornet/ag_hornethit2.wav" );
	PRECACHE_SOUND( "hornet/ag_hornethit3.wav" );

	iHornetPuff = PRECACHE_MODEL( "sprites/muz1.spr" );
	iHornetTrail = PRECACHE_MODEL("sprites/laserbeam.spr");
}	

//=========================================================
// hornets will never get mad at each other, no matter who the owner is.
//=========================================================
Relationship CHornet::IRelationship ( CBaseEntity *pTarget )
{
	//TODO: not exactly a good identity check - Solokiller
	if ( pTarget->GetModelIndex() == GetModelIndex() )
	{
		return R_NO;
	}

	return CBaseMonster :: IRelationship( pTarget );
}

//=========================================================
// ID's Hornet as their owner
//=========================================================
EntityClassification_t CHornet::GetClassification()
{
	if( GetOwner() && GetOwner()->GetFlags().Any( FL_CLIENT ) )
	{
		return EntityClassifications().GetClassificationId( classify::PLAYER_BIOWEAPON );
	}

	return EntityClassifications().GetClassificationId( classify::ALIEN_BIOWEAPON );
}

//=========================================================
// StartTrack - starts a hornet out tracking its target
//=========================================================
void CHornet :: StartTrack ( void )
{
	IgniteTrail();

	SetTouch( &CHornet::TrackTouch );
	SetThink( &CHornet::TrackTarget );

	SetNextThink( gpGlobals->time + 0.1 );
}

//=========================================================
// StartDart - starts a hornet out just flying straight.
//=========================================================
void CHornet :: StartDart ( void )
{
	IgniteTrail();

	SetTouch( &CHornet::DartTouch );

	SetThink( &CHornet::SUB_Remove );
	SetNextThink( gpGlobals->time + 4 );
}

void CHornet::IgniteTrail( void )
{
/*

  ted's suggested trail colors:

r161
g25
b97

r173
g39
b14

old colors
		case HORNET_TYPE_RED:
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 128 );   // r, g, b
			WRITE_BYTE( 0 );   // r, g, b
			break;
		case HORNET_TYPE_ORANGE:
			WRITE_BYTE( 0   );   // r, g, b
			WRITE_BYTE( 100 );   // r, g, b
			WRITE_BYTE( 255 );   // r, g, b
			break;
	
*/

	// trail
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE(  TE_BEAMFOLLOW );
		WRITE_SHORT( entindex() );	// entity
		WRITE_SHORT( iHornetTrail );	// model
		WRITE_BYTE( 10 ); // life
		WRITE_BYTE( 2 );  // width
		
		switch ( m_iHornetType )
		{
		case HORNET_TYPE_RED:
			WRITE_BYTE( 179 );   // r, g, b
			WRITE_BYTE( 39 );   // r, g, b
			WRITE_BYTE( 14 );   // r, g, b
			break;
		case HORNET_TYPE_ORANGE:
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 128 );   // r, g, b
			WRITE_BYTE( 0 );   // r, g, b
			break;
		}

		WRITE_BYTE( 128 );	// brightness

	MESSAGE_END();
}

//=========================================================
// Hornet is flying, gently tracking target
//=========================================================
void CHornet :: TrackTarget ( void )
{
	Vector	vecFlightDir;
	Vector	vecDirToEnemy;
	float	flDelta;

	StudioFrameAdvance( );

	if (gpGlobals->time > m_flStopAttack)
	{
		SetTouch( NULL );
		SetThink( &CHornet::SUB_Remove );
		SetNextThink( gpGlobals->time + 0.1 );
		return;
	}

	// UNDONE: The player pointer should come back after returning from another level
	if ( m_hEnemy == NULL )
	{// enemy is dead.
		Look( 512 );
		m_hEnemy = BestVisibleEnemy( );
	}
	
	if ( m_hEnemy != NULL && FVisible( m_hEnemy ))
	{
		m_vecEnemyLKP = m_hEnemy->BodyTarget( GetAbsOrigin() );
	}
	else
	{
		m_vecEnemyLKP = m_vecEnemyLKP + GetAbsVelocity() * m_flFlySpeed * 0.1;
	}

	vecDirToEnemy = ( m_vecEnemyLKP - GetAbsOrigin() ).Normalize();

	if ( GetAbsVelocity().Length() < 0.1)
		vecFlightDir = vecDirToEnemy;
	else 
		vecFlightDir = GetAbsVelocity().Normalize();

	// measure how far the turn is, the wider the turn, the slow we'll go this time.
	flDelta = DotProduct ( vecFlightDir, vecDirToEnemy );
	
	if ( flDelta < 0.5 )
	{// hafta turn wide again. play sound
		switch (RANDOM_LONG(0,2))
		{
		case 0:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_buzz1.wav", HORNET_BUZZ_VOLUME, ATTN_NORM);	break;
		case 1:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_buzz2.wav", HORNET_BUZZ_VOLUME, ATTN_NORM);	break;
		case 2:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_buzz3.wav", HORNET_BUZZ_VOLUME, ATTN_NORM);	break;
		}
	}

	if ( flDelta <= 0 && m_iHornetType == HORNET_TYPE_RED )
	{// no flying backwards, but we don't want to invert this, cause we'd go fast when we have to turn REAL far.
		flDelta = 0.25;
	}

	SetAbsVelocity( ( vecFlightDir + vecDirToEnemy).Normalize() );

	if ( GetOwner() && GetOwner()->GetFlags().Any( FL_MONSTER ) )
	{
		// random pattern only applies to hornets fired by monsters, not players. 
		Vector vecVelocity = GetAbsVelocity();
		vecVelocity.x += RANDOM_FLOAT ( -0.10, 0.10 );// scramble the flight dir a bit.
		vecVelocity.y += RANDOM_FLOAT ( -0.10, 0.10 );
		vecVelocity.z += RANDOM_FLOAT ( -0.10, 0.10 );
		SetAbsVelocity( vecVelocity );
	}
	
	switch ( m_iHornetType )
	{
		case HORNET_TYPE_RED:
			SetAbsVelocity( GetAbsVelocity() * ( m_flFlySpeed * flDelta ) );// scale the dir by the ( speed * width of turn )
			SetNextThink( gpGlobals->time + RANDOM_FLOAT( 0.1, 0.3 ) );
			break;
		case HORNET_TYPE_ORANGE:
			SetAbsVelocity( GetAbsVelocity() * m_flFlySpeed );// do not have to slow down to turn.
			SetNextThink( gpGlobals->time + 0.1 );// fixed think time
			break;
	}

	SetAbsAngles( UTIL_VecToAngles( GetAbsVelocity() ) );

	SetSolidType( SOLID_BBOX );

	// if hornet is close to the enemy, jet in a straight line for a half second.
	// (only in the single player game)
	if ( m_hEnemy != NULL && !g_pGameRules->IsMultiplayer() )
	{
		if ( flDelta >= 0.4 && ( GetAbsOrigin() - m_vecEnemyLKP ).Length() <= 300 )
		{
			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, GetAbsOrigin() );
				WRITE_BYTE( TE_SPRITE );
				WRITE_COORD( GetAbsOrigin().x);	// pos
				WRITE_COORD( GetAbsOrigin().y);
				WRITE_COORD( GetAbsOrigin().z);
				WRITE_SHORT( iHornetPuff );		// model
				// WRITE_BYTE( 0 );				// life * 10
				WRITE_BYTE( 2 );				// size * 10
				WRITE_BYTE( 128 );			// brightness
			MESSAGE_END();

			switch (RANDOM_LONG(0,2))
			{
			case 0:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_buzz1.wav", HORNET_BUZZ_VOLUME, ATTN_NORM);	break;
			case 1:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_buzz2.wav", HORNET_BUZZ_VOLUME, ATTN_NORM);	break;
			case 2:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_buzz3.wav", HORNET_BUZZ_VOLUME, ATTN_NORM);	break;
			}
			SetAbsVelocity( GetAbsVelocity() * 2 );
			SetNextThink( gpGlobals->time + 1.0 );
			// don't attack again
			m_flStopAttack = gpGlobals->time;
		}
	}
}

//=========================================================
// Tracking Hornet hit something
//=========================================================
void CHornet :: TrackTouch ( CBaseEntity *pOther )
{
	if ( pOther == GetOwner() || pOther->GetModelIndex() == GetModelIndex() )
	{// bumped into the guy that shot it.
		SetSolidType( SOLID_NOT );
		return;
	}

	if ( IRelationship( pOther ) <= R_NO )
	{
		// hit something we don't want to hurt, so turn around.

		Vector vecVelocity = GetAbsVelocity().Normalize();

		vecVelocity.x *= -1;
		vecVelocity.y *= -1;

		SetAbsOrigin( GetAbsOrigin() + vecVelocity * 4 ); // bounce the hornet off a bit.
		vecVelocity = vecVelocity * m_flFlySpeed;

		SetAbsVelocity( vecVelocity );

		return;
	}

	DieTouch( pOther );
}

void CHornet::DartTouch( CBaseEntity *pOther )
{
	DieTouch( pOther );
}

void CHornet::DieTouch ( CBaseEntity *pOther )
{
	if ( pOther && pOther->GetTakeDamageMode() != DAMAGE_NO )
	{// do the damage

		switch (RANDOM_LONG(0,2))
		{// buzz when you plug someone
			case 0:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_hornethit1.wav", 1, ATTN_NORM);	break;
			case 1:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_hornethit2.wav", 1, ATTN_NORM);	break;
			case 2:	EMIT_SOUND( this, CHAN_VOICE, "hornet/ag_hornethit3.wav", 1, ATTN_NORM);	break;
		}

		CBaseEntity* pOwner = GetOwner();

		//Fall back to using yourself as the attacker if the owner is gone. - Solokiller
		if( !pOwner )
			pOwner = this;
			
		pOther->TakeDamage( this, pOwner, GetDamage(), DMG_BULLET );
	}

	SetModelIndex( 0 );// so will disappear for the 0.1 secs we wait until NEXTTHINK gets rid
	SetSolidType( SOLID_NOT );

	SetThink ( &CHornet::SUB_Remove );
	SetNextThink( gpGlobals->time + 1 );// stick around long enough for the sound to finish!
}

