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
#ifndef GAME_SERVER_ENTITIES_SOUND_CSPEAKER_H
#define GAME_SERVER_ENTITIES_SOUND_CSPEAKER_H

#define ANNOUNCE_MINUTES_MIN	0.25	 
#define ANNOUNCE_MINUTES_MAX	2.25

#define SPEAKER_START_SILENT			1	// wait for trigger 'on' to start announcements

// ===================================================================================
//
// Speaker class. Used for announcements per level, for door lock/unlock spoken voice. 
//
class CSpeaker : public CBaseEntity
{
public:
	DECLARE_CLASS( CSpeaker, CBaseEntity );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData* pkvd ) override;
	void Spawn() override;
	void Precache() override;
	void ToggleUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void SpeakerThink();

	virtual int	ObjectCaps() const override { return ( CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION ); }

	int	m_preset;			// preset number
};

#endif //GAME_SERVER_ENTITIES_SOUND_CSPEAKER_H