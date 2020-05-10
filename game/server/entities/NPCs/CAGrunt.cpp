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
// Agrunt - Dominant, warlike alien grunt monster
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Monsters.h"
#include "Schedule.h"
#include "CSquadMonster.h"
#include "Weapons.h"
#include "entities/CSoundEnt.h"
#include "CHornet.h"

#include "CAGrunt.h"

const char *CAGrunt::pAttackHitSounds[] =
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CAGrunt::pAttackMissSounds[] =
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *CAGrunt::pAttackSounds[] =
{
	"agrunt/ag_attack1.wav",
	"agrunt/ag_attack2.wav",
	"agrunt/ag_attack3.wav",
};

const char *CAGrunt::pDieSounds[] =
{
	"agrunt/ag_die1.wav",
	"agrunt/ag_die4.wav",
	"agrunt/ag_die5.wav",
};

const char *CAGrunt::pPainSounds[] =
{
	"agrunt/ag_pain1.wav",
	"agrunt/ag_pain2.wav",
	"agrunt/ag_pain3.wav",
	"agrunt/ag_pain4.wav",
	"agrunt/ag_pain5.wav",
};

const char *CAGrunt::pIdleSounds[] =
{
	"agrunt/ag_idle1.wav",
	"agrunt/ag_idle2.wav",
	"agrunt/ag_idle3.wav",
	"agrunt/ag_idle4.wav",
};

const char *CAGrunt::pAlertSounds[] =
{
	"agrunt/ag_alert1.wav",
	"agrunt/ag_alert3.wav",
	"agrunt/ag_alert4.wav",
	"agrunt/ag_alert5.wav",
};

int iAgruntMuzzleFlash;

BEGIN_DATADESC(	CAGrunt )
	DEFINE_FIELD( m_fCanHornetAttack, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flNextHornetAttackCheck, FIELD_TIME ),
	DEFINE_FIELD( m_flNextPainTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextSpeakTime, FIELD_TIME ),
	DEFINE_FIELD( m_flNextWordTime, FIELD_TIME ),
	DEFINE_FIELD( m_iLastWord, FIELD_INTEGER ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( monster_alien_grunt, CAGrunt );

Relationship CAGrunt::IRelationship ( CBaseEntity *pTarget )
{
	if ( pTarget->ClassnameIs( "monster_human_grunt" ) )
	{
		return R_NM;
	}

	return CSquadMonster :: IRelationship( pTarget );
}

int CAGrunt :: ISoundMask ( void )
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_PLAYER	|
			bits_SOUND_DANGER;
}

void CAGrunt :: TraceAttack( const CTakeDamageInfo& info, Vector vecDir, TraceResult& tr )
{
	CTakeDamageInfo newInfo = info;

	if ( tr.iHitgroup == 10 && (newInfo.GetDamageTypes() & (DMG_BULLET | DMG_SLASH | DMG_CLUB)))
	{
		// hit armor
		if ( GetDamageTime() != gpGlobals->time || (RANDOM_LONG(0,10) < 1) )
		{
			UTIL_Ricochet( tr.vecEndPos, RANDOM_FLOAT( 1, 2) );
			SetDamageTime( gpGlobals->time );
		}

		if ( RANDOM_LONG( 0, 1 ) == 0 )
		{
			Vector vecTracerDir = vecDir;

			vecTracerDir.x += RANDOM_FLOAT( -0.3, 0.3 );
			vecTracerDir.y += RANDOM_FLOAT( -0.3, 0.3 );
			vecTracerDir.z += RANDOM_FLOAT( -0.3, 0.3 );

			vecTracerDir = vecTracerDir * -512;

			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, tr.vecEndPos );
			WRITE_BYTE( TE_TRACER );
				WRITE_COORD( tr.vecEndPos.x );
				WRITE_COORD( tr.vecEndPos.y );
				WRITE_COORD( tr.vecEndPos.z );

				WRITE_COORD( vecTracerDir.x );
				WRITE_COORD( vecTracerDir.y );
				WRITE_COORD( vecTracerDir.z );
			MESSAGE_END();
		}

		newInfo.GetMutableDamage() -= 20;
		if (newInfo.GetDamage() <= 0)
			newInfo.GetMutableDamage() = 0.1;// don't hurt the monster much, but allow bits_COND_LIGHT_DAMAGE to be generated
	}
	else
	{
		SpawnBlood( tr.vecEndPos, BloodColor(), newInfo.GetDamage());// a little surface blood.
		TraceBleed( newInfo, vecDir, tr );
	}

	g_MultiDamage.AddMultiDamage( info, this );
}

