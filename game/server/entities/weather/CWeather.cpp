#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "entities/triggers/CBaseTrigger.h"

#include "CWeather.h"

LINK_ENTITY_TO_CLASS( env_rain, CWeather );
LINK_ENTITY_TO_CLASS( env_snow, CWeather );

void CWeather::Spawn()
{
	InitTrigger();
}