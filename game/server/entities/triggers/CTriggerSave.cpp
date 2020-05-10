#include "extdll.h"
#include "util.h"
#include "gamerules/GameRules.h"
#include "cbase.h"

#include "CTriggerSave.h"

BEGIN_DATADESC( CTriggerSave )
	DEFINE_TOUCHFUNC( SaveTouch ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_autosave, CTriggerSave );

void CTriggerSave::Spawn( void )
{
	if( g_pGameRules->IsDeathmatch() )
	{
		UTIL_RemoveNow( this );
		return;
	}

	InitTrigger();
	SetTouch( &CTriggerSave::SaveTouch );
}

void CTriggerSave::SaveTouch( CBaseEntity *pOther )
{
	if( !UTIL_IsMasterTriggered( m_sMaster, pOther ) )
		return;

	// Only save on clients
	if( !pOther->IsPlayer() )
		return;

	SetTouch( NULL );
	UTIL_Remove( this );
	SERVER_COMMAND( "autosave\n" );
}