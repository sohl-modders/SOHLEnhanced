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
#ifndef GAME_SERVER_CWORLD_H
#define GAME_SERVER_CWORLD_H

#define SF_WORLD_DARK		0x0001		// Fade from black at startup
#define SF_WORLD_TITLE		0x0002		// Display game title at startup
#define SF_WORLD_FORCETEAM	0x0004		// Force teams

// this moved here from world.cpp, to allow classes to be derived from it
//=======================
// CWorld
//
// This spawns first when each level begins.
//=======================
class CWorld : public CBaseEntity
{
public:

	/**
	*	The world always has model index 1.
	*/
	static const int WORLD_MODELINDEX = 1;

public:
	DECLARE_CLASS( CWorld, CBaseEntity );
	DECLARE_DATADESC();

	void OnCreate() override;

	void OnDestroy() override;

	void Spawn() override;
	void Precache() override;
	void KeyValue( KeyValueData *pkvd ) override;

	bool Save( CSave& save ) override;

	bool Restore( CRestore& restore ) override;

	/**
	*	Gets the global instance of this entity.
	*/
	static CWorld* GetInstance()
	{
		return m_pInstance;
	}

private:
	void LoadGMR();

private:
	static CWorld* m_pInstance;

	string_t m_iszMapScript = iStringNull;
	string_t m_iszGMR = iStringNull;
};

#endif //GAME_SERVER_CWORLD_H