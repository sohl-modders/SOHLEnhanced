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
#ifndef GAME_SERVER_ENTITIES_NPCS_CCONTROLLERHEADBALL_H
#define GAME_SERVER_ENTITIES_NPCS_CCONTROLLERHEADBALL_H

//=========================================================
// Controller bouncy ball attack
//=========================================================
class CControllerHeadBall : public CBaseMonster
{
public:
	DECLARE_CLASS( CControllerHeadBall, CBaseMonster );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	void HuntThink( void );
	void DieThink( void );
	void BounceTouch( CBaseEntity *pOther );
	void MovetoTarget( Vector vecTarget );
	void Crawl( void );

private:
	int m_iTrail;
	Vector m_vecIdeal;
	EHANDLE m_hOwner;
};

#endif //GAME_SERVER_ENTITIES_NPCS_CCONTROLLERHEADBALL_H