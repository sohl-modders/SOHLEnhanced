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
#ifndef GAME_SERVER_ENTITIES_NPCS_CSTOMP_H
#define GAME_SERVER_ENTITIES_NPCS_CSTOMP_H

#define	STOMP_INTERVAL		0.025
#define STOMP_SPRITE_COUNT			10

#define GARG_STOMP_BUZZ_SOUND		"weapons/mine_charge.wav"
#define GARG_STOMP_SPRITE_NAME		"sprites/gargeye1.spr"

class CStomp : public CBaseEntity
{
public:
	DECLARE_CLASS( CStomp, CBaseEntity );

	void Spawn( void ) override;
	void Think( void ) override;
	static CStomp *StompCreate( const Vector &origin, const Vector &end, float speed );

private:
	// UNDONE: re-use this sprite list instead of creating new ones all the time
	//	CSprite		*m_pSprites[ STOMP_SPRITE_COUNT ];
};

#endif //GAME_SERVER_ENTITIES_NPCS_CSTOMP_H