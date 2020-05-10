//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: Functionality for the observer chase camera
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"
#include "Weapons.h"
#include "pm_shared.h"
#include "entities/CCorpse.h"

// Find the next client in the game for this player to spectate
void CBasePlayer::Observer_FindNextPlayer( bool bReverse )
{
	// MOD AUTHORS: Modify the logic of this function if you want to restrict the observer to watching
	//				only a subset of the players. e.g. Make it check the target's team.

	int		iStart;
	if ( m_hObserverTarget )
		iStart = m_hObserverTarget->entindex();
	else
		iStart = entindex();
	int	    iCurrent = iStart;
	m_hObserverTarget = NULL;
	int iDir = bReverse ? -1 : 1; 

	do
	{
		iCurrent += iDir;

		// Loop through the clients
		if (iCurrent > gpGlobals->maxClients)
			iCurrent = 1;
		if (iCurrent < 1)
			iCurrent = gpGlobals->maxClients;

		CBaseEntity *pEnt = UTIL_PlayerByIndex( iCurrent );
		if ( !pEnt )
			continue;
		if ( pEnt == this )
			continue;
		// Don't spec observers or players who haven't picked a class yet
		if ( ((CBasePlayer*)pEnt)->IsObserver() || (pEnt->GetEffects().Any( EF_NODRAW ) ) )
			continue;

		// MOD AUTHORS: Add checks on target here.

		m_hObserverTarget = pEnt;
		break;

	} while ( iCurrent != iStart );

	// Did we find a target?
	if ( m_hObserverTarget )
	{
		// Move to the target
		SetAbsOrigin( m_hObserverTarget->GetAbsOrigin() );

		// ALERT( at_console, "Now Tracking %s\n", m_hObserverTarget->GetNetName() );

		// Store the target in pev so the physics DLL can get to it
		if ( GetObserverMode() != OBS_ROAMING)
			pev->iuser2 = m_hObserverTarget->entindex();
	
		
		
	}
}

// Handle buttons in observer mode
void CBasePlayer::Observer_HandleButtons()
{
	// Slow down mouse clicks
	if ( m_flNextObserverInput > gpGlobals->time )
		return;

	// Jump changes from modes: Chase to Roaming
	if ( m_afButtonPressed & IN_JUMP )
	{
		if ( GetObserverMode() == OBS_CHASE_LOCKED )
			Observer_SetMode( OBS_CHASE_FREE );

		else if ( GetObserverMode() == OBS_CHASE_FREE )
			Observer_SetMode( OBS_IN_EYE );

		else if ( GetObserverMode() == OBS_IN_EYE )
			Observer_SetMode( OBS_ROAMING );

		else if ( GetObserverMode() == OBS_ROAMING )
			Observer_SetMode( OBS_MAP_FREE );

		else if ( GetObserverMode() == OBS_MAP_FREE )
			Observer_SetMode( OBS_MAP_CHASE );

		else
			Observer_SetMode( OBS_CHASE_FREE );	// don't use OBS_CHASE_LOCKED anymore

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	// Attack moves to the next player
	if ( m_afButtonPressed & IN_ATTACK )//&& GetObserverMode() != OBS_ROAMING )
	{
		Observer_FindNextPlayer( false );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}

	// Attack2 moves to the prev player
	if ( m_afButtonPressed & IN_ATTACK2)// && GetObserverMode() != OBS_ROAMING )
	{
		Observer_FindNextPlayer( true );

		m_flNextObserverInput = gpGlobals->time + 0.2;
	}
}

void CBasePlayer::Observer_CheckTarget()
{
	if( GetObserverMode() == OBS_ROAMING )
		return;

	// try to find a traget if we have no current one
	if ( m_hObserverTarget == NULL)
	{
		Observer_FindNextPlayer( false );

		if (m_hObserverTarget == NULL)
		{
			// no target found at all 

			int lastMode = GetObserverMode();

			Observer_SetMode( OBS_ROAMING );

			m_iObserverLastMode = lastMode;	// don't overwrite users lastmode

			return;	// we still have np target return
		}
	}

	CBasePlayer* target = UTIL_PlayerByIndex( m_hObserverTarget->entindex() );

	if ( !target )
	{
		Observer_FindNextPlayer( false );
		return;
	}

	// check taget
	if (target->GetDeadFlag() == DEAD_DEAD)
	{
		if ( (target->m_fDeadTime + 2.0f ) < gpGlobals->time )
		{
			// 3 secs after death change target
			Observer_FindNextPlayer( false );
			return;
		}
	}
}

void CBasePlayer::Observer_CheckProperties()
{
	// try to find a traget if we have no current one
	if ( GetObserverMode() == OBS_IN_EYE && m_hObserverTarget != NULL)
	{
		CBasePlayer* target = UTIL_PlayerByIndex( m_hObserverTarget->entindex() );

		if (!target )
			return;

		int weapon = (target->m_pActiveItem!=NULL)?target->m_pActiveItem->m_iId:0;
		// use fov of tracked client
		if ( m_iFOV != target->m_iFOV || m_iObserverWeapon != weapon )
		{
			m_iFOV = target->m_iFOV;
			m_iClientFOV = m_iFOV;
			// write fov before wepon data, so zoomed crosshair is set correctly
			MESSAGE_BEGIN( MSG_ONE, gmsgSetFOV, NULL, this );
				WRITE_BYTE( m_iFOV );
			MESSAGE_END();


			m_iObserverWeapon = weapon;
			//send weapon update
			MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, this );
				WRITE_BYTE( 1 );	// 1 = current weapon, not on target
				WRITE_BYTE( m_iObserverWeapon );	
				WRITE_BYTE( 0 );	// clip
			MESSAGE_END();
		}
	}
	else
	{
		m_iFOV = 90;
		
		if ( m_iObserverWeapon != 0 )
		{
			m_iObserverWeapon = 0;

			MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, this );
				WRITE_BYTE( 1 );	// 1 = current weapon
				WRITE_BYTE( m_iObserverWeapon );	
				WRITE_BYTE( 0 );	// clip
			MESSAGE_END();
		}
	}
}

