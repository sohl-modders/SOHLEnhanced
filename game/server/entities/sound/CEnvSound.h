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
#ifndef GAME_SERVER_ENTITIES_SOUND_CENVSOUND_H
#define GAME_SERVER_ENTITIES_SOUND_CENVSOUND_H

// =================== ROOM SOUND FX ==========================================
//
// env_sound - spawn a sound entity that will set player roomtype
// when player moves in range and sight.
//
//
class CEnvSound : public CPointEntity
{
public:
	DECLARE_CLASS( CEnvSound, CPointEntity );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData* pkvd ) override;
	void Spawn( void ) override;

	void Think( void ) override;

	float m_flRadius;
	float m_flRoomtype;
};

#endif //GAME_SERVER_ENTITIES_SOUND_CENVSOUND_H