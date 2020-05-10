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
#ifndef GAME_SERVER_ENTITIES_XEN_CXENHAIR_H
#define GAME_SERVER_ENTITIES_XEN_CXENHAIR_H

#include "entities/CActAnimating.h"

#define SF_HAIR_SYNC		0x0001

class CXenHair : public CActAnimating
{
public:
	DECLARE_CLASS( CXenHair, CActAnimating );

	void		Spawn( void ) override;
	void		Precache( void ) override;
	void		Think( void ) override;
};

#endif //GAME_SERVER_ENTITIES_XEN_CXENHAIR_H