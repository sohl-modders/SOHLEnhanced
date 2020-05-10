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
// icthyosaur - evin, satan fish monster
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Monsters.h"
#include "Schedule.h"
#include "CFlyingMonster.h"
#include "nodes/Nodes.h"
#include "entities/CSoundEnt.h"
#include "animation.h"
#include "Effects.h"
#include "Weapons.h"

#include "CIchthyosaur.h"

#define SEARCH_RETRY	16

extern CGraph WorldGraph;

#define EYE_MAD		0
#define EYE_BASE	1
#define EYE_CLOSED	2
#define EYE_BACK	3
#define EYE_LOOK	4

const char *CIchthyosaur::pIdleSounds[] =
{
	"ichy/ichy_idle1.wav",
	"ichy/ichy_idle2.wav",
	"ichy/ichy_idle3.wav",
	"ichy/ichy_idle4.wav",
};

const char *CIchthyosaur::pAlertSounds[] =
{
	"ichy/ichy_alert2.wav",
	"ichy/ichy_alert3.wav",
};

const char *CIchthyosaur::pAttackSounds[] =
{
	"ichy/ichy_attack1.wav",
	"ichy/ichy_attack2.wav",
};

const char *CIchthyosaur::pBiteSounds[] =
{
	"ichy/ichy_bite1.wav",
	"ichy/ichy_bite2.wav",
};

const char *CIchthyosaur::pPainSounds[] =
{
	"ichy/ichy_pain2.wav",
	"ichy/ichy_pain3.wav",
	"ichy/ichy_pain5.wav",
};

const char *CIchthyosaur::pDieSounds[] =
{
	"ichy/ichy_die2.wav",
	"ichy/ichy_die4.wav",
};

BEGIN_DATADESC(	CIchthyosaur )
	DEFINE_FIELD( m_SaveVelocity, FIELD_VECTOR ),
	DEFINE_FIELD( m_idealDist, FIELD_FLOAT ),
	DEFINE_FIELD( m_flBlink, FIELD_FLOAT ),
	DEFINE_FIELD( m_flEnemyTouched, FIELD_FLOAT ),
	DEFINE_FIELD( m_bOnAttack, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flMaxSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_flMinSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_flMaxDist, FIELD_FLOAT ),
	DEFINE_FIELD( m_flNextAlert, FIELD_TIME ),
	DEFINE_USEFUNC( CombatUse ),
	DEFINE_TOUCHFUNC( BiteTouch ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( monster_ichthyosaur, CIchthyosaur );

#define EMIT_ICKY_SOUND( chan, array ) \
	EMIT_SOUND_DYN ( this, chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], 1.0, 0.6, 0, RANDOM_LONG(95,105) ); 


void CIchthyosaur :: IdleSound( void )	
{ 
	EMIT_ICKY_SOUND( CHAN_VOICE, pIdleSounds ); 
}

void CIchthyosaur :: AlertSound( void ) 
{ 
	EMIT_ICKY_SOUND( CHAN_VOICE, pAlertSounds ); 
}

void CIchthyosaur :: AttackSound( void ) 
{ 
	EMIT_ICKY_SOUND( CHAN_VOICE, pAttackSounds );
}

void CIchthyosaur :: BiteSound( void ) 
{ 
	EMIT_ICKY_SOUND( CHAN_WEAPON, pBiteSounds );
}

void CIchthyosaur :: DeathSound( void ) 
{ 
	EMIT_ICKY_SOUND( CHAN_VOICE, pDieSounds ); 
}

void CIchthyosaur :: PainSound( void )	
{ 
	EMIT_ICKY_SOUND( CHAN_VOICE, pPainSounds ); 
}

//=========================================================
// monster-specific tasks and states
//=========================================================
enum 
{
	TASK_ICHTHYOSAUR_CIRCLE_ENEMY = LAST_COMMON_TASK + 1,
	TASK_ICHTHYOSAUR_SWIM,
	TASK_ICHTHYOSAUR_FLOAT,
};

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

