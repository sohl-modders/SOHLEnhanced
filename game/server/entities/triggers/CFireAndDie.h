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
#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CFIREANDDIE_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CFIREANDDIE_H

// Fires a target after level transition and then dies
class CFireAndDie : public CBaseDelay
{
public:
	DECLARE_CLASS( CFireAndDie, CBaseDelay );

	void Spawn() override;
	void Precache() override;
	void Think() override;
	int ObjectCaps() const override { return CBaseDelay::ObjectCaps() | FCAP_FORCE_TRANSITION; }	// Always go across transitions
};

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CFIREANDDIE_H