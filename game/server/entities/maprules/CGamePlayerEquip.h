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
#ifndef GAME_SERVER_ENTITIES_MAPRULES_CGAMEPLAYEREQUIP_H
#define GAME_SERVER_ENTITIES_MAPRULES_CGAMEPLAYEREQUIP_H

#include "CRulePointEntity.h"

#define SF_PLAYEREQUIP_USEONLY			0x0001
#define MAX_EQUIP		32

//
// CGamePlayerEquip / game_playerequip	-- Sets the default player equipment
// Flag: USE Only
class CGamePlayerEquip : public CRulePointEntity
{
public:
	DECLARE_CLASS( CGamePlayerEquip, CRulePointEntity );

	void		KeyValue( KeyValueData *pkvd ) override;
	void		Touch( CBaseEntity *pOther ) override;
	void		Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	inline bool	UseOnly() const { return GetSpawnFlags().Any( SF_PLAYEREQUIP_USEONLY ); }

private:

	void		EquipPlayer( CBaseEntity *pPlayer );

	string_t	m_weaponNames[ MAX_EQUIP ];
	int			m_weaponCount[ MAX_EQUIP ];
};

#endif //GAME_SERVER_ENTITIES_MAPRULES_CGAMEPLAYEREQUIP_H