//=========================================================
// StopTalking - won't speak again for 10-20 seconds.
//=========================================================
void CAGrunt::StopTalking( void )
{
	m_flNextWordTime = m_flNextSpeakTime = gpGlobals->time + 10 + RANDOM_LONG(0, 10);
}

//=========================================================
// ShouldSpeak - Should this agrunt be talking?
//=========================================================
bool CAGrunt::ShouldSpeak()
{
	if ( m_flNextSpeakTime > gpGlobals->time )
	{
		// my time to talk is still in the future.
		return false;
	}

	if ( GetSpawnFlags().Any( SF_MONSTER_GAG ) )
	{
		if ( m_MonsterState != MONSTERSTATE_COMBAT )
		{
			// if gagged, don't talk outside of combat.
			// if not going to talk because of this, put the talk time 
			// into the future a bit, so we don't talk immediately after 
			// going into combat
			m_flNextSpeakTime = gpGlobals->time + 3;
			return false;
		}
	}

	return true;
}

void CAGrunt :: PrescheduleThink ( void )
{
	if ( ShouldSpeak() )
	{
		if ( m_flNextWordTime < gpGlobals->time )
		{
			int num = -1;

			do
			{
				num = RANDOM_LONG(0,ARRAYSIZE(pIdleSounds)-1);
			} while( num == m_iLastWord );

			m_iLastWord = num;

			// play a new sound
			EMIT_SOUND ( this, CHAN_VOICE, pIdleSounds[ num ], 1.0, ATTN_NORM );

			// is this word our last?
			if ( RANDOM_LONG( 1, 10 ) <= 1 )
			{
				// stop talking.
				StopTalking();
			}
			else
			{
				m_flNextWordTime = gpGlobals->time + RANDOM_FLOAT( 0.5, 1 );
			}
		}
	}
}

void CAGrunt :: DeathSound ( void )
{
	StopTalking();

	EMIT_SOUND ( this, CHAN_VOICE, pDieSounds[RANDOM_LONG(0,ARRAYSIZE(pDieSounds)-1)], 1.0, ATTN_NORM );
}

void CAGrunt :: AlertSound ( void )
{
	StopTalking();

	EMIT_SOUND ( this, CHAN_VOICE, pAlertSounds[RANDOM_LONG(0,ARRAYSIZE(pAlertSounds)-1)], 1.0, ATTN_NORM );
}

void CAGrunt :: AttackSound ( void )
{
	StopTalking();

	EMIT_SOUND ( this, CHAN_VOICE, pAttackSounds[RANDOM_LONG(0,ARRAYSIZE(pAttackSounds)-1)], 1.0, ATTN_NORM );
}

void CAGrunt :: PainSound ( void )
{
	if ( m_flNextPainTime > gpGlobals->time )
	{
		return;
	}

	m_flNextPainTime = gpGlobals->time + 0.6;

	StopTalking();

	EMIT_SOUND ( this, CHAN_VOICE, pPainSounds[RANDOM_LONG(0,ARRAYSIZE(pPainSounds)-1)], 1.0, ATTN_NORM );
}

EntityClassification_t CAGrunt::GetClassification()
{
	return EntityClassifications().GetClassificationId( classify::ALIEN_MILITARY );
}