static Task_t	tlSwimAround[] =
{
	{ TASK_SET_ACTIVITY,			(float)ACT_WALK },
	{ TASK_ICHTHYOSAUR_SWIM,		0.0 },
};

static Schedule_t	slSwimAround[] =
{
	{ 
		tlSwimAround,
		ARRAYSIZE(tlSwimAround), 
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_SEE_ENEMY		|
		bits_COND_NEW_ENEMY		|
		bits_COND_HEAR_SOUND,
		bits_SOUND_PLAYER |
		bits_SOUND_COMBAT,
		"SwimAround"
	},
};

static Task_t	tlSwimAgitated[] =
{
	{ TASK_STOP_MOVING,				(float) 0 },
	{ TASK_SET_ACTIVITY,			(float)ACT_RUN },
	{ TASK_WAIT,					(float)2.0 },
};

static Schedule_t	slSwimAgitated[] =
{
	{ 
		tlSwimAgitated,
		ARRAYSIZE(tlSwimAgitated), 
		0, 
		0, 
		"SwimAgitated"
	},
};


static Task_t	tlCircleEnemy[] =
{
	{ TASK_SET_ACTIVITY,			(float)ACT_WALK },
	{ TASK_ICHTHYOSAUR_CIRCLE_ENEMY, 0.0 },
};

static Schedule_t	slCircleEnemy[] =
{
	{ 
		tlCircleEnemy,
		ARRAYSIZE(tlCircleEnemy), 
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_CAN_MELEE_ATTACK1 |
		bits_COND_CAN_RANGE_ATTACK1,
		0,
		"CircleEnemy"
	},
};


Task_t tlTwitchDie[] =
{
	{ TASK_STOP_MOVING,			0		 },
	{ TASK_SOUND_DIE,			(float)0 },
	{ TASK_DIE,					(float)0 },
	{ TASK_ICHTHYOSAUR_FLOAT,	(float)0 },
};

Schedule_t slTwitchDie[] =
{
	{
		tlTwitchDie,
		ARRAYSIZE( tlTwitchDie ),
		0,
		0,
		"Die"
	},
};


BEGIN_SCHEDULES(CIchthyosaur)
    slSwimAround,
	slSwimAgitated,
	slCircleEnemy,
	slTwitchDie,
END_SCHEDULES()

EntityClassification_t CIchthyosaur::GetClassification()
{
	return EntityClassifications().GetClassificationId( classify::ALIEN_MONSTER );
}

bool CIchthyosaur :: CheckMeleeAttack1 ( float flDot, float flDist )
{
	if ( flDot >= 0.7 && m_flEnemyTouched > gpGlobals->time - 0.2 )
	{
		return true;
	}
	return false;
}

void CIchthyosaur::BiteTouch( CBaseEntity *pOther )
{
	// bite if we hit who we want to eat
	if ( pOther == m_hEnemy ) 
	{
		m_flEnemyTouched = gpGlobals->time;
		m_bOnAttack = true;
	}
}

void CIchthyosaur::CombatUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !ShouldToggle( useType, m_bOnAttack ) )
		return;

	if (m_bOnAttack)
	{
		m_bOnAttack = false;
	}
	else
	{
		m_bOnAttack = true;
	}
}

bool CIchthyosaur :: CheckRangeAttack1 ( float flDot, float flDist )
{
	if ( flDot > -0.7 && (m_bOnAttack || ( flDist <= 192 && m_idealDist <= 192)))
	{
		return true;
	}

	return false;
}

void CIchthyosaur::UpdateYawSpeed()
{
	SetYawSpeed( 100 );
}



//=========================================================
// Killed - overrides CFlyingMonster.
//
void CIchthyosaur::Killed( const CTakeDamageInfo& info, GibAction gibAction )
{
	CBaseMonster::Killed( info, gibAction );
	SetAbsVelocity( Vector( 0, 0, 0 ) );
}

