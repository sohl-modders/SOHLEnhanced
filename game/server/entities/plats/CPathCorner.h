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
#ifndef GAME_SERVER_ENTITIES_PLATS_CPATHCORNER_H
#define GAME_SERVER_ENTITIES_PLATS_CPATHCORNER_H

// Spawnflags of CPathCorner
#define SF_CORNER_WAITFORTRIG	0x001
#define SF_CORNER_TELEPORT		0x002
#define SF_CORNER_FIREONCE		0x004

class CPathCorner : public CPointEntity
{
public:
	DECLARE_CLASS( CPathCorner, CPointEntity );
	DECLARE_DATADESC();

	void Spawn() override;
	void KeyValue( KeyValueData* pkvd ) override;
	float GetDelay( void ) override { return m_flWait; }
	//	void Touch( CBaseEntity *pOther ) override;

private:
	float	m_flWait;
};

#endif //GAME_SERVER_ENTITIES_PLATS_CPATHCORNER_H