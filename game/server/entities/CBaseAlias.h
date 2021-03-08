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
#ifndef GAME_SERVER_CBASEALIAS_H
#define GAME_SERVER_CBASEALIAS_H

class CBaseAlias : public CPointEntity
{
public:
	DECLARE_CLASS(CBaseAlias, CPointEntity);
	DECLARE_DATADESC();
	
	bool IsAlias() override
	{
		return true;
	}
	
	virtual CBaseEntity* FollowAlias(CBaseEntity* pFrom)
	{
		return nullptr;
	}
	
	virtual void ChangeValue(int iszValue)
	{
		ALERT(at_error, "%s entities cannot change value!", GetClassname());
	}
	
	virtual void ChangeValue(CBaseEntity* pValue)
	{
		ChangeValue(pValue->pev->targetname);
	}
	
	virtual void FlushChanges() {}

	CBaseAlias* m_pNextAlias;
};

#endif //GAME_SERVER_CBASEALIAS_H