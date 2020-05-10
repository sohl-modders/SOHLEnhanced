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
#ifndef GAME_SERVER_ENTITIES_NPCS_CSQUIDSPIT_H
#define GAME_SERVER_ENTITIES_NPCS_CSQUIDSPIT_H

extern int iSquidSpitSprite;

//=========================================================
// Bullsquid's spit projectile
//=========================================================
class CSquidSpit : public CBaseEntity
{
public:
	DECLARE_CLASS( CSquidSpit, CBaseEntity );
	DECLARE_DATADESC();

	void Spawn( void ) override;

	static void Shoot( CBaseEntity* pOwner, Vector vecStart, Vector vecVelocity );
	void Touch( CBaseEntity *pOther ) override;
	void Animate( void );

	int  m_maxFrame;
};

#endif //GAME_SERVER_ENTITIES_NPCS_CSQUIDSPIT_H