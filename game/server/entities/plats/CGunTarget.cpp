#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CPathCorner.h"

#include "CGunTarget.h"

BEGIN_DATADESC( CGunTarget )
	DEFINE_FIELD( m_bOn, FIELD_BOOLEAN ),
	DEFINE_THINKFUNC( Next ),
	DEFINE_THINKFUNC( Start ),
	DEFINE_THINKFUNC( Wait ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( func_guntarget, CGunTarget );

void CGunTarget::Spawn()
{
	SetSolidType( SOLID_BSP );
	SetMoveType( MOVETYPE_PUSH );

	SetAbsOrigin( GetAbsOrigin() );
	SetModel( GetModelName() );

	if( GetSpeed() == 0 )
		SetSpeed( 100 );

	// Don't take damage until "on"
	SetTakeDamageMode( DAMAGE_NO );
	GetFlags() |= FL_MONSTER;

	m_bOn = false;
	SetMaxHealth( GetHealth() );

	if( ShouldStartOn() )
	{
		SetThink( &CGunTarget::Start );
		SetNextThink( GetLastThink() + 0.3 );
	}
}

void CGunTarget::Activate()
{
	// now find our next target
	CBaseEntity* pTarg = GetNextTarget();

	if( pTarg )
	{
		m_hTargetEnt = pTarg;
		SetAbsOrigin( pTarg->GetAbsOrigin() - ( GetRelMin() + GetRelMax() ) * 0.5 );
	}
}

void CGunTarget::Next()
{
	SetThink( nullptr );

	m_hTargetEnt = GetNextTarget();
	CBaseEntity *pTarget = m_hTargetEnt;

	if( !pTarget )
	{
		Stop();
		return;
	}
	SetMoveDone( &CGunTarget::Wait );
	LinearMove( pTarget->GetAbsOrigin() - ( GetRelMin() + GetRelMax() ) * 0.5, GetSpeed() );
}

void CGunTarget::Start()
{
	Use( this, this, USE_ON, 0 );
}

void CGunTarget::Wait()
{
	CBaseEntity *pTarget = m_hTargetEnt;

	if( !pTarget )
	{
		Stop();
		return;
	}

	// Fire the pass target if there is one
	if( pTarget->HasMessage() )
	{
		FireTargets( pTarget->GetMessage(), this, this, USE_TOGGLE, 0 );
		if( pTarget->GetSpawnFlags().Any( SF_CORNER_FIREONCE ) )
			pTarget->ClearMessage();
	}

	m_flWait = pTarget->GetDelay();

	SetTarget( pTarget->GetTarget() );

	SetThink( &CGunTarget::Next );
	if( m_flWait != 0 )
	{
		// -1 wait will wait forever!		
		SetNextThink( GetLastThink() + m_flWait );
	}
	else
	{
		Next();// do it RIGHT now!
	}
}

void CGunTarget::Stop()
{
	m_bOn = false;

	SetAbsVelocity( g_vecZero );
	SetNextThink( 0 );
	SetTakeDamageMode( DAMAGE_NO );
}

void CGunTarget::OnTakeDamage( const CTakeDamageInfo& info )
{
	if( GetHealth() > 0 )
	{
		SetHealth( GetHealth() - info.GetDamage() );
		if( GetHealth() <= 0 )
		{
			SetHealth( 0 );
			Stop();
			if( HasMessage() )
				FireTargets( GetMessage(), this, this, USE_TOGGLE, 0 );
		}
	}
}

void CGunTarget::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !ShouldToggle( useType, m_bOn ) )
		return;

	if( m_bOn )
	{
		Stop();
	}
	else
	{
		m_bOn = true;

		SetTakeDamageMode( DAMAGE_AIM );
		m_hTargetEnt = GetNextTarget();
		if( m_hTargetEnt == NULL )
			return;
		SetHealth( GetMaxHealth() );
		Next();
	}
}
