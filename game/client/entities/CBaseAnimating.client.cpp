#include "extdll.h"
#include "util.h"
#include "cbase.h"

float CBaseAnimating::StudioFrameAdvance( float flInterval )
{
	return 0.0;
}

int CBaseAnimating::GetSequenceFlags() const
{
	return 0;
}

int CBaseAnimating::LookupActivity( int activity )
{
	return 0;
}

int CBaseAnimating::LookupActivityHeaviest( int activity )
{
	return 0;
}

int CBaseAnimating::LookupSequence( const char* label )
{
	return 0;
}

void CBaseAnimating::ResetSequenceInfo()
{
}

void CBaseAnimating::DispatchAnimEvents( float flInterval )
{
}

float CBaseAnimating::SetBoneController( int iController, float flValue )
{
	return 0.0;
}

void CBaseAnimating::InitBoneControllers()
{
}

float CBaseAnimating::SetBlending( int iBlender, float flValue )
{
	return 0;
}

void CBaseAnimating::GetBonePosition( int iBone, Vector& origin, Vector& angles )
{
}

int CBaseAnimating::FindTransition( int iEndingSequence, int iGoalSequence, int *piDir )
{
	return -1;
}

void CBaseAnimating::GetAttachment( int iAttachment, Vector& origin, Vector& angles )
{
}

void CBaseAnimating::SetBodygroup( int iGroup, int iValue )
{
}

int CBaseAnimating::GetBodygroup( int iGroup )
{
	return 0;
}