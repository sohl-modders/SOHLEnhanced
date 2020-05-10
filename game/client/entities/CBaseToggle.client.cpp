#include "extdll.h"
#include "util.h"
#include "cbase.h"

void CBaseToggle::KeyValue( KeyValueData* )
{
}

bool CBaseToggle::IsLockedByMaster() const
{
	return false;
}

void CBaseToggle::LinearMoveDone()
{
}

void CBaseToggle::AngularMoveDone()
{
}