// Attempt to change the observer mode
void CBasePlayer::Observer_SetMode( int iMode )
{

	// Just abort if we're changing to the mode we're already in
	if ( iMode == GetObserverMode() )
		return;

	// is valid mode ?
	if ( iMode < OBS_CHASE_LOCKED || iMode > OBS_MAP_CHASE )
		iMode = OBS_IN_EYE; // now it is
	// verify observer target again
	if ( m_hObserverTarget != NULL)
	{
		CBaseEntity *pEnt = m_hObserverTarget;

		if ( (pEnt == this) || (pEnt == NULL) )
			m_hObserverTarget = NULL;
		else if ( ((CBasePlayer*)pEnt)->IsObserver() || (pEnt->GetEffects().Any( EF_NODRAW ) ) )
			m_hObserverTarget = NULL;
	}

	// set spectator mode
	InternalSetObserverMode( iMode );

	// if we are not roaming, we need a valid target to track
	if ( (iMode != OBS_ROAMING) && (m_hObserverTarget == NULL) )
	{
		Observer_FindNextPlayer( false );

		// if we didn't find a valid target switch to roaming
		if (m_hObserverTarget == NULL)
		{
			ClientPrint( this, HUD_PRINTCENTER, "#Spec_NoTarget"  );
			InternalSetObserverMode( OBS_ROAMING );
		}
	}

	// set target if not roaming
	if ( GetObserverMode() == OBS_ROAMING)
	{
		SetObserverTargetIndex( 0 );
	}
	else
		SetObserverTargetIndex( m_hObserverTarget->entindex() );
	
	pev->iuser3 = 0;	// clear second target from death cam
	
	// print spepctaor mode on client screen

	char modemsg[16];
	sprintf(modemsg,"#Spec_Mode%i", GetObserverMode() );
	ClientPrint( this, HUD_PRINTCENTER, modemsg );

	m_iObserverLastMode = iMode;
}

