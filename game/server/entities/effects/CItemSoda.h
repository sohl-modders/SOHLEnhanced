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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CITEMSODA_H
#define GAME_SERVER_ENTITIES_EFFECTS_CITEMSODA_H

//=========================================================
// Soda can
//=========================================================
class CItemSoda : public CBaseEntity
{
public:
	DECLARE_CLASS( CItemSoda, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	Precache( void ) override;
	void	CanThink( void );
	void	CanTouch( CBaseEntity *pOther );
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CITEMSODA_H