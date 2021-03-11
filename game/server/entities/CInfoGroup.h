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
#ifndef GAME_SERVER_CINFOGROUP_H
#define GAME_SERVER_CINFOGROUP_H

#define SF_GROUP_DEBUG 2
#define MAX_MULTI_TARGETS_IT	32

class CInfoGroup : public CPointEntity
{
public:
	DECLARE_CLASS(CInfoGroup, CPointEntity);
	DECLARE_DATADESC();
	
	void KeyValue(KeyValueData* pkvd) override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
	int GetMember(const char* szMemberName);

	int		m_cMembers;
	int		m_iszMemberName[MAX_MULTI_TARGETS_IT];
	int		m_iszMemberValue[MAX_MULTI_TARGETS_IT];
	int		m_iszDefaultMember;
};

#endif //GAME_SERVER_CINFOGROUP_H