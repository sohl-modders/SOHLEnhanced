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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CBUBBLING_H
#define GAME_SERVER_ENTITIES_EFFECTS_CBUBBLING_H

#define SF_BUBBLES_STARTOFF		0x0001

class CBubbling : public CBaseEntity
{
public:
	DECLARE_CLASS( CBubbling, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	Precache( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;

	void	FizzThink( void );
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	virtual int		ObjectCaps() const override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	int		m_density;
	int		m_frequency;
	int		m_bubbleModel;
	bool	m_state;
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CBUBBLING_H