void CAGrunt::UpdateYawSpeed()
{
	int ys;

	switch ( m_Activity )
	{
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 110;
		break;
	default:			ys = 100;
	}

	SetYawSpeed( ys );
}

void CAGrunt :: HandleAnimEvent( AnimEvent_t& event )
{
	switch( event.event )
	{
	case AGRUNT_AE_HORNET1:
	case AGRUNT_AE_HORNET2:
	case AGRUNT_AE_HORNET3:
	case AGRUNT_AE_HORNET4:
	case AGRUNT_AE_HORNET5:
		{
			// m_vecEnemyLKP should be center of enemy body
			Vector vecArmPos, vecArmDir;
			Vector vecDirToEnemy;
			Vector angDir;

			if (HasConditions( bits_COND_SEE_ENEMY))
			{
				vecDirToEnemy = ( ( m_vecEnemyLKP ) - GetAbsOrigin() );
				angDir = UTIL_VecToAngles( vecDirToEnemy );
				vecDirToEnemy = vecDirToEnemy.Normalize();
			}
			else
			{
				angDir = GetAbsAngles();
				UTIL_MakeAimVectors( angDir );
				vecDirToEnemy = gpGlobals->v_forward;
			}

			GetEffects() = EF_MUZZLEFLASH;

			// make angles +-180
			if (angDir.x > 180)
			{
				angDir.x = angDir.x - 360;
			}

			SetBlending( 0, angDir.x );
			GetAttachment( 0, vecArmPos, vecArmDir );

			vecArmPos = vecArmPos + vecDirToEnemy * 32;
			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, vecArmPos );
				WRITE_BYTE( TE_SPRITE );
				WRITE_COORD( vecArmPos.x );	// pos
				WRITE_COORD( vecArmPos.y );	
				WRITE_COORD( vecArmPos.z );	
				WRITE_SHORT( iAgruntMuzzleFlash );		// model
				WRITE_BYTE( 6 );				// size * 10
				WRITE_BYTE( 128 );			// brightness
			MESSAGE_END();

			CBaseEntity *pHornet = CBaseEntity::Create( "hornet", vecArmPos, UTIL_VecToAngles( vecDirToEnemy ), edict() );
			UTIL_MakeVectors ( pHornet->GetAbsAngles() );
			pHornet->SetAbsVelocity( gpGlobals->v_forward * 300 );
			
			
			
			switch ( RANDOM_LONG ( 0 , 2 ) )
			{
				case 0:	EMIT_SOUND_DYN ( this, CHAN_WEAPON, "agrunt/ag_fire1.wav", 1.0, ATTN_NORM, 0, 100 );	break;
				case 1:	EMIT_SOUND_DYN ( this, CHAN_WEAPON, "agrunt/ag_fire2.wav", 1.0, ATTN_NORM, 0, 100 );	break;
				case 2:	EMIT_SOUND_DYN ( this, CHAN_WEAPON, "agrunt/ag_fire3.wav", 1.0, ATTN_NORM, 0, 100 );	break;
			}

			CBaseMonster *pHornetMonster = pHornet->MyMonsterPointer();

			if ( pHornetMonster )
			{
				pHornetMonster->m_hEnemy = m_hEnemy;
			}
		}
		break;

	case AGRUNT_AE_LEFT_FOOT:
		switch (RANDOM_LONG(0,1))
		{
		// left foot
		case 0:	EMIT_SOUND_DYN ( this, CHAN_BODY, "player/pl_ladder2.wav", 1, ATTN_NORM, 0, 70 );	break;
		case 1:	EMIT_SOUND_DYN ( this, CHAN_BODY, "player/pl_ladder4.wav", 1, ATTN_NORM, 0, 70 );	break;
		}
		break;
	case AGRUNT_AE_RIGHT_FOOT:
		// right foot
		switch (RANDOM_LONG(0,1))
		{
		case 0:	EMIT_SOUND_DYN ( this, CHAN_BODY, "player/pl_ladder1.wav", 1, ATTN_NORM, 0, 70 );	break;
		case 1:	EMIT_SOUND_DYN ( this, CHAN_BODY, "player/pl_ladder3.wav", 1, ATTN_NORM, 0 ,70);	break;
		}
		break;

	case AGRUNT_AE_LEFT_PUNCH:
		{
			CBaseEntity *pHurt = CheckTraceHullAttack( AGRUNT_MELEE_DIST, gSkillData.GetAGruntDmgPunch(), DMG_CLUB );
			
			if ( pHurt )
			{
				Vector vecPunchAngle = pHurt->GetPunchAngle();
				vecPunchAngle.y = -25;
				vecPunchAngle.x = 8;
				pHurt->SetPunchAngle( vecPunchAngle );

				// OK to use gpGlobals without calling MakeVectors, cause CheckTraceHullAttack called it above.
				if ( pHurt->IsPlayer() )
				{
					// this is a player. Knock him around.
					pHurt->SetAbsVelocity( pHurt->GetAbsVelocity() + gpGlobals->v_right * 250 );
				}

				EMIT_SOUND_DYN ( this, CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );

				Vector vecArmPos, vecArmAng;
				GetAttachment( 0, vecArmPos, vecArmAng );
				SpawnBlood(vecArmPos, pHurt->BloodColor(), 25);// a little surface blood.
			}
			else
			{
				// Play a random attack miss sound
				EMIT_SOUND_DYN ( this, CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
			}
		}
		break;

	case AGRUNT_AE_RIGHT_PUNCH:
		{
			CBaseEntity *pHurt = CheckTraceHullAttack( AGRUNT_MELEE_DIST, gSkillData.GetAGruntDmgPunch(), DMG_CLUB );

			if ( pHurt )
			{
				Vector vecPunchAngle = pHurt->GetPunchAngle();
				vecPunchAngle.y = 25;
				vecPunchAngle.x = 8;
				pHurt->SetPunchAngle( vecPunchAngle );

				// OK to use gpGlobals without calling MakeVectors, cause CheckTraceHullAttack called it above.
				if ( pHurt->IsPlayer() )
				{
					// this is a player. Knock him around.
					pHurt->SetAbsVelocity( pHurt->GetAbsVelocity() + gpGlobals->v_right * -250 );
				}

				EMIT_SOUND_DYN ( this, CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );

				Vector vecArmPos, vecArmAng;
				GetAttachment( 0, vecArmPos, vecArmAng );
				SpawnBlood(vecArmPos, pHurt->BloodColor(), 25);// a little surface blood.
			}
			else
			{
				// Play a random attack miss sound
				EMIT_SOUND_DYN ( this, CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
			}
		}
		break;

	default:
		CSquadMonster::HandleAnimEvent( event );
		break;
	}
}

void CAGrunt :: Spawn()
{
	Precache( );

	SetModel( "models/agrunt.mdl");
	SetSize( Vector(-32, -32, 0), Vector(32, 32, 64));

	SetSolidType( SOLID_SLIDEBOX );
	SetMoveType( MOVETYPE_STEP );
	m_bloodColor		= BLOOD_COLOR_GREEN;
	GetEffects().ClearAll();
	SetHealth( gSkillData.GetAGruntHealth() );
	m_flFieldOfView		= 0.2;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;
	m_afCapability		= 0;
	m_afCapability		|= bits_CAP_SQUAD;

	m_HackedGunPos		= Vector( 24, 64, 48 );

	m_flNextSpeakTime	= m_flNextWordTime = gpGlobals->time + 10 + RANDOM_LONG(0, 10);


	MonsterInit();
}

void CAGrunt :: Precache()
{
	size_t i;

	PRECACHE_MODEL("models/agrunt.mdl");

	for ( i = 0; i < ARRAYSIZE( pAttackHitSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackHitSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackMissSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackMissSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pIdleSounds ); i++ )
		PRECACHE_SOUND((char *)pIdleSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pDieSounds ); i++ )
		PRECACHE_SOUND((char *)pDieSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pPainSounds ); i++ )
		PRECACHE_SOUND((char *)pPainSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAlertSounds ); i++ )
		PRECACHE_SOUND((char *)pAlertSounds[i]);


	PRECACHE_SOUND( "hassault/hw_shoot1.wav" );

	iAgruntMuzzleFlash = PRECACHE_MODEL( "sprites/muz4.spr" );

	UTIL_PrecacheOther( "hornet" );
}	
	
