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
#ifndef GAME_SERVER_ENTITIES_TANKS_CFUNCTANKLASER_H
#define GAME_SERVER_ENTITIES_TANKS_CFUNCTANKLASER_H

#include "CFuncTank.h"

class CLaser;

class CFuncTankLaser : public CFuncTank
{
public:
	DECLARE_CLASS( CFuncTankLaser, CFuncTank );
	DECLARE_DATADESC();

	void	Activate( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;
	void	Fire( const Vector &barrelEnd, const Vector &forward, CBaseEntity* pAttacker ) override;
	void	Think( void ) override;
	CLaser *GetLaser( void );

private:
	CLaser	*m_pLaser;
	float	m_laserTime;
};

#endif //GAME_SERVER_ENTITIES_TANKS_CFUNCTANKLASER_H