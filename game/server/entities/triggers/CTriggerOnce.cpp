#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerOnce.h"

LINK_ENTITY_TO_CLASS( trigger_once, CTriggerOnce );

void CTriggerOnce::Spawn( void )
{
	m_flWait = -1;

	CTriggerMultiple::Spawn();
}