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
#ifndef GAME_SERVER_ENTITIES_PLATS_CFUNCPLAT_H
#define GAME_SERVER_ENTITIES_PLATS_CFUNCPLAT_H

#include "CBasePlatTrain.h"

/*QUAKED func_plat (0 .5 .8) ? PLAT_LOW_TRIGGER
speed	default 150

Plats are always drawn in the extended position, so they will light correctly.

If the plat is the target of another trigger or button, it will start out disabled in
the extended position until it is trigger, when it will lower and become a normal plat.

If the "height" key is set, that will determine the amount the plat moves, instead of
being implicitly determined by the model's height.

Set "sounds" to one of the following:
1) base fast
2) chain slow
*/
class CFuncPlat : public CBasePlatTrain
{
public:
	DECLARE_CLASS( CFuncPlat, CBasePlatTrain );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	void Setup( void );

	virtual void Blocked( CBaseEntity *pOther ) override;


	void PlatUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	void	CallGoDown( void ) { GoDown(); }
	void	CallHitTop( void ) { HitTop(); }
	void	CallHitBottom( void ) { HitBottom(); }

	virtual void GoUp( void );
	virtual void GoDown( void );
	virtual void HitTop( void );
	virtual void HitBottom( void );
};

#endif //GAME_SERVER_ENTITIES_PLATS_CFUNCPLAT_H