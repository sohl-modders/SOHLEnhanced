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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CGIBSHOOTER_H
#define GAME_SERVER_ENTITIES_EFFECTS_CGIBSHOOTER_H

class CGib;

#define	SF_GIBSHOOTER_REPEATABLE	1 // allows a gibshooter to be refired

class CGibShooter : public CBaseDelay
{
public:
	DECLARE_CLASS( CGibShooter, CBaseDelay );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	Precache( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;
	void ShootThink( void );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	virtual CGib *CreateGib( void );

	int	m_iGibs;
	int m_iGibCapacity;
	int m_iGibMaterial;
	int m_iGibModelIndex;
	float m_flGibVelocity;
	float m_flVariance;
	float m_flGibLife;
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CGIBSHOOTER_H