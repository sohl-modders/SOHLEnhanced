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
#ifndef GAME_SERVER_ENTITIES_TANKS_CFUNCTANKCONTROLS_H
#define GAME_SERVER_ENTITIES_TANKS_CFUNCTANKCONTROLS_H

class CFuncTank;

//============================================================================
// FUNC TANK CONTROLS
//============================================================================
class CFuncTankControls : public CBaseEntity
{
public:
	DECLARE_CLASS( CFuncTankControls, CBaseEntity );
	DECLARE_DATADESC();

	virtual int	ObjectCaps() const override;
	void Spawn() override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void Think() override;

	CFuncTank *m_pTank;
};

#endif //GAME_SERVER_ENTITIES_TANKS_CFUNCTANKCONTROLS_H