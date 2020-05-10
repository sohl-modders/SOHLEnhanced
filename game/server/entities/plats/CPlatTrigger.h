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
#ifndef GAME_SERVER_ENTITIES_PLATS_CPLATTRIGGER_H
#define GAME_SERVER_ENTITIES_PLATS_CPLATTRIGGER_H

class CFuncPlat;

// UNDONE: Need to save this!!! It needs class & linkage
class CPlatTrigger : public CBaseEntity
{
public:
	DECLARE_CLASS( CPlatTrigger, CBaseEntity );

	virtual int	ObjectCaps() const override { return ( CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION ) | FCAP_DONT_SAVE; }
	void SpawnInsideTrigger( CFuncPlat *pPlatform );
	void Touch( CBaseEntity *pOther ) override;

	static void PlatSpawnInsideTrigger( CFuncPlat* pPlatform );

	EHANDLE m_hPlatform;
};

#endif //GAME_SERVER_ENTITIES_PLATS_CPLATTRIGGER_H