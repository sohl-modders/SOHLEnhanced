#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerTeleport.h"

LINK_ENTITY_TO_CLASS( trigger_teleport, CTriggerTeleport );

//TODO: Consider making this its own class - Solokiller
LINK_ENTITY_TO_CLASS( info_teleport_destination, CPointEntity );

void CTriggerTeleport::Spawn( void )
{
	InitTrigger();

	SetTouch( &CTriggerTeleport::TeleportTouch );
}