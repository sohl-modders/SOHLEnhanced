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
#ifndef GAME_SERVER_ENTITIES_CLASERSPOT_H
#define GAME_SERVER_ENTITIES_CLASERSPOT_H

class CLaserSpot : public CBaseEntity
{
public:
	DECLARE_CLASS( CLaserSpot, CBaseEntity );
	DECLARE_DATADESC();

	void Spawn() override;
	void Precache() override;

	int	ObjectCaps() const override { return FCAP_DONT_SAVE; }

	void Suspend( float flSuspendTime );
	void Revive();

	static CLaserSpot* CreateSpot();
};

#endif //GAME_SERVER_ENTITIES_CLASERSPOT_H