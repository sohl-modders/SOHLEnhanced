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
#ifndef GAME_SERVER_ENTITIES_MAPRULES_CRULEENTITY_H
#define GAME_SERVER_ENTITIES_MAPRULES_CRULEENTITY_H

//	-------------------------------------------
//
//	This module contains entities for implementing/changing game
//	rules dynamically within each map (.BSP)
//
//	-------------------------------------------

class CRuleEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CRuleEntity, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;

	void	SetMaster( int iszMaster ) { m_iszMaster = iszMaster; }

protected:
	bool	CanFireForActivator( CBaseEntity *pActivator ) const;

private:
	string_t	m_iszMaster;
};

#endif //GAME_SERVER_ENTITIES_MAPRULES_CRULEENTITY_H