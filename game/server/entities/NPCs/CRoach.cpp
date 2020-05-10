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
//=========================================================
// cockroach
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"Monsters.h"
#include	"Schedule.h"
#include	"entities/CSoundEnt.h"
#include	"Decals.h"

#include "CRoach.h"

BEGIN_DATADESC( CRoach )
	DEFINE_THINKFUNC( MonsterThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( monster_cockroach, CRoach );

int CRoach :: ISoundMask ( void )
{
	return	bits_SOUND_CARCASS | bits_SOUND_MEAT;
}

EntityClassification_t CRoach::GetClassification()
{
	return EntityClassifications().GetClassificationId( classify::INSECT );
}

//=========================================================
// Touch
//=========================================================
void CRoach :: Touch ( CBaseEntity *pOther )
{
	Vector		vecSpot;
	TraceResult	tr;

	if ( pOther->GetAbsVelocity() == g_vecZero || !pOther->IsPlayer() )
	{
		return;
	}

	vecSpot = GetAbsOrigin() + Vector ( 0 , 0 , 8 );//move up a bit, and trace down.
	UTIL_TraceLine ( vecSpot, vecSpot + Vector ( 0, 0, -24 ),  ignore_monsters, ENT(pev), & tr);

	// This isn't really blood.  So you don't have to screen it out based on violence levels (UTIL_ShouldShowBlood())
	UTIL_DecalTrace( &tr, DECAL_YBLOOD1 +RANDOM_LONG(0,5) );

	TakeDamage( pOther, pOther, GetHealth(), DMG_CRUSH );
}

void CRoach::UpdateYawSpeed()
{
	int ys;

	ys = 120;

	SetYawSpeed( ys );
}

void CRoach :: Spawn()
{
	Precache( );

	SetModel( "models/roach.mdl");
	SetSize( Vector( -1, -1, 0 ), Vector( 1, 1, 2 ) );

	SetSolidType( SOLID_SLIDEBOX );
	SetMoveType( MOVETYPE_STEP );
	m_bloodColor		= BLOOD_COLOR_YELLOW;
	GetEffects().ClearAll();
	SetHealth( 1 );
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	MonsterInit();
	SetActivity ( ACT_IDLE );

	SetViewOffset( Vector ( 0, 0, 1 ) );// position of the eyes relative to monster's origin.
	SetTakeDamageMode( DAMAGE_YES );
	m_fLightHacked		= false;
	m_flLastLightLevel	= -1;
	m_iMode				= ROACH_IDLE;
	m_flNextSmellTime	= gpGlobals->time;
}

void CRoach :: Precache()
{
	PRECACHE_MODEL("models/roach.mdl");

	PRECACHE_SOUND("roach/rch_die.wav");
	PRECACHE_SOUND("roach/rch_walk.wav");
	PRECACHE_SOUND("roach/rch_smash.wav");
}	


//=========================================================
// Killed.
//=========================================================
void CRoach::Killed( const CTakeDamageInfo& info, GibAction gibAction )
{
	SetSolidType( SOLID_NOT );

	//random sound
	if ( RANDOM_LONG(0,4) == 1 )
	{
		EMIT_SOUND_DYN( this, CHAN_VOICE, "roach/rch_die.wav", 0.8, ATTN_NORM, 0, 80 + RANDOM_LONG(0,39) );
	}
	else
	{
		EMIT_SOUND_DYN( this, CHAN_BODY, "roach/rch_smash.wav", 0.7, ATTN_NORM, 0, 80 + RANDOM_LONG(0,39) );
	}
	
	CSoundEnt::InsertSound ( bits_SOUND_WORLD, GetAbsOrigin(), 128, 1 );

	CBaseEntity *pOwner = GetOwner();
	if ( pOwner )
	{
		pOwner->DeathNotice( this );
	}
	UTIL_Remove( this );
}

//=========================================================
// MonsterThink, overridden for roaches.
//=========================================================
void CRoach :: MonsterThink( void  )
{
	if ( !UTIL_FindClientInPVS( this ) )
		SetNextThink( gpGlobals->time + RANDOM_FLOAT(1,1.5) );
	else
		SetNextThink( gpGlobals->time + 0.1 );// keep monster thinking

	float flInterval = StudioFrameAdvance( ); // animate

	if ( !m_fLightHacked )
	{
		// if light value hasn't been collection for the first time yet, 
		// suspend the creature for a second so the world finishes spawning, then we'll collect the light level.
		SetNextThink( gpGlobals->time + 1 );
		m_fLightHacked = true;
		return;
	}
	else if ( m_flLastLightLevel < 0 )
	{
		// collect light level for the first time, now that all of the lightmaps in the roach's area have been calculated.
		m_flLastLightLevel = GetLightLevel();
	}

	switch ( m_iMode )
	{
	case	ROACH_IDLE:
	case	ROACH_EAT:
		{
			// if not moving, sample environment to see if anything scary is around. Do a radius search 'look' at random.
			if ( RANDOM_LONG(0,3) == 1 )
			{
				Look( 150 );
				if (HasConditions(bits_COND_SEE_FEAR))
				{
					// if see something scary
					//ALERT ( at_aiconsole, "Scared\n" );
					Eat( 30 +  ( RANDOM_LONG(0,14) ) );// roach will ignore food for 30 to 45 seconds
					PickNewDest( ROACH_SCARED_BY_ENT );
					SetActivity ( ACT_WALK );
				}
				else if ( RANDOM_LONG(0,149) == 1 )
				{
					// if roach doesn't see anything, there's still a chance that it will move. (boredom)
					//ALERT ( at_aiconsole, "Bored\n" );
					PickNewDest( ROACH_BORED );
					SetActivity ( ACT_WALK );

					if ( m_iMode == ROACH_EAT )
					{
						// roach will ignore food for 30 to 45 seconds if it got bored while eating. 
						Eat( 30 +  ( RANDOM_LONG(0,14) ) );
					}
				}
			}
	
			// don't do this stuff if eating!
			if ( m_iMode == ROACH_IDLE )
			{
				if ( FShouldEat() )
				{
					Listen();
				}

				if ( GetLightLevel() > m_flLastLightLevel )
				{
					// someone turned on lights!
					//ALERT ( at_console, "Lights!\n" );
					PickNewDest( ROACH_SCARED_BY_LIGHT );
					SetActivity ( ACT_WALK );
				}
				else if ( HasConditions(bits_COND_SMELL_FOOD) )
				{
					CSound *pSound;

					pSound = CSoundEnt::SoundPointerForIndex( m_iAudibleList );

					// roach smells food and is just standing around. Go to food unless food isn't on same z-plane.
					if ( pSound && fabs( pSound->m_vecOrigin.z - GetAbsOrigin().z ) <= 3 )
					{
						PickNewDest( ROACH_SMELL_FOOD );
						SetActivity ( ACT_WALK );
					}
				}
			}

			break;
		}
	case	ROACH_SCARED_BY_LIGHT:
		{
			// if roach was scared by light, then stop if we're over a spot at least as dark as where we started!
			if ( GetLightLevel() <= m_flLastLightLevel )
			{
				SetActivity ( ACT_IDLE );
				m_flLastLightLevel = GetLightLevel();// make this our new light level.
			}
			break;
		}
	}
	
	if ( m_flGroundSpeed != 0 )
	{
		Move( flInterval );
	}
}

//=========================================================
// Picks a new spot for roach to run to.(
//=========================================================
void CRoach :: PickNewDest ( int iCondition )
{
	Vector	vecNewDir;
	Vector	vecDest;
	float	flDist;

	m_iMode = iCondition;

	if ( m_iMode == ROACH_SMELL_FOOD )
	{
		// find the food and go there.
		CSound *pSound;

		pSound = CSoundEnt::SoundPointerForIndex( m_iAudibleList );

		if ( pSound )
		{
			m_Route[ 0 ].vecLocation.x = pSound->m_vecOrigin.x + ( 3 - RANDOM_LONG(0,5) );
			m_Route[ 0 ].vecLocation.y = pSound->m_vecOrigin.y + ( 3 - RANDOM_LONG(0,5) );
			m_Route[ 0 ].vecLocation.z = pSound->m_vecOrigin.z;
			m_Route[ 0 ].iType = bits_MF_TO_LOCATION;
			m_movementGoal = RouteClassify( m_Route[ 0 ].iType );
			return;
		}
	}

	do 
	{
		// picks a random spot, requiring that it be at least 128 units away
		// else, the roach will pick a spot too close to itself and run in 
		// circles. this is a hack but buys me time to work on the real monsters.
		vecNewDir.x = RANDOM_FLOAT( -1, 1 );
		vecNewDir.y = RANDOM_FLOAT( -1, 1 );
		flDist		= 256 + ( RANDOM_LONG(0,255) );
		vecDest = GetAbsOrigin() + vecNewDir * flDist;

	} while ( ( vecDest - GetAbsOrigin() ).Length2D() < 128 );

	m_Route[ 0 ].vecLocation.x = vecDest.x;
	m_Route[ 0 ].vecLocation.y = vecDest.y;
	m_Route[ 0 ].vecLocation.z = GetAbsOrigin().z;
	m_Route[ 0 ].iType = bits_MF_TO_LOCATION;
	m_movementGoal = RouteClassify( m_Route[ 0 ].iType );

	if ( RANDOM_LONG(0,9) == 1 )
	{
		// every once in a while, a roach will play a skitter sound when they decide to run
		EMIT_SOUND_DYN( this, CHAN_BODY, "roach/rch_walk.wav", 1, ATTN_NORM, 0, 80 + RANDOM_LONG(0,39) );
	}
}

//=========================================================
// roach's move function
//=========================================================
void CRoach :: Move ( float flInterval ) 
{
	float		flWaypointDist;
	Vector		vecApex;

	// local move to waypoint.
	flWaypointDist = ( m_Route[ m_iRouteIndex ].vecLocation - GetAbsOrigin() ).Length2D();
	MakeIdealYaw ( m_Route[ m_iRouteIndex ].vecLocation );

	ChangeYaw ( GetYawSpeed() );
	UTIL_MakeVectors( GetAbsAngles() );

	if ( RANDOM_LONG(0,7) == 1 )
	{
		// randomly check for blocked path.(more random load balancing)
		if ( !UTIL_WalkMove( this, GetIdealYaw(), 4, WALKMOVE_NORMAL ) )
		{
			// stuck, so just pick a new spot to run off to
			PickNewDest( m_iMode );
		}
	}
	
	UTIL_WalkMove( this, GetIdealYaw(), m_flGroundSpeed * flInterval, WALKMOVE_NORMAL );

	// if the waypoint is closer than step size, then stop after next step (ok for roach to overshoot)
	if ( flWaypointDist <= m_flGroundSpeed * flInterval )
	{
		// take truncated step and stop

		SetActivity ( ACT_IDLE );
		m_flLastLightLevel = GetLightLevel();// this is roach's new comfortable light level

		if ( m_iMode == ROACH_SMELL_FOOD )
		{
			m_iMode = ROACH_EAT;
		}
		else
		{
			m_iMode = ROACH_IDLE;
		}
	}

	if ( RANDOM_LONG(0,149) == 1 && m_iMode != ROACH_SCARED_BY_LIGHT && m_iMode != ROACH_SMELL_FOOD )
	{
		// random skitter while moving as long as not on a b-line to get out of light or going to food
		PickNewDest( ROACH_IDLE );
	}
}

//=========================================================
// Look - overriden for the roach, which can virtually see 
// 360 degrees.
//=========================================================
void CRoach :: Look ( int iDistance )
{
	CBaseEntity	*pSightEnt = NULL;// the current visible entity that we're dealing with
	CBaseEntity	*pPreviousEnt;// the last entity added to the link list 
	int			iSighted = 0;

	// DON'T let visibility information from last frame sit around!
	ClearConditions( bits_COND_SEE_HATE |bits_COND_SEE_DISLIKE | bits_COND_SEE_ENEMY | bits_COND_SEE_FEAR );

	// don't let monsters outside of the player's PVS act up, or most of the interesting
	// things will happen before the player gets there!
	if ( !UTIL_FindClientInPVS( this ) )
	{
		return;
	}

	m_pLink = NULL;
	pPreviousEnt = this;

	// Does sphere also limit itself to PVS?
	// Examine all entities within a reasonable radius
	// !!!PERFORMANCE - let's trivially reject the ent list before radius searching!
	while ((pSightEnt = UTIL_FindEntityInSphere( pSightEnt, GetAbsOrigin(), iDistance )) != NULL)
	{
		// only consider ents that can be damaged. !!!temporarily only considering other monsters and clients
		if (  pSightEnt->IsPlayer() || pSightEnt->GetFlags().Any( FL_MONSTER ) )
		{
			if ( /*FVisible( pSightEnt ) &&*/ !pSightEnt->GetFlags().Any( FL_NOTARGET ) && pSightEnt->GetHealth() > 0 )
			{
				// NULL the Link pointer for each ent added to the link list. If other ents follow, the will overwrite
				// this value. If this ent happens to be the last, the list will be properly terminated.
				pPreviousEnt->m_pLink = pSightEnt;
				pSightEnt->m_pLink = NULL;
				pPreviousEnt = pSightEnt;

				// don't add the Enemy's relationship to the conditions. We only want to worry about conditions when
				// we see monsters other than the Enemy.
				switch ( IRelationship ( pSightEnt ) )
				{
				case	R_FR:		
					iSighted |= bits_COND_SEE_FEAR;	
					break;
				case	R_NO:
					break;
				default:
					ALERT ( at_console, "%s can't assess %s\n", GetClassname(), pSightEnt->GetClassname() );
					break;
				}
			}
		}
	}
	SetConditions( iSighted );
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

