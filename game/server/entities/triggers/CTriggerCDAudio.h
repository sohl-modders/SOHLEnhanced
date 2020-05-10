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
#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERCDAUDIO_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERCDAUDIO_H

#include "CBaseTrigger.h"

//
// trigger_cdaudio - starts/stops cd audio tracks
//
class CTriggerCDAudio : public CBaseTrigger
{
public:
	DECLARE_CLASS( CTriggerCDAudio, CBaseTrigger );

	void Spawn( void ) override;

	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void PlayTrack( void );
	void Touch( CBaseEntity *pOther ) override;
};

void PlayCDTrack( int iTrack );

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERCDAUDIO_H