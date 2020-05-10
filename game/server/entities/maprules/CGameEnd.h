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
#ifndef GAME_SERVER_ENTITIES_MAPRULES_CGAMEEND_H
#define GAME_SERVER_ENTITIES_MAPRULES_CGAMEEND_H

#include "CRulePointEntity.h"

// CGameEnd / game_end	-- Ends the game in MP
class CGameEnd : public CRulePointEntity
{
public:
	DECLARE_CLASS( CGameEnd, CRulePointEntity );

	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
private:
};

#endif //GAME_SERVER_ENTITIES_MAPRULES_CGAMEEND_H