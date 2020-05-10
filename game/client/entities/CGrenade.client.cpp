#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

void CGrenade::Spawn()
{
}

CGrenade* CGrenade::ShootTimed( CBaseEntity* pOwner, Vector vecStart, Vector vecVelocity, float time )
{
	return 0;
}

CGrenade* CGrenade::ShootContact( CBaseEntity* pOwner, Vector vecStart, Vector vecVelocity )
{
	return 0;
}

void CGrenade::Explode( Vector, Vector )
{
}

void CGrenade::Explode( TraceResult*, int )
{
}

void CGrenade::DetonateUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value )
{
}

void CGrenade::BounceSound()
{
}

void CGrenade::Killed( const CTakeDamageInfo& info, GibAction gibAction )
{
}