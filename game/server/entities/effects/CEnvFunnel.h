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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CENVFUNNEL_H
#define GAME_SERVER_ENTITIES_EFFECTS_CENVFUNNEL_H

#define SF_FUNNEL_REVERSE			1 // funnel effect repels particles instead of attracting them.

//=========================================================
// FunnelEffect
//=========================================================
class CEnvFunnel : public CBaseDelay
{
public:
	DECLARE_CLASS( CEnvFunnel, CBaseDelay );

	void	Spawn( void ) override;
	void	Precache( void ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	int		m_iSprite;	// Don't save, precache
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CENVFUNNEL_H