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
	DECLARE_CLASS(CLight, CPointEntity);
	DECLARE_DATADESC();

	virtual void KeyValue(KeyValueData* pkvd) override;
	virtual void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	void Think() override;

	virtual STATE GetState() { return m_iState; };
	int	GetStyle() { return m_iszCurrentStyle; };
	void SetStyle(int iszPattern);

	void SetCorrectStyle();
private:
	STATE	m_iState; // current state
	int		m_iOnStyle; // style to use while on
	int		m_iOffStyle; // style to use while off
	int		m_iTurnOnStyle; // style to use while turning on
	int		m_iTurnOffStyle; // style to use while turning off
	int		m_iTurnOnTime; // time taken to turn on
	int		m_iTurnOffTime; // time taken to turn off
	int		m_iszPattern; // custom style to use while on
	int		m_iszCurrentStyle; // current style string
};

#endif //GAME_SERVER_ENTITIES_LIGHTS_CLIGHT_H