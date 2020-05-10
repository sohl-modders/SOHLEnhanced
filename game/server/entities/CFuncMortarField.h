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
#ifndef GAME_SERVER_ENTITIES_CFUNCMORTARFIELD_H
#define GAME_SERVER_ENTITIES_CFUNCMORTARFIELD_H

/*

===== mortar.cpp ========================================================

the "LaBuznik" mortar device

*/
class CFuncMortarField : public CBaseToggle
{
public:
	DECLARE_CLASS( CFuncMortarField, CBaseToggle );
	DECLARE_DATADESC();

	void Spawn() override;
	void Precache() override;
	void KeyValue( KeyValueData *pkvd ) override;

	// Bmodels don't go across transitions
	virtual int	ObjectCaps() const override { return CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	void FieldUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	int m_iszXController;
	int m_iszYController;
	float m_flSpread;
	float m_flDelay;
	int m_iCount;
	int m_fControl;
};

#endif //GAME_SERVER_ENTITIES_CFUNCMORTARFIELD_H