#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerCounter.h"

LINK_ENTITY_TO_CLASS( trigger_counter, CTriggerCounter );

void CTriggerCounter::Spawn( void )
{
	// By making the flWait be -1, this counter-trigger will disappear after it's activated
	// (but of course it needs cTriggersLeft "uses" before that happens).
	m_flWait = -1;

	if( m_cTriggersLeft == 0 )
		m_cTriggersLeft = 2;
	SetUse( &CTriggerCounter::CounterUse );
}