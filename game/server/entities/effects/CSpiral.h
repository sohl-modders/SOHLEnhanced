/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CSPIRAL_H
#define GAME_SERVER_ENTITIES_EFFECTS_CSPIRAL_H

#define SPIRAL_INTERVAL		0.1 //025

// Spiral Effect
class CSpiral : public CBaseEntity
{
public:
	DECLARE_CLASS( CSpiral, CBaseEntity );

	void Spawn() override;
	void Think() override;
	int ObjectCaps() const override { return FCAP_DONT_SAVE; }
	static CSpiral *Create( const Vector &origin, float height, float radius, float duration );
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CSPIRAL_H