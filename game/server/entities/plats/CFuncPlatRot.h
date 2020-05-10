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
#ifndef GAME_SERVER_ENTITIES_PLATS_CFUNCPLATROT_H
#define GAME_SERVER_ENTITIES_PLATS_CFUNCPLATROT_H

#include "CFuncPlat.h"

class CFuncPlatRot : public CFuncPlat
{
public:
	DECLARE_CLASS( CFuncPlatRot, CFuncPlat );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void SetupRotation( void );

	virtual void	GoUp( void ) override;
	virtual void	GoDown( void ) override;
	virtual void	HitTop( void ) override;
	virtual void	HitBottom( void ) override;

	void			RotMove( Vector &destAngle, float time );

	Vector	m_end, m_start;
};

#endif //GAME_SERVER_ENTITIES_PLATS_CFUNCPLATROT_H