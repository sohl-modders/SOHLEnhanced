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
#ifndef GAME_SERVER_ENTITIES_NPCS_CTURRET_H
#define GAME_SERVER_ENTITIES_NPCS_CTURRET_H

#include "CBaseTurret.h"

class CTurret : public CBaseTurret
{
public:
	DECLARE_CLASS( CTurret, CBaseTurret );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	// Think functions
	void SpinUpCall( void ) override;
	void SpinDownCall( void ) override;

	// other functions
	void Shoot( Vector &vecSrc, Vector &vecDirToEnemy ) override;

private:
	int m_iStartSpin;

};

#endif //GAME_SERVER_ENTITIES_NPCS_CTURRET_H