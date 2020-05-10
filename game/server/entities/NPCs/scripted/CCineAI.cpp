#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CCineAI.h"

LINK_ENTITY_TO_CLASS( aiscripted_sequence, CCineAI );

// lookup a sequence name and setup the target monster to play it
// overridden for CCineAI because it's ok for them to not have an animation sequence
// for the monster to play. For a regular Scripted Sequence, that situation is an error.
bool CCineAI::StartSequence( CBaseMonster *pTarget, int iszSeq, const bool completeOnEmpty )
{
	if( iszSeq == 0 && completeOnEmpty )
	{
		// no sequence was provided. Just let the monster proceed, however, we still have to fire any Sequence target
		// and remove any non-repeatable CineAI entities here ( because there is code elsewhere that handles those tasks, but
		// not until the animation sequence is finished. We have to manually take care of these things where there is no sequence.

		SequenceDone( pTarget );

		return true;
	}

	pTarget->SetSequence( pTarget->LookupSequence( STRING( iszSeq ) ) );

	if( pTarget->GetSequence() == -1 )
	{
		ALERT( at_error, "%s: unknown aiscripted sequence \"%s\"\n", pTarget->GetTargetname(), STRING( iszSeq ) );
		pTarget->SetSequence( 0 );
		// return false;
	}

	pTarget->SetFrame( 0 );
	pTarget->ResetSequenceInfo();
	return true;
}

// make the entity carry out the scripted sequence instructions, but without 
// destroying the monster's state.
void CCineAI::PossessEntity( void )
{
	Schedule_t *pNewSchedule;

	CBaseEntity		*pEntity = m_hTargetEnt;
	CBaseMonster	*pTarget = NULL;
	if( pEntity )
		pTarget = pEntity->MyMonsterPointer();

	if( pTarget )
	{
		if( !pTarget->CanPlaySequence( FCanOverrideState(), SS_INTERRUPT_AI ) )
		{
			ALERT( at_aiconsole, "(AI)Can't possess entity %s\n", pTarget->GetClassname() );
			return;
		}

		pTarget->m_hGoalEnt = this;
		pTarget->m_pCine = this;
		pTarget->m_hTargetEnt = this;

		m_saved_movetype = pTarget->GetMoveType();
		m_saved_solid = pTarget->GetSolidType();
		m_saved_effects = pTarget->GetEffects();
		pTarget->GetEffects() |= GetEffects();

		switch( m_fMoveTo )
		{
		case 0:
		case 5:
			pTarget->m_scriptState = SCRIPT_WAIT;
			break;

		case 1:
			pTarget->m_scriptState = SCRIPT_WALK_TO_MARK;
			break;

		case 2:
			pTarget->m_scriptState = SCRIPT_RUN_TO_MARK;
			break;

		case 4:
			{
				// zap the monster instantly to the site of the script entity.
				pTarget->SetAbsOrigin( GetAbsOrigin() );
				pTarget->SetIdealYaw( GetAbsAngles().y );
				pTarget->SetAngularVelocity( g_vecZero );
				pTarget->SetAbsVelocity( Vector( 0, 0, 0 ) );
				pTarget->GetEffects() |= EF_NOINTERP;
				Vector vecAngles = pTarget->GetAbsAngles();
				vecAngles.y = GetAbsAngles().y;
				pTarget->SetAbsAngles( vecAngles );
				pTarget->m_scriptState = SCRIPT_WAIT;
				m_startTime = gpGlobals->time + 1E6;
				// UNDONE: Add a flag to do this so people can fixup physics after teleporting monsters
				pTarget->GetFlags().ClearFlags( FL_ONGROUND );
				break;
			}
		default:
			ALERT( at_aiconsole, "aiscript:  invalid Move To Position value!" );
			break;
		}

		ALERT( at_aiconsole, "\"%s\" found and used\n", pTarget->GetTargetname() );

		pTarget->m_IdealMonsterState = MONSTERSTATE_SCRIPT;

		/*
		if (m_iszIdle)
		{
		StartSequence( pTarget, m_iszIdle, false );
		if (FStrEq( STRING(m_iszIdle), STRING(m_iszPlay)))
		{
		pTarget->SetFrameRate( 0 );
		}
		}
		*/
		// Already in a scripted state?
		if( pTarget->m_MonsterState == MONSTERSTATE_SCRIPT )
		{
			pNewSchedule = pTarget->GetScheduleOfType( SCHED_AISCRIPT );
			pTarget->ChangeSchedule( pNewSchedule );
		}
	}
}

//=========================================================
// FCanOverrideState - returns true because scripted AI can
// possess entities regardless of their state.
//=========================================================
bool CCineAI::FCanOverrideState() const
{
	return true;
}

//=========================================================
// When a monster finishes a scripted sequence, we have to 
// fix up its state and schedule for it to return to a 
// normal AI monster. 
//
// AI Scripted sequences will, depending on what the level
// designer selects:
//
// -Dirty the monster's schedule and drop out of the 
//  sequence in their current state.
//
// -Select a specific AMBUSH schedule, regardless of state.
//=========================================================
void CCineAI::FixScriptMonsterSchedule( CBaseMonster *pMonster )
{
	switch( m_iFinishSchedule )
	{
	case SCRIPT_FINISHSCHED_DEFAULT:
		pMonster->ClearSchedule();
		break;
	case SCRIPT_FINISHSCHED_AMBUSH:
		pMonster->ChangeSchedule( pMonster->GetScheduleOfType( SCHED_AMBUSH ) );
		break;
	default:
		ALERT( at_aiconsole, "FixScriptMonsterSchedule - no case!\n" );
		pMonster->ClearSchedule();
		break;
	}
}