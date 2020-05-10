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
#ifndef GAME_SERVER_ENTITIES_CRPGROCKET_H
#define GAME_SERVER_ENTITIES_CRPGROCKET_H

class CRpg;

class CRpgRocket : public CGrenade
{
public:
	DECLARE_CLASS( CRpgRocket, CGrenade );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	void FollowThink( void );
	void IgniteThink( void );
	void RocketTouch( CBaseEntity *pOther );
	static CRpgRocket *CreateRpgRocket( Vector vecOrigin, Vector vecAngles, CBaseEntity *pOwner, CRpg *pLauncher );

	int m_iTrail;
	float m_flIgniteTime;
	EHANDLE m_hLauncher;// handle back to the launcher that fired me. 
};

#endif //GAME_SERVER_ENTITIES_CRPGROCKET_H