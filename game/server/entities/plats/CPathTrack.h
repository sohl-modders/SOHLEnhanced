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
#ifndef GAME_SERVER_ENTITIES_PLATS_CPATHTRACK_H
#define GAME_SERVER_ENTITIES_PLATS_CPATHTRACK_H

// Spawnflag for CPathTrack
#define SF_PATH_DISABLED		0x00000001
#define SF_PATH_FIREONCE		0x00000002
#define SF_PATH_ALTREVERSE		0x00000004
#define SF_PATH_DISABLE_TRAIN	0x00000008
#define SF_PATH_ALTERNATE		0x00008000

//#define PATH_SPARKLE_DEBUG		1	// This makes a particle effect around path_track entities for debugging
class CPathTrack : public CPointEntity
{
public:
	DECLARE_CLASS( CPathTrack, CPointEntity );
	DECLARE_DATADESC();

	void		Spawn( void ) override;
	void		Activate( void ) override;
	void		KeyValue( KeyValueData* pkvd ) override;

	void		SetPrevious( CPathTrack *pprevious );
	void		Link( void );
	void		Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

	CPathTrack	*ValidPath( CPathTrack *ppath, const bool bTestFlag );		// Returns ppath if enabled, NULL otherwise
	void		Project( CPathTrack *pstart, CPathTrack *pend, Vector& origin, float dist );

	static CPathTrack* Instance( CBaseEntity* pEntity );

	CPathTrack	*LookAhead( Vector& origin, float dist, const bool bMove );
	CPathTrack	*Nearest( Vector origin );

	CPathTrack	*GetNext( void );
	CPathTrack	*GetPrevious( void );

#if PATH_SPARKLE_DEBUG
	void Sparkle( void );
#endif

	float		m_length;
	string_t	m_altName;
	CPathTrack	*m_pnext;
	CPathTrack	*m_pprevious;
	CPathTrack	*m_paltpath;
};

#endif //GAME_SERVER_ENTITIES_PLATS_CPATHTRACK_H