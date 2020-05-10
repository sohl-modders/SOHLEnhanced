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
#ifndef GAME_SERVER_ENTITIES_BMODELS_CFUNCILLUSIONARY_H
#define GAME_SERVER_ENTITIES_BMODELS_CFUNCILLUSIONARY_H

/*QUAKED func_illusionary (0 .5 .8) ?
A simple entity that looks solid but lets you walk through it.
*/
class CFuncIllusionary : public CBaseToggle
{
public:
	DECLARE_CLASS( CFuncIllusionary, CBaseToggle );

	void Spawn( void ) override;
	void KeyValue( KeyValueData *pkvd ) override;
	virtual int	ObjectCaps() const override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
};

#endif //GAME_SERVER_ENTITIES_BMODELS_CFUNCILLUSIONARY_H