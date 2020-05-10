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
#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CXenTreeTrigger.h"

LINK_ENTITY_TO_CLASS( xen_ttrigger, CXenTreeTrigger );

void CXenTreeTrigger::Touch( CBaseEntity *pOther )
{
	if( GetOwner() )
	{
		CBaseEntity *pEntity = GetOwner();
		pEntity->Touch( pOther );
	}
}

CXenTreeTrigger *CXenTreeTrigger::TriggerCreate( CBaseEntity* pOwner, const Vector &position )
{
	auto pTrigger = static_cast<CXenTreeTrigger*>( UTIL_CreateNamedEntity( "xen_ttrigger" ) );
	pTrigger->SetAbsOrigin( position );
	pTrigger->SetSolidType( SOLID_TRIGGER );
	pTrigger->SetMoveType( MOVETYPE_NONE );
	pTrigger->SetOwner( pOwner );

	return pTrigger;
}