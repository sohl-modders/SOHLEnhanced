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
#ifndef GAME_SERVER_ENTITIES_ITEMS_CITEM_H
#define GAME_SERVER_ENTITIES_ITEMS_CITEM_H

class CItem : public CBaseEntity
{
public:
	DECLARE_CLASS( CItem, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	CBaseEntity*	Respawn( void ) override;
	void	ItemTouch( CBaseEntity *pOther );
	void	Materialize( void );
	virtual bool MyTouch( CBasePlayer *pPlayer ) { return false; }
};

#endif //GAME_SERVER_ENTITIES_ITEMS_CITEM_H
