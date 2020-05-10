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
#ifndef GAME_SERVER_ENTITIES_CBUTTONTARGET_H
#define GAME_SERVER_ENTITIES_CBUTTONTARGET_H

#define SF_BTARGET_USE		0x0001
#define SF_BTARGET_ON		0x0002

class CButtonTarget : public CBaseEntity
{
public:
	DECLARE_CLASS( CButtonTarget, CBaseEntity );

	void Spawn( void ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	void OnTakeDamage( const CTakeDamageInfo& info ) override;
	int	ObjectCaps() const override;
};

#endif //GAME_SERVER_ENTITIES_CBUTTONTARGET_H