void CIchthyosaur::BecomeDead( void )
{
	SetTakeDamageMode( DAMAGE_YES );// don't let autoaim aim at corpses.

	// give the corpse half of the monster's original maximum health. 
	SetHealth( GetMaxHealth() / 2 );
	SetMaxHealth( 5 ); // max_health now becomes a counter for how many blood decals the corpse can place.
}

#define ICHTHYOSAUR_AE_SHAKE_RIGHT 1
#define ICHTHYOSAUR_AE_SHAKE_LEFT  2

void CIchthyosaur :: HandleAnimEvent( AnimEvent_t& event )
{
	bool bDidAttack = false;
	switch( event.event )
	{
	case ICHTHYOSAUR_AE_SHAKE_RIGHT:
	case ICHTHYOSAUR_AE_SHAKE_LEFT:
		{
			if (m_hEnemy != NULL && FVisible( m_hEnemy ))
			{
				CBaseEntity *pHurt = m_hEnemy;

				if (m_flEnemyTouched < gpGlobals->time - 0.2 && (m_hEnemy->BodyTarget( GetAbsOrigin() ) - GetAbsOrigin()).Length() > (32+16+32))
					break;

				Vector vecShootDir = ShootAtEnemy( GetAbsOrigin() );
				UTIL_MakeAimVectors ( GetAbsAngles() );

				if (DotProduct( vecShootDir, gpGlobals->v_forward ) > 0.707)
				{
					m_bOnAttack = true;
					Vector vecPunchAngle = pHurt->GetPunchAngle();
					vecPunchAngle.z = -18;
					vecPunchAngle.x = 5;
					pHurt->SetPunchAngle( vecPunchAngle );
					pHurt->SetAbsVelocity( pHurt->GetAbsVelocity() - gpGlobals->v_right * 300 );
					if (pHurt->IsPlayer())
					{
						Vector vecAngles = pHurt->GetAbsAngles();
						vecAngles.x += RANDOM_FLOAT( -35, 35 );
						vecAngles.y += RANDOM_FLOAT( -90, 90 );
						vecAngles.z = 0;
						pHurt->SetAbsAngles( vecAngles );
						pHurt->SetFixAngleMode( FIXANGLE_SET );
					}
					pHurt->TakeDamage( this, this, gSkillData.GetIchthyosaurDmgShake(), DMG_SLASH );
				}
			}
			BiteSound();

			bDidAttack = true;
		}
		break;
	default:
		CFlyingMonster::HandleAnimEvent( event );
		break;
	}

	if (bDidAttack)
	{
		Vector vecSrc = GetAbsOrigin() + gpGlobals->v_forward * 32;
		UTIL_Bubbles( vecSrc - Vector( 8, 8, 8 ), vecSrc + Vector( 8, 8, 8 ), 16 );
	}
}

void CIchthyosaur :: Spawn()
{
	Precache( );

	SetModel( "models/icky.mdl");
	SetSize( Vector( -32, -32, -32 ), Vector( 32, 32, 32 ) );

	SetSolidType( SOLID_BBOX );
	SetMoveType( MOVETYPE_FLY );
	m_bloodColor		= BLOOD_COLOR_GREEN;
	SetHealth( gSkillData.GetIchthyosaurHealth() );
	SetViewOffset( Vector ( 0, 0, 16 ) );
	m_flFieldOfView		= VIEW_FIELD_WIDE;
	m_MonsterState		= MONSTERSTATE_NONE;
	GetFlags() |= FL_SWIM;
	SetFlyingSpeed( ICHTHYOSAUR_SPEED );
	SetFlyingMomentum( 2.5 );	// Set momentum constant

	m_afCapability		= bits_CAP_RANGE_ATTACK1 | bits_CAP_SWIM;

	MonsterInit();

	SetTouch( &CIchthyosaur::BiteTouch );
	SetUse( &CIchthyosaur::CombatUse );

	m_idealDist = 384;
	m_flMinSpeed = 80;
	m_flMaxSpeed = 300;
	m_flMaxDist = 384;

	Vector Forward;
	UTIL_MakeVectorsPrivate( GetAbsAngles(), &Forward, nullptr, nullptr );
	SetAbsVelocity( m_flightSpeed * Forward.Normalize() );
	m_SaveVelocity = GetAbsVelocity();
}

