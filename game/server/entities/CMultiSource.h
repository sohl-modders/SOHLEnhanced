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
#ifndef GAME_SERVER_CMULTISOURCE_H
#define GAME_SERVER_CMULTISOURCE_H

//
// MultiSouce
//

#define MS_MAX_TARGETS 32

class CMultiSource : public CPointEntity
{
public:
	DECLARE_CLASS( CMultiSource, CPointEntity );
	DECLARE_DATADESC();

	void Spawn() override;
	void KeyValue( KeyValueData *pkvd ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	int	ObjectCaps() const override { return ( CPointEntity::ObjectCaps() | FCAP_MASTER ); }
	bool IsTriggered( const CBaseEntity* const pActivator ) const override;
	void Register( void );

	EHANDLE		m_rgEntities[ MS_MAX_TARGETS ];
	int			m_rgTriggered[ MS_MAX_TARGETS ];

	int			m_iTotal;
	string_t	m_globalstate;
};

#endif //GAME_SERVER_CMULTISOURCE_H