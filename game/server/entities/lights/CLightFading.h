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
#ifndef GAME_SERVER_ENTITIES_LIGHTS_CLIGHTFADING_H
#define GAME_SERVER_ENTITIES_LIGHTS_CLIGHTFADING_H

enum LightModes
{
	Light_Off,
	Light_On
};

class CLightFading : public CLight
{
public:
	DECLARE_CLASS(CLightFading, CLight);
	DECLARE_DATADESC();
	
	void		Spawn() override;
	void		KeyValue(KeyValueData* pkvd) override;
	void		Use(CBaseEntity* activator, CBaseEntity* caller, USE_TYPE useType, float value) override;
	void		Think() override;

private:
	int			lightMode;
	char		lightIntensity[2];
	float		lightUpdateTime{ 0.1f };
};

#endif //GAME_SERVER_ENTITIES_LIGHTS_CLIGHTFADING_H