void CIchthyosaur :: Precache()
{
	PRECACHE_MODEL("models/icky.mdl");

	PRECACHE_SOUND_ARRAY( pIdleSounds );
	PRECACHE_SOUND_ARRAY( pAlertSounds );
	PRECACHE_SOUND_ARRAY( pAttackSounds );
	PRECACHE_SOUND_ARRAY( pBiteSounds );
	PRECACHE_SOUND_ARRAY( pDieSounds );
	PRECACHE_SOUND_ARRAY( pPainSounds );
}

Schedule_t* CIchthyosaur::GetSchedule()
{
	// ALERT( at_console, "GetSchedule( )\n" );
	switch(m_MonsterState)
	{
	case MONSTERSTATE_IDLE:
		m_flightSpeed = 80;
		return GetScheduleOfType( SCHED_IDLE_WALK );

	case MONSTERSTATE_ALERT:
		m_flightSpeed = 150;
		return GetScheduleOfType( SCHED_IDLE_WALK );

	case MONSTERSTATE_COMBAT:
		m_flMaxSpeed = 400;
		// eat them
		if ( HasConditions( bits_COND_CAN_MELEE_ATTACK1 ) )
		{
			return GetScheduleOfType( SCHED_MELEE_ATTACK1 );
		}
		// chase them down and eat them
		if ( HasConditions( bits_COND_CAN_RANGE_ATTACK1 ) )
		{
			return GetScheduleOfType( SCHED_CHASE_ENEMY );
		}
		if ( HasConditions( bits_COND_HEAVY_DAMAGE ) )
		{
			m_bOnAttack = true;
		}
		if ( GetHealth() < GetMaxHealth() - 20 )
		{
			m_bOnAttack = true;
		}

		return GetScheduleOfType( SCHED_STANDOFF );

	default: break;
	}

	return CFlyingMonster :: GetSchedule();
}


//=========================================================
//=========================================================
Schedule_t* CIchthyosaur :: GetScheduleOfType ( int Type ) 
{
	// ALERT( at_console, "GetScheduleOfType( %d ) %d\n", Type, m_bOnAttack );
	switch	( Type )
	{
	case SCHED_IDLE_WALK:
		return slSwimAround;
	case SCHED_STANDOFF:
		return slCircleEnemy;
	case SCHED_FAIL:
		return slSwimAgitated;
	case SCHED_DIE:
		return slTwitchDie;
	case SCHED_CHASE_ENEMY:
		AttackSound( );
	}

	return CBaseMonster :: GetScheduleOfType( Type );
}

void CIchthyosaur::StartTask( const Task_t& task )
{
	switch (task.iTask)
	{
	case TASK_ICHTHYOSAUR_CIRCLE_ENEMY:
		break;
	case TASK_ICHTHYOSAUR_SWIM:
		break;
	case TASK_SMALL_FLINCH:
		if (m_idealDist > 128)
		{
			m_flMaxDist = 512;
			m_idealDist = 512;
		}
		else
		{
			m_bOnAttack = true;
		}
		CFlyingMonster::StartTask( task );
		break;

	case TASK_ICHTHYOSAUR_FLOAT:
		SetSkin( EYE_BASE );
		SetSequenceByName( "bellyup" );
		break;

	default:
		CFlyingMonster::StartTask( task );
		break;
	}
}

