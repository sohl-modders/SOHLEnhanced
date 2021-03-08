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
#ifndef GAME_SERVER_ENTITIES_LIGHTS_CLIGHTFADER_H
#define GAME_SERVER_ENTITIES_LIGHTS_CLIGHTFADER_H

class CLightFader : public CPointEntity
{
public:
	DECLARE_CLASS(CLightFader, CPointEntity);
	DECLARE_DATADESC();
	
	void FadeThink();
	void WaitThink();

	CLight* m_pLight;
	char m_cFrom;
	char m_cTo;
	char m_szCurStyle[2];
	float m_fEndTime;
	int m_iszPattern;
	float m_fStep;
	int m_iWait;
};
#endif //GAME_SERVER_ENTITIES_LIGHTS_CLIGHTFADER_H