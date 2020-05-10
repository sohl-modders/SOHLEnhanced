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
#ifndef GAME_SERVER_CMOMENTARYDOOR_H
#define GAME_SERVER_CMOMENTARYDOOR_H

class CMomentaryDoor : public CBaseToggle
{
public:
	DECLARE_CLASS( CMomentaryDoor, CBaseToggle );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void Precache( void ) override;

	void	KeyValue( KeyValueData *pkvd ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	virtual int	ObjectCaps() const override { return CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	void DoorMoveDone( void );

	byte	m_bMoveSnd;			// sound a door makes while moving	
};


#endif //GAME_SERVER_CMOMENTARYDOOR_H