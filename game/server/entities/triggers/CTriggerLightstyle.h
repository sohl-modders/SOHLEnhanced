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
#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERLIGHTSTYLE_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERLIGHTSTYLE_H

class CTriggerLightstyle : public CPointEntity
{
public:
	DECLARE_CLASS(CTriggerLightstyle, CPointEntity);
	DECLARE_DATADESC();

	void KeyValue(KeyValueData* pkvd) override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

private:
	int m_iszPattern;
	int m_iFade;
	int m_iWait;
};

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CTRIGGERLIGHTSTYLE_H
