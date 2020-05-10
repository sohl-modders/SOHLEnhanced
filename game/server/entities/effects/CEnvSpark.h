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
#ifndef GAME_SERVER_ENTITIES_EFFECTS_CENVSPARK_H
#define GAME_SERVER_ENTITIES_EFFECTS_CENVSPARK_H

enum EnvSparkSpawnFlag
{
	SF_SPARK_TOGGLE		= 1 << 5,
	SF_SPARK_START_ON	= 1 << 6
};

//----------------------------------------------------------------
// Spark
//----------------------------------------------------------------
class CEnvSpark : public CBaseEntity
{
public:
	DECLARE_CLASS( CEnvSpark, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	Precache( void ) override;
	void	SparkThink( void );
	void	SparkStart( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	SparkStop( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	KeyValue( KeyValueData *pkvd ) override;

	float	m_flDelay;
};

#endif //GAME_SERVER_ENTITIES_EFFECTS_CENVSPARK_H