void CIchthyosaur :: RunTask ( const Task_t& task )
{
	switch ( task.iTask )
	{
	case TASK_ICHTHYOSAUR_CIRCLE_ENEMY:
		if (m_hEnemy == NULL)
		{
			TaskComplete( );
		}
		else if (FVisible( m_hEnemy ))
		{
			Vector vecFrom = m_hEnemy->EyePosition( );

			Vector vecDelta = (GetAbsOrigin() - vecFrom).Normalize( );
			Vector vecSwim = CrossProduct( vecDelta, Vector( 0, 0, 1 ) ).Normalize( );
			
			if (DotProduct( vecSwim, m_SaveVelocity ) < 0)
				vecSwim = vecSwim * -1.0;

			Vector vecPos = vecFrom + vecDelta * m_idealDist + vecSwim * 32;

			// ALERT( at_console, "vecPos %.0f %.0f %.0f\n", vecPos.x, vecPos.y, vecPos.z );

			TraceResult tr;
		
			UTIL_TraceHull( vecFrom, vecPos, ignore_monsters, Hull::LARGE, m_hEnemy->edict(), &tr );

			if (tr.flFraction > 0.5)
				vecPos = tr.vecEndPos;

			m_SaveVelocity = m_SaveVelocity * 0.8 + 0.2 * (vecPos - GetAbsOrigin()).Normalize() * m_flightSpeed;

			// ALERT( at_console, "m_SaveVelocity %.2f %.2f %.2f\n", m_SaveVelocity.x, m_SaveVelocity.y, m_SaveVelocity.z );

			if (HasConditions( bits_COND_ENEMY_FACING_ME ) && m_hEnemy->FVisible( this ))
			{
				m_flNextAlert -= 0.1;

				if (m_idealDist < m_flMaxDist)
				{
					m_idealDist += 4;
				}
				if (m_flightSpeed > m_flMinSpeed)
				{
					m_flightSpeed -= 2;
				}
				else if (m_flightSpeed < m_flMinSpeed)
				{
					m_flightSpeed += 2;
				}
				if (m_flMinSpeed < m_flMaxSpeed)
				{
					m_flMinSpeed += 0.5;
				}
			}
			else 
			{
				m_flNextAlert += 0.1;

				if (m_idealDist > 128)
				{
					m_idealDist -= 4;
				}
				if (m_flightSpeed < m_flMaxSpeed)
				{
					m_flightSpeed += 4;
				}
			}
			// ALERT( at_console, "%.0f\n", m_idealDist );
		}
		else
		{
			m_flNextAlert = gpGlobals->time + 0.2;
		}

		if (m_flNextAlert < gpGlobals->time)
		{
			// ALERT( at_console, "AlertSound()\n");
			AlertSound( );
			m_flNextAlert = gpGlobals->time + RANDOM_FLOAT( 3, 5 );
		}

		break;
	case TASK_ICHTHYOSAUR_SWIM:
		if (m_fSequenceFinished)
		{
			TaskComplete( );
		}
		break;
	case TASK_DIE:
		if ( m_fSequenceFinished )
		{
			SetDeadFlag( DEAD_DEAD );

			TaskComplete( );
		}
		break;

	case TASK_ICHTHYOSAUR_FLOAT:
		{
			Vector vecAngles = GetAbsAngles();
			vecAngles.x = UTIL_ApproachAngle( 0, vecAngles.x, 20 );
			SetAbsAngles( vecAngles );
			SetAbsVelocity( GetAbsVelocity() * 0.8 );
			if ( GetWaterLevel() > WATERLEVEL_FEET && GetAbsVelocity().z < 64)
			{
				Vector vecVelocity = GetAbsVelocity();
				vecVelocity.z += 8;
				SetAbsVelocity( vecVelocity );
			}
			else 
			{
				Vector vecVelocity = GetAbsVelocity();
				vecVelocity.z -= 8;
				SetAbsVelocity( vecVelocity );
			}
			// ALERT( at_console, "%f\n", GetAbsVelocity().z );
			break;
		}

	default: 
		CFlyingMonster :: RunTask ( task );
		break;
	}
}



