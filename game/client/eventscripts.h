//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// eventscripts.h
#if !defined ( EVENTSCRIPTSH )
#define EVENTSCRIPTSH

#include "event_args.h"

#include "Damage.h"

struct cl_entity_t;

// Some of these are HL/TFC specific?
void EV_EjectBrass( const Vector& vecOrigin, const Vector& vecVelocity, float rotation, int model, int soundtype );
void EV_GetGunPosition( event_args_t *args, Vector& pos, const Vector& origin );
void EV_GetDefaultShellInfo( event_args_t *args, 
							 const Vector& origin, const Vector& velocity,
							 Vector& ShellVelocity, Vector& ShellOrigin,
							 const Vector& forward, const Vector& right, const Vector& up,
							 float forwardScale, float upScale, float rightScale );
bool EV_IsLocal( int idx );
bool EV_IsPlayer( int idx );
void EV_CreateTracer( Vector& start, const Vector& end );

cl_entity_t *GetEntity( int idx );
cl_entity_t *GetViewEntity( void );
void EV_MuzzleFlash( void );

#endif // EVENTSCRIPTSH