//=========================================================
// StartDeathCam - find an intermission spot and send the
// player off into observer mode
//=========================================================
void CBasePlayer::StartDeathCam()
{
	if( GetViewOffset() == g_vecZero )
	{
		// don't accept subsequent attempts to StartDeathCam()
		return;
	}

	CBaseEntity* pSpot = UTIL_FindEntityByClassname( nullptr, "info_intermission" );

	if( !FNullEnt( pSpot ) )
	{
		CBaseEntity* pNewSpot;

		// at least one intermission spot in the world.
		int iRand = RANDOM_LONG( 0, 3 );

		while( iRand > 0 )
		{
			pNewSpot = UTIL_FindEntityByClassname( pSpot, "info_intermission" );

			if( pNewSpot )
			{
				pSpot = pNewSpot;
			}

			iRand--;
		}

		CopyToBodyQue( this );

		SetAbsOrigin( pSpot->GetAbsOrigin() );
		SetViewAngle( pSpot->GetViewAngle() );
		SetAbsAngles( pSpot->GetViewAngle() );
	}
	else
	{
		// no intermission spot. Push them up in the air, looking down at their corpse
		TraceResult tr;
		CopyToBodyQue( this );
		UTIL_TraceLine( GetAbsOrigin(), GetAbsOrigin() + Vector( 0, 0, 128 ), ignore_monsters, edict(), &tr );

		SetAbsOrigin( tr.vecEndPos );
		SetViewAngle( UTIL_VecToAngles( tr.vecEndPos - GetAbsOrigin() ) );
		SetAbsAngles( GetViewAngle() );
	}

	// start death cam

	m_afPhysicsFlags |= PFLAG_OBSERVER;
	SetViewOffset( g_vecZero );
	SetFixAngleMode( FIXANGLE_SET );
	SetSolidType( SOLID_NOT );
	SetTakeDamageMode( DAMAGE_NO );
	SetMoveType( MOVETYPE_NONE );
	SetModelIndex( 0 );
}

void CBasePlayer::StartObserver( Vector vecPosition, Vector vecViewAngle )
{
	// clear any clientside entities attached to this player
	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, GetAbsOrigin() );
		WRITE_BYTE( TE_KILLPLAYERATTACHMENTS );
		WRITE_BYTE( ( byte ) entindex() );
	MESSAGE_END();

	// Holster weapon immediately, to allow it to cleanup
	if( m_pActiveItem )
		m_pActiveItem->Holster();

	if( m_pTank != NULL )
	{
		m_pTank->Use( this, this, USE_OFF, 0 );
		m_pTank = NULL;
	}

	// clear out the suit message cache so we don't keep chattering
	SetSuitUpdate( NULL, SUIT_SENTENCE, 0 );

	// Tell Ammo Hud that the player is dead
	MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, this );
		WRITE_BYTE( 0 );
		WRITE_BYTE( 0XFF );
		WRITE_BYTE( 0xFF );
	MESSAGE_END();

	// reset FOV
	m_iFOV = m_iClientFOV = 0;
	SetFOV( m_iFOV );
	MESSAGE_BEGIN( MSG_ONE, gmsgSetFOV, NULL, this );
		WRITE_BYTE( 0 );
	MESSAGE_END();

	// Setup flags
	m_iHideHUD = ( HIDEHUD_HEALTH | HIDEHUD_WEAPONS );
	m_afPhysicsFlags |= PFLAG_OBSERVER;
	GetEffects() = EF_NODRAW;
	SetViewOffset( g_vecZero );
	SetViewAngle( vecViewAngle );
	SetAbsAngles( vecViewAngle );
	SetFixAngleMode( FIXANGLE_SET );
	SetSolidType( SOLID_NOT );
	SetTakeDamageMode( DAMAGE_NO );
	SetMoveType( MOVETYPE_NONE );
	ClearBits( m_afPhysicsFlags, PFLAG_DUCKING );
	GetFlags().ClearFlags( FL_DUCKING );
	SetDeadFlag( DEAD_RESPAWNABLE );
	SetHealth( 1 );

	// Clear out the status bar
	m_fInitHUD = true;

	pev->team = 0;
	MESSAGE_BEGIN( MSG_ALL, gmsgTeamInfo );
	WRITE_BYTE( entindex() );
		WRITE_STRING( "" );
	MESSAGE_END();

	// Remove all the player's stuff
	RemoveAllItems( false );

	// Move them to the new position
	SetAbsOrigin( vecPosition );

	// Find a player to watch
	m_flNextObserverInput = 0;
	Observer_SetMode( m_iObserverLastMode );
}