float CIchthyosaur::VectorToPitch( const Vector &vec )
{
	float pitch;
	if (vec.z == 0 && vec.x == 0)
		pitch = 0;
	else
	{
		pitch = (int) (atan2(vec.z, sqrt(vec.x*vec.x+vec.y*vec.y)) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	return pitch;
}

//=========================================================
void CIchthyosaur::Move(float flInterval)
{
	CFlyingMonster::Move( flInterval );
}

float CIchthyosaur::FlPitchDiff( void )
{
	float	flPitchDiff;
	float	flCurrentPitch;

	flCurrentPitch = UTIL_AngleMod( GetAbsAngles().z );

	if ( flCurrentPitch == GetIdealPitch() )
	{
		return 0;
	}

	flPitchDiff = GetIdealPitch() - flCurrentPitch;

	if ( GetIdealPitch() > flCurrentPitch )
	{
		if (flPitchDiff >= 180)
			flPitchDiff = flPitchDiff - 360;
	}
	else 
	{
		if (flPitchDiff <= -180)
			flPitchDiff = flPitchDiff + 360;
	}
	return flPitchDiff;
}

float CIchthyosaur :: ChangePitch( int speed )
{
	if ( GetMoveType() == MOVETYPE_FLY )
	{
		float diff = FlPitchDiff();
		float target = 0;
		if ( m_IdealActivity != GetStoppedActivity() )
		{
			if (diff < -20)
				target = 45;
			else if (diff > 20)
				target = -45;
		}
		Vector vecAngles = GetAbsAngles();
		vecAngles.x = UTIL_Approach(target, vecAngles.x, 220.0 * 0.1 );
		SetAbsAngles( vecAngles );
	}
	return 0;
}

float CIchthyosaur::ChangeYaw( int speed )
{
	if ( GetMoveType() == MOVETYPE_FLY )
	{
		float diff = FlYawDiff();
		float target = 0;

		if ( m_IdealActivity != GetStoppedActivity() )
		{
			if ( diff < -20 )
				target = 20;
			else if ( diff > 20 )
				target = -20;
		}
		Vector vecAngles = GetAbsAngles();
		vecAngles.z = UTIL_Approach( target, vecAngles.z, 220.0 * 0.1 );
		SetAbsAngles( vecAngles );
	}
	return CFlyingMonster::ChangeYaw( speed );
}


Activity CIchthyosaur:: GetStoppedActivity( void )
{ 
	if ( GetMoveType() != MOVETYPE_FLY )		// UNDONE: Ground idle here, IDLE may be something else
		return ACT_IDLE;
	return ACT_WALK;
}

void CIchthyosaur::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
{
	m_SaveVelocity = vecDir * m_flightSpeed;
}


void CIchthyosaur::MonsterThink ( void )
{
	CFlyingMonster::MonsterThink( );

	if ( GetDeadFlag() == DEAD_NO)
	{
		if (m_MonsterState != MONSTERSTATE_SCRIPT)
		{
			Swim( );

			// blink the eye
			if (m_flBlink < gpGlobals->time)
			{
				SetSkin( EYE_CLOSED );
				if (m_flBlink + 0.2 < gpGlobals->time)
				{
					m_flBlink = gpGlobals->time + RANDOM_FLOAT( 3, 4 );
					if (m_bOnAttack)
						SetSkin( EYE_MAD );
					else
						SetSkin( EYE_BASE );
				}
			}
		}
	}
}

void CIchthyosaur :: Stop( void ) 
{
	if (!m_bOnAttack)
		m_flightSpeed = 80.0;
}

void CIchthyosaur::Swim( )
{
	Vector start = GetAbsOrigin();

	Vector Angles;
	Vector Forward, Right, Up;

	if( GetFlags().Any( FL_ONGROUND ) )
	{
		Vector vecAngles = GetAbsAngles();
		vecAngles.x = 0;
		vecAngles.y += RANDOM_FLOAT( -45, 45 );
		SetAbsAngles( vecAngles );
		GetFlags().ClearFlags( FL_ONGROUND );

		Angles = Vector( -GetAbsAngles().x, GetAbsAngles().y, GetAbsAngles().z );
		UTIL_MakeVectorsPrivate(Angles, &Forward, &Right, &Up);

		SetAbsVelocity( Forward * 200 + Up * 200 );

		return;
	}

	if (m_bOnAttack && m_flightSpeed < m_flMaxSpeed)
	{
		m_flightSpeed += 40;
	}
	if (m_flightSpeed < 180)
	{
		if (m_IdealActivity == ACT_RUN)
			SetActivity( ACT_WALK );
		if (m_IdealActivity == ACT_WALK)
			SetFrameRate( m_flightSpeed / 150.0 );
		// ALERT( at_console, "walk %.2f\n", GetFrameRate() );
	}
	else
	{
		if (m_IdealActivity == ACT_WALK)
			SetActivity( ACT_RUN );
		if (m_IdealActivity == ACT_RUN)
			SetFrameRate( m_flightSpeed / 150.0 );
		// ALERT( at_console, "run  %.2f\n", GetFrameRate() );
	}

/*
	if (!m_pBeam)
	{
		m_pBeam = CBeam::BeamCreate( "sprites/laserbeam.spr", 80 );
		m_pBeam->PointEntInit( GetAbsOrigin() + m_SaveVelocity, entindex( ) );
		m_pBeam->SetEndAttachment( 1 );
		m_pBeam->SetColor( 255, 180, 96 );
		m_pBeam->SetBrightness( 192 );
	}
*/
#define PROBE_LENGTH 150
	Angles = UTIL_VecToAngles( m_SaveVelocity );
	Angles.x = -Angles.x;
	UTIL_MakeVectorsPrivate(Angles, &Forward, &Right, &Up);

	Vector f, u, l, r, d;
	f = DoProbe(start + PROBE_LENGTH   * Forward);
	r = DoProbe(start + PROBE_LENGTH/3 * Forward+Right);
	l = DoProbe(start + PROBE_LENGTH/3 * Forward-Right);
	u = DoProbe(start + PROBE_LENGTH/3 * Forward+Up);
	d = DoProbe(start + PROBE_LENGTH/3 * Forward-Up);

	Vector SteeringVector = f+r+l+u+d;
	m_SaveVelocity = (m_SaveVelocity + SteeringVector/2).Normalize();

	Angles = Vector( -GetAbsAngles().x, GetAbsAngles().y, GetAbsAngles().z );
	UTIL_MakeVectorsPrivate(Angles, &Forward, &Right, &Up);
	// ALERT( at_console, "%f : %f\n", Angles.x, Forward.z );

	float flDot = DotProduct( Forward, m_SaveVelocity );
	if (flDot > 0.5)
		m_SaveVelocity = m_SaveVelocity * m_flightSpeed;
	else if (flDot > 0)
		m_SaveVelocity = m_SaveVelocity * m_flightSpeed * (flDot + 0.5);
	else
		m_SaveVelocity = m_SaveVelocity * 80;

	SetAbsVelocity( m_SaveVelocity );

	// ALERT( at_console, "%.0f %.0f\n", m_flightSpeed, GetAbsVelocity().Length() );


	// ALERT( at_console, "Steer %f %f %f\n", SteeringVector.x, SteeringVector.y, SteeringVector.z );

/*
	m_pBeam->SetStartPos( GetAbsOrigin() + GetAbsVelocity() );
	m_pBeam->RelinkBeam( );
*/

	// ALERT( at_console, "speed %f\n", m_flightSpeed );
	
	Angles = UTIL_VecToAngles( m_SaveVelocity );

	// Smooth Pitch
	//
	if (Angles.x > 180)
		Angles.x = Angles.x - 360;
	{
		Vector vecAngles = GetAbsAngles();
		vecAngles.x = UTIL_Approach(Angles.x, vecAngles.x, 50 * 0.1 );
		if (vecAngles.x < -80) vecAngles.x = -80;
		if (vecAngles.x >  80) vecAngles.x =  80;
		SetAbsAngles( vecAngles );
	}

	// Smooth Yaw and generate Roll
	//
	float turn = 360;
	// ALERT( at_console, "Y %.0f %.0f\n", Angles.y, GetAbsAngles().y );

	if (fabs(Angles.y - GetAbsAngles().y) < fabs(turn))
	{
		turn = Angles.y - GetAbsAngles().y;
	}
	if (fabs(Angles.y - GetAbsAngles().y + 360) < fabs(turn))
	{
		turn = Angles.y - GetAbsAngles().y + 360;
	}
	if (fabs(Angles.y - GetAbsAngles().y - 360) < fabs(turn))
	{
		turn = Angles.y - GetAbsAngles().y - 360;
	}

	float speed = m_flightSpeed * 0.1;

	// ALERT( at_console, "speed %.0f %f\n", turn, speed );
	if (fabs(turn) > speed)
	{
		if (turn < 0.0)
		{
			turn = -speed;
		}
		else
		{
			turn = speed;
		}
	}
	{
		Vector vecAngles = GetAbsAngles();
		vecAngles.y += turn;
		vecAngles.z -= turn;
		vecAngles.y = fmod(( vecAngles.y + 360.0), 360.0);
		SetAbsAngles( vecAngles );
	}

	static float yaw_adj;

	yaw_adj = yaw_adj * 0.8 + turn;

	// ALERT( at_console, "yaw %f : %f\n", turn, yaw_adj );

	SetBoneController( 0, -yaw_adj / 4.0 );

	// Roll Smoothing
	//
	turn = 360;
	if (fabs(Angles.z - GetAbsAngles().z) < fabs(turn))
	{
		turn = Angles.z - GetAbsAngles().z;
	}
	if (fabs(Angles.z - GetAbsAngles().z + 360) < fabs(turn))
	{
		turn = Angles.z - GetAbsAngles().z + 360;
	}
	if (fabs(Angles.z - GetAbsAngles().z - 360) < fabs(turn))
	{
		turn = Angles.z - GetAbsAngles().z - 360;
	}
	speed = m_flightSpeed/2 * 0.1;

	Vector vecAngles = GetAbsAngles();

	if (fabs(turn) < speed)
	{
		vecAngles.z += turn;
	}
	else
	{
		if (turn < 0.0)
		{
			vecAngles.z -= speed;
		}
		else
		{
			vecAngles.z += speed;
		}
	}
	if (vecAngles.z < -20) vecAngles.z = -20;
	if (vecAngles.z >  20) vecAngles.z =  20;

	SetAbsAngles( vecAngles );

	UTIL_MakeVectorsPrivate( Vector( -Angles.x, Angles.y, Angles.z ), &Forward, &Right, &Up);

	// UTIL_MoveToOrigin ( this, GetAbsOrigin() + Forward * speed, speed, MOVE_STRAFE );
}


Vector CIchthyosaur::DoProbe(const Vector &Probe)
{
	Vector WallNormal = Vector(0,0,-1); // WATER normal is Straight Down for fish.
	float frac;
	bool bBumpedSomething = ProbeZ(GetAbsOrigin(), Probe, &frac);

	TraceResult tr;
	TRACE_MONSTER_HULL(edict(), GetAbsOrigin(), Probe, dont_ignore_monsters, edict(), &tr);
	if ( tr.fAllSolid || tr.flFraction < 0.99 )
	{
		if (tr.flFraction < 0.0) tr.flFraction = 0.0;
		if (tr.flFraction > 1.0) tr.flFraction = 1.0;
		if (tr.flFraction < frac)
		{
			frac = tr.flFraction;
			bBumpedSomething = true;
			WallNormal = tr.vecPlaneNormal;
		}
	}

	if (bBumpedSomething && (m_hEnemy == NULL || tr.pHit != m_hEnemy->edict()))
	{
		Vector ProbeDir = Probe - GetAbsOrigin();

		Vector NormalToProbeAndWallNormal = CrossProduct(ProbeDir, WallNormal);
		Vector SteeringVector = CrossProduct( NormalToProbeAndWallNormal, ProbeDir);

		float SteeringForce = m_flightSpeed * (1-frac) * (DotProduct(WallNormal.Normalize(), m_SaveVelocity.Normalize()));
		if (SteeringForce < 0.0)
		{
			SteeringForce = -SteeringForce;
		}
		SteeringVector = SteeringForce * SteeringVector.Normalize();
		
		return SteeringVector;
	}
	return Vector(0, 0, 0);
}