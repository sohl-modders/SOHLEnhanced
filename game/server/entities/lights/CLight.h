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
#ifndef GAME_SERVER_ENTITIES_LIGHTS_CLIGHT_H
#define GAME_SERVER_ENTITIES_LIGHTS_CLIGHT_H

#define SF_LIGHT_START_OFF		1

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) LIGHT_START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/
class CLight : public CPointEntity
{
public:
	DECLARE_CLASS( CLight, CPointEntity );
	DECLARE_DATADESC();

	virtual void	KeyValue( KeyValueData* pkvd ) override;
	virtual void	Spawn( void ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;

private:
	int		m_iStyle;
	int		m_iszPattern;
};

#endif //GAME_SERVER_ENTITIES_LIGHTS_CLIGHT_H