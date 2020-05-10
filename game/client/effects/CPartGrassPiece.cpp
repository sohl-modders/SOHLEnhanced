#include "hud.h"
#include "cl_util.h"

#include "CPartGrassPiece.h"

void CPartGrassPiece::Think( float flTime )
{
	cl_entity_t* pPlayer = gEngfuncs.GetLocalPlayer();

	Vector vecDist = m_vOrigin - pPlayer->origin;

	const float flDist = vecDist.Length();

	if( flDist > 855.0 )
		m_flDieTime = flTime;

	if( flDist > 500.0 )
	{
		float flRemainder = 500.0 - flDist;

		if( flRemainder < 0.0 )
			flRemainder = -flRemainder;

		float flBrightness = 255.0 - flRemainder;

		m_flBrightness = flBrightness > 0.0 ? flBrightness : 1.0;
	}

	CBaseParticle::Think( flTime );
}