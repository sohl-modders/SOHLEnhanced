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
#ifndef GAME_SERVER_ENTITIES_XEN_CXENTREETRIGGER_H
#define GAME_SERVER_ENTITIES_XEN_CXENTREETRIGGER_H

class CXenTreeTrigger : public CBaseEntity
{
public:
	DECLARE_CLASS( CXenTreeTrigger, CBaseEntity );

	void		Touch( CBaseEntity *pOther ) override;
	static CXenTreeTrigger *TriggerCreate( CBaseEntity* pOwner, const Vector &position );
};

#endif //GAME_SERVER_ENTITIES_XEN_CXENTREETRIGGER_H