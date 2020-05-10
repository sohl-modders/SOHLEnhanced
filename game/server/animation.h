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
#ifndef ANIMATION_H
#define ANIMATION_H

#define ACTIVITY_NOT_AVAILABLE		-1

#include "entities/AnimationEvent.h"

bool IsSoundEvent( int eventNumber );

int LookupActivity( void *pmodel, int activity );
int LookupActivityHeaviest( void *pmodel, int activity );
int LookupSequence( void *pmodel, const char *label );
void GetSequenceInfo( void *pmodel, entvars_t *pev, float& flFrameRate, float& flGroundSpeed );
int GetSequenceFlags( void *pmodel, entvars_t *pev );
int LookupAnimationEvents( void *pmodel, entvars_t *pev, float flStart, float flEnd );
float SetController( void *pmodel, entvars_t *pev, int iController, float flValue );
float SetBlending( void *pmodel, entvars_t *pev, int iBlender, float flValue );
void GetEyePosition( void *pmodel, Vector& vecEyePosition );
void SequencePrecache( void *pmodel, const char *pSequenceName );
int FindTransition( void *pmodel, int iEndingAnim, int iGoalAnim, int *piDir );
void SetBodygroup( void *pmodel, entvars_t *pev, int iGroup, int iValue );
int GetBodygroup( void *pmodel, entvars_t *pev, int iGroup );

int GetAnimationEvent( void *pmodel, entvars_t *pev, AnimEvent_t& event, float flStart, float flEnd, int index );
int ExtractBbox( void *pmodel, int sequence, Vector& vecMins, Vector& vecMaxs );

// From /engine/studio.h
#define STUDIO_LOOPING		0x0001


#endif	//ANIMATION_H
