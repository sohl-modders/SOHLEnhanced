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
#ifndef GAME_SERVER_ENTITIES_LIGHTS_CLIGHTDYNAMIC_H
#define GAME_SERVER_ENTITIES_LIGHTS_CLIGHTDYNAMIC_H

#define SF_LIGHTDYNAMIC_START_OFF	1
#define SF_LIGHTDYNAMIC_FLARE		2

class CLightDynamic : public CBaseEntity
{
public:
	DECLARE_CLASS(CLightDynamic, CBaseEntity);

	void Spawn() override;
	void Precache() override;
	
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	int ObjectCaps() { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	
	void SetEffects();
	STATE GetState() {
		return m_iState;
	}
private:
	STATE m_iState; // current state
};

#endif //GAME_SERVER_ENTITIES_LIGHTS_CLIGHTDYNAMIC_H