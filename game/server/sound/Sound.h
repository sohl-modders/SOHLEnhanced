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
#ifndef GAME_SERVER_SOUND_SOUND_H
#define GAME_SERVER_SOUND_SOUND_H

#include "CSentenceGroups.h"

enum SoundFlag
{
	/**
	*	Duplicated in protocol.h we're spawing, used in some cases for ambients.
	*	Uses the signon buffer instead of the datagram buffer.
	*/
	SND_SPAWNING = 1 << 8,

	/**
	*	Duplicated in protocol.h stop sound.
	*/
	SND_STOP = 1 << 5,

	/**
	*	Duplicated in protocol.h change sound vol.
	*/
	SND_CHANGE_VOL = 1 << 6,

	/**
	*	Duplicated in protocol.h change sound pitch.
	*/
	SND_CHANGE_PITCH = 1 << 7,
};

extern CSentenceGroups g_Sentences;

float TEXTURETYPE_PlaySound( const TraceResult& tr, Vector vecSrc, Vector vecEnd, int iBulletType );

// NOTE: use EMIT_SOUND_DYN to set the pitch of a sound. Pitch of 100
// is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
// down to 1 is a lower pitch.   150 to 70 is the realistic range.
// EMIT_SOUND_DYN with pitch != 100 should be used sparingly, as it's not quite as
// fast as EMIT_SOUND (the pitchshift mixer is not native coded).

void EMIT_SOUND_DYN( CBaseEntity* pEntity, int channel, const char *sample, float volume, float attenuation,
					 int flags, int pitch );


void EMIT_SOUND( CBaseEntity* pEntity, int channel, const char *sample, float volume, float attenuation );

void STOP_SOUND( CBaseEntity* pEntity, int channel, const char* const pszSample );

void EMIT_SOUND_SUIT( CBaseEntity* pEntity, const char *sample );
void EMIT_GROUPID_SUIT( CBaseEntity* pEntity, int isentenceg );
void EMIT_GROUPNAME_SUIT( CBaseEntity* pEntity, const char *groupname );

void UTIL_EmitAmbientSound( CBaseEntity* pEntity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch );

#define PRECACHE_SOUND_ARRAY( a ) \
	{ for( size_t i = 0; i < ARRAYSIZE( a ); ++i ) PRECACHE_SOUND((char *) a [i]); }

#define EMIT_SOUND_ARRAY_DYN( chan, array ) \
	EMIT_SOUND_DYN ( this, chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], 1.0, ATTN_NORM, 0, RANDOM_LONG(95,105) ); 

#define RANDOM_SOUND_ARRAY( array ) (array) [ RANDOM_LONG(0,ARRAYSIZE( (array) )-1) ]

#endif //GAME_SERVER_SOUND_SOUND_H