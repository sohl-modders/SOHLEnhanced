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
#ifndef GAME_SERVER_ENTITIES_BMODELS_CFUNCWALLTOGGLE_H
#define GAME_SERVER_ENTITIES_BMODELS_CFUNCWALLTOGGLE_H

#include "CFuncWall.h"

#define SF_WALL_START_OFF		0x0001

class CFuncWallToggle : public CFuncWall
{
public:
	void	Spawn( void ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void	TurnOff( void );
	void	TurnOn( void );
	bool	IsOn() const;
};

#endif //GAME_SERVER_ENTITIES_BMODELS_CFUNCWALLTOGGLE_H