//=========================================================
// AI Schedules Specific to this monster
//=========================================================

//=========================================================
// Fail Schedule
//=========================================================
Task_t	tlAGruntFail[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slAGruntFail[] =
{
	{
		tlAGruntFail,
		ARRAYSIZE ( tlAGruntFail ),
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1,
		0,
		"AGrunt Fail"
	},
};

//=========================================================
// Combat Fail Schedule
//=========================================================
Task_t	tlAGruntCombatFail[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT_FACE_ENEMY,		(float)2		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slAGruntCombatFail[] =
{
	{
		tlAGruntCombatFail,
		ARRAYSIZE ( tlAGruntCombatFail ),
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1,
		0,
		"AGrunt Combat Fail"
	},
};

//=========================================================
// Standoff schedule. Used in combat when a monster is 
// hiding in cover or the enemy has moved out of sight. 
// Should we look around in this schedule?
//=========================================================
Task_t	tlAGruntStandoff[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE				},
	{ TASK_WAIT_FACE_ENEMY,			(float)2					},
};

Schedule_t slAGruntStandoff[] = 
{
	{
		tlAGruntStandoff,
		ARRAYSIZE ( tlAGruntStandoff ),
		bits_COND_CAN_RANGE_ATTACK1		|
		bits_COND_CAN_MELEE_ATTACK1		|
		bits_COND_SEE_ENEMY				|
		bits_COND_NEW_ENEMY				|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Agrunt Standoff"
	}
};

//=========================================================
// Suppress
//=========================================================
Task_t	tlAGruntSuppressHornet[] =
{
	{ TASK_STOP_MOVING,		(float)0		},
	{ TASK_RANGE_ATTACK1,	(float)0		},
};

Schedule_t slAGruntSuppress[] =
{
	{
		tlAGruntSuppressHornet,
		ARRAYSIZE ( tlAGruntSuppressHornet ),
		0,
		0,
		"AGrunt Suppress Hornet",
	},
};

//=========================================================
// primary range attacks
//=========================================================
Task_t	tlAGruntRangeAttack1[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
};

Schedule_t	slAGruntRangeAttack1[] =
{
	{ 
		tlAGruntRangeAttack1,
		ARRAYSIZE ( tlAGruntRangeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE,
		
		0,
		"AGrunt Range Attack1"
	},
};


Task_t	tlAGruntHiddenRangeAttack1[] =
{
	{ TASK_SET_FAIL_SCHEDULE,			(float)SCHED_STANDOFF },
	{ TASK_AGRUNT_SETUP_HIDE_ATTACK,	0				},
	{ TASK_STOP_MOVING,					0				},
	{ TASK_FACE_IDEAL,					0				},
	{ TASK_RANGE_ATTACK1_NOTURN,		(float)0		},
};

Schedule_t	slAGruntHiddenRangeAttack[] =
{
	{ 
		tlAGruntHiddenRangeAttack1,
		ARRAYSIZE ( tlAGruntHiddenRangeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AGrunt Hidden Range Attack1"
	},
};

//=========================================================
// Take cover from enemy! Tries lateral cover before node 
// cover! 
//=========================================================
Task_t	tlAGruntTakeCoverFromEnemy[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
	{ TASK_FACE_ENEMY,				(float)0					},
};

Schedule_t	slAGruntTakeCoverFromEnemy[] =
{
	{ 
		tlAGruntTakeCoverFromEnemy,
		ARRAYSIZE ( tlAGruntTakeCoverFromEnemy ), 
		bits_COND_NEW_ENEMY,
		0,
		"AGruntTakeCoverFromEnemy"
	},
};

//=========================================================
// Victory dance!
//=========================================================
Task_t	tlAGruntVictoryDance[] =
{
	{ TASK_STOP_MOVING,						(float)0					},
	{ TASK_SET_FAIL_SCHEDULE,				(float)SCHED_AGRUNT_THREAT_DISPLAY	},
	{ TASK_WAIT,							(float)0.2					},
	{ TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE,	(float)0					},
	{ TASK_WALK_PATH,						(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,				(float)0					},
	{ TASK_FACE_ENEMY,						(float)0					},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_CROUCH			},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_STAND			},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_THREAT_DISPLAY	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_CROUCH			},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_VICTORY_DANCE	},
	{ TASK_PLAY_SEQUENCE,					(float)ACT_STAND			},
};

Schedule_t	slAGruntVictoryDance[] =
{
	{ 
		tlAGruntVictoryDance,
		ARRAYSIZE ( tlAGruntVictoryDance ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE,
		0,
		"AGruntVictoryDance"
	},
};

//=========================================================
//=========================================================
Task_t	tlAGruntThreatDisplay[] =
{
	{ TASK_STOP_MOVING,			(float)0					},
	{ TASK_FACE_ENEMY,			(float)0					},
	{ TASK_PLAY_SEQUENCE,		(float)ACT_THREAT_DISPLAY	},
};

Schedule_t	slAGruntThreatDisplay[] =
{
	{ 
		tlAGruntThreatDisplay,
		ARRAYSIZE ( tlAGruntThreatDisplay ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE,
		
		bits_SOUND_PLAYER			|
		bits_SOUND_COMBAT			|
		bits_SOUND_WORLD,
		"AGruntThreatDisplay"
	},
};

BEGIN_SCHEDULES( CAGrunt )
	slAGruntFail,
	slAGruntCombatFail,
	slAGruntStandoff,
	slAGruntSuppress,
	slAGruntRangeAttack1,
	slAGruntHiddenRangeAttack,
	slAGruntTakeCoverFromEnemy,
	slAGruntVictoryDance,
	slAGruntThreatDisplay,
END_SCHEDULES()

bool CAGrunt::FCanCheckAttacks() const
{
	if ( !HasConditions( bits_COND_ENEMY_TOOFAR ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CAGrunt :: CheckMeleeAttack1 ( float flDot, float flDist )
{
	if ( HasConditions ( bits_COND_SEE_ENEMY ) && flDist <= AGRUNT_MELEE_DIST && flDot >= 0.6 && m_hEnemy != NULL )
	{
		return true;
	}
	return false;
}

bool CAGrunt :: CheckRangeAttack1 ( float flDot, float flDist )
{
	// !!!LATER - we may want to load balance this. Several
	// tracelines are done, so we may not want to do this every
	// server frame. Definitely not while firing. 

	if ( gpGlobals->time < m_flNextHornetAttackCheck )
	{
		return m_fCanHornetAttack;
	}

	if ( HasConditions( bits_COND_SEE_ENEMY ) && flDist >= AGRUNT_MELEE_DIST && flDist <= 1024 && flDot >= 0.5 && NoFriendlyFire() )
	{
		TraceResult	tr;
		Vector	vecArmPos, vecArmDir;

		// verify that a shot fired from the gun will hit the enemy before the world.
		// !!!LATER - we may wish to do something different for projectile weapons as opposed to instant-hit
		UTIL_MakeVectors( GetAbsAngles() );
		GetAttachment( 0, vecArmPos, vecArmDir );
//		UTIL_TraceLine( vecArmPos, vecArmPos + gpGlobals->v_forward * 256, ignore_monsters, ENT(pev), &tr);
		UTIL_TraceLine( vecArmPos, m_hEnemy->BodyTarget(vecArmPos), dont_ignore_monsters, ENT(pev), &tr);

		if ( tr.flFraction == 1.0 || tr.pHit == m_hEnemy->edict() )
		{
			m_flNextHornetAttackCheck = gpGlobals->time + RANDOM_FLOAT( 2, 5 );
			m_fCanHornetAttack = true;
			return m_fCanHornetAttack;
		}
	}
	
	m_flNextHornetAttackCheck = gpGlobals->time + 0.2;// don't check for half second if this check wasn't successful
	m_fCanHornetAttack = false;
	return m_fCanHornetAttack;
}

void CAGrunt :: StartTask ( const Task_t& task )
{
	switch ( task.iTask )
	{
	case TASK_AGRUNT_GET_PATH_TO_ENEMY_CORPSE:
		{
			UTIL_MakeVectors( GetAbsAngles() );
			if ( BuildRoute ( m_vecEnemyLKP - gpGlobals->v_forward * 50, bits_MF_TO_LOCATION, NULL ) )
			{
				TaskComplete();
			}
			else
			{
				ALERT ( at_aiconsole, "AGruntGetPathToEnemyCorpse failed!!\n" );
				TaskFail();
			}
		}
		break;

	case TASK_AGRUNT_SETUP_HIDE_ATTACK:
		// alien grunt shoots hornets back out into the open from a concealed location. 
		// try to find a spot to throw that gives the smart weapon a good chance of finding the enemy.
		// ideally, this spot is along a line that is perpendicular to a line drawn from the agrunt to the enemy.

		CBaseMonster	*pEnemyMonsterPtr;

		pEnemyMonsterPtr = m_hEnemy->MyMonsterPointer();

		if ( pEnemyMonsterPtr )
		{
			Vector		vecCenter;
			TraceResult	tr;
			bool		fSkip = false;

			vecCenter = Center();

			UTIL_VecToAngles( m_vecEnemyLKP - GetAbsOrigin() );

			UTIL_TraceLine( Center() + gpGlobals->v_forward * 128, m_vecEnemyLKP, ignore_monsters, ENT(pev), &tr);
			if ( tr.flFraction == 1.0 )
			{
				MakeIdealYaw ( GetAbsOrigin() + gpGlobals->v_right * 128 );
				fSkip = true;
				TaskComplete();
			}
			
			if ( !fSkip )
			{
				UTIL_TraceLine( Center() - gpGlobals->v_forward * 128, m_vecEnemyLKP, ignore_monsters, ENT(pev), &tr);
				if ( tr.flFraction == 1.0 )
				{
					MakeIdealYaw ( GetAbsOrigin() - gpGlobals->v_right * 128 );
					fSkip = true;
					TaskComplete();
				}
			}
			
			if ( !fSkip )
			{
				UTIL_TraceLine( Center() + gpGlobals->v_forward * 256, m_vecEnemyLKP, ignore_monsters, ENT(pev), &tr);
				if ( tr.flFraction == 1.0 )
				{
					MakeIdealYaw ( GetAbsOrigin() + gpGlobals->v_right * 256 );
					fSkip = true;
					TaskComplete();
				}
			}
			
			if ( !fSkip )
			{
				UTIL_TraceLine( Center() - gpGlobals->v_forward * 256, m_vecEnemyLKP, ignore_monsters, ENT(pev), &tr);
				if ( tr.flFraction == 1.0 )
				{
					MakeIdealYaw ( GetAbsOrigin() - gpGlobals->v_right * 256 );
					fSkip = true;
					TaskComplete();
				}
			}
			
			if ( !fSkip )
			{
				TaskFail();
			}
		}
		else
		{
			ALERT ( at_aiconsole, "AGRunt - no enemy monster ptr!!!\n" );
			TaskFail();
		}
		break;

	default:
		CSquadMonster :: StartTask ( task );
		break;
	}
}

Schedule_t *CAGrunt :: GetSchedule ( void )
{
	if ( HasConditions(bits_COND_HEAR_SOUND) )
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT( pSound != NULL );
		if ( pSound && (pSound->m_iType & bits_SOUND_DANGER) )
		{
			// dangerous sound nearby!
			return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
		}
	}

	switch	( m_MonsterState )
	{
	case MONSTERSTATE_COMBAT:
		{
// dead enemy
			if ( HasConditions( bits_COND_ENEMY_DEAD ) )
			{
				// call base class, all code to handle dead enemies is centralized there.
				return CBaseMonster :: GetSchedule();
			}

			if ( HasConditions(bits_COND_NEW_ENEMY) )
			{
				return GetScheduleOfType( SCHED_WAKE_ANGRY );
			}

	// zap player!
			if ( HasConditions ( bits_COND_CAN_MELEE_ATTACK1 ) )
			{
				AttackSound();// this is a total hack. Should be parto f the schedule
				return GetScheduleOfType ( SCHED_MELEE_ATTACK1 );
			}

			if ( HasConditions ( bits_COND_HEAVY_DAMAGE ) )
			{
				return GetScheduleOfType( SCHED_SMALL_FLINCH );
			}

	// can attack
			if ( HasConditions ( bits_COND_CAN_RANGE_ATTACK1 ) && OccupySlot ( bits_SLOTS_AGRUNT_HORNET ) )
			{
				return GetScheduleOfType ( SCHED_RANGE_ATTACK1 );
			}

			if ( OccupySlot ( bits_SLOT_AGRUNT_CHASE ) )
			{
				return GetScheduleOfType ( SCHED_CHASE_ENEMY );
			}

			return GetScheduleOfType ( SCHED_STANDOFF );
		}

	default: break;
	}

	return CSquadMonster :: GetSchedule();
}

//=========================================================
//=========================================================
Schedule_t* CAGrunt :: GetScheduleOfType ( int Type ) 
{
	switch	( Type )
	{
	case SCHED_TAKE_COVER_FROM_ENEMY:
		return &slAGruntTakeCoverFromEnemy[ 0 ];
		break;
	
	case SCHED_RANGE_ATTACK1:
		if ( HasConditions( bits_COND_SEE_ENEMY ) )
		{
			//normal attack
			return &slAGruntRangeAttack1[ 0 ];
		}
		else
		{
			// attack an unseen enemy
			// return &slAGruntHiddenRangeAttack[ 0 ];
			return &slAGruntRangeAttack1[ 0 ];
		}
		break;

	case SCHED_AGRUNT_THREAT_DISPLAY:
		return &slAGruntThreatDisplay[ 0 ];
		break;

	case SCHED_AGRUNT_SUPPRESS:
		return &slAGruntSuppress[ 0 ];
		break;

	case SCHED_STANDOFF:
		return &slAGruntStandoff[ 0 ];
		break;

	case SCHED_VICTORY_DANCE:
		return &slAGruntVictoryDance[ 0 ];
		break;

	case SCHED_FAIL:
		// no fail schedule specified, so pick a good generic one.
		{
			if ( m_hEnemy != NULL )
			{
				// I have an enemy
				// !!!LATER - what if this enemy is really far away and i'm chasing him?
				// this schedule will make me stop, face his last known position for 2 
				// seconds, and then try to move again
				return &slAGruntCombatFail[ 0 ];
			}

			return &slAGruntFail[ 0 ];
		}
		break;

	}

	return CSquadMonster :: GetScheduleOfType